# Clusterize
An SSH library for managing tasks across a botnet.
######Example usage:

```
net = clusterize()
net.client("192.168.0.23","user","pass") # SSH details.
net.connect() # Connect to all the clients that have been added to pool.

def test(x,y):
    return x+y # Define your function to be run on the machine(s) you desire.

task = net.task(test,[5,5]) # Create a task on one of the computers in the pool. task(func,args,machine_id)

# Tasks are multithreaded so you can do other stuff before calling the results.

print(net.result(task)) # Get the results of the task when you're ready. In this case, It prints the results.
net.end() # Close all the connections safely.
```

This code is provided under the MIT License. Check "license.md" for more details.
