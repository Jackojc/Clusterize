import pysftp
from paramiko import ssh_exception
from multiprocessing.pool import ThreadPool
import os
import inspect
import time


class clusterize():
    def __init__(self):
        self.Accounts = []
        self.Temp = []
        self.Sessions = []

        self.remote_temp = "/tmp/temp.tmp"
        self.local_temp = "temp.tmp"

        self.Tasks = []
        self.Pool = []

        self.machine_to_run = 0

    def client(self, host, user, passw):
        self.Accounts.append([host, user, passw])

    def connect(self):
        self.Pool = ThreadPool(processes=len(self.Accounts))

        for num, machine in enumerate(self.Accounts):
            self.Temp.append(self.Pool.apply_async(pysftp.Connection, (machine[0], machine[1], None, machine[2])))

        for num, machine in enumerate(self.Temp):
            try:
                self.Sessions.append(machine.get())
            except:
                raise Exception("Error connecting...", self.Accounts[num][0])

    def parallelize(self, target, args=()):
        results = []
        args = list(args)
        args.append(0)

        for num, machine in enumerate(self.Sessions):
            results.append(self.Pool.apply_async(target, tuple(args)))
            args[-1] = num+1

        for num, machine in enumerate(results):
            results[num] = machine.get()

        return results

    def cmd_on(self, cmd, mid):
        return [x.decode().replace("\n", "") for x in self.Sessions[mid].execute(cmd)]

    def read_on(self, mid):
        f = self.Sessions[mid].open(self.remote_temp, "r")
        a = f.read()
        f.close()

        return a.decode()

    def write_on(self, string, mid):
        f = self.Sessions[mid].open(self.remote_temp, "w")
        f.write(string)
        f.close()

    def mkdir_on(self, pathname, mid):
        return self.Sessions[mid].mkdir(pathname)

    def ls_on(self, directory, mid):
        return self.Sessions[mid].listdir()

    def rmf_on(self, directory, mid):
        return self.Sessions[mid].remove(directory)

    def rmd_on(self, directory, mid):
        return self.Sessions[mid].rmdir(directory)

    def dir_on(self, mid):
        return self.Sessions[mid].getcwd()

    def cd_on(self, directory, mid):
        return self.Sessions[mid].chdir(directory)

    def chmod_on(self, path, mode, mid):
        return self.Sessions[mid].chmod(path, mode)

    def end_on(self, mid):
        return self.Sessions[mid].close()

    def cmd(self, cmd):
        return self.parallelize(self.cmd_on, (cmd))

    def end(self):
        return self.parallelize(self.end_on)

    def write(self, string):
        return self.parallelize(self.write_on, (string))

    def get_function(self, file, args):
        source = list(inspect.getsourcelines(file))[0]
        source.append("print({}({}))".format(file.__name__, ",".join([str(x) for x in args])))

        return "".join(source)

    def tasker(self, source, args, mid):
        self.write_on(source, mid)
        return self.cmd_on("sudo python3 {}".format(self.remote_temp), mid)

    def task_all(self, function, args):
        source = self.get_function(function, args)
        return self.parallelize(self.tasker, (source, args))

    def task(self, function, args):
        args = list(args)
        source = self.get_function(function, args)
        self.write_on(source, self.machine_to_run)
        thread = self.Pool.apply_async(self.cmd_on, ("sudo python3 {}".format(self.remote_temp), self.machine_to_run))

        return thread

    def result(self,task):
        if self.machine_to_run == len(self.Accounts)-1:
            self.machine_to_run = 0
        else:
            self.machine_to_run += 1

        return task.get()