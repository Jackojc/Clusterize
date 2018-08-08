#include "clusterize/clusterize.h"


int main(int argc, char const *argv[]) {
    // Load the program.
    cluster::Program prog("./sum.so");

    // Run the program, passing in the following arguments.
    prog.run(121470, {"Hello", " ", "there!", "\n"});

    return 0;
}