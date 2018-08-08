#pragma once


namespace cluster {
    // Helpers.
    // Open a handle to a library.
    cluster::handler library_open(const std::string& filename) {
        auto handle = dlopen(filename.c_str(), RTLD_NOW);

        if (not handle) throw cluster::ErrorLoadSharedLibrary();

        return handle;
    }


    // Find a symbol within the library.
    template <typename F>
    F library_find(
        cluster::handler library,
        const std::string& symbol
    ) {
        F fn = reinterpret_cast<F>(
            dlsym(library, symbol.c_str())
        );

        char* error;

        if ((error = dlerror())) throw cluster::ErrorLoadSymbol();

        return fn;
    }


    // Close the library handle.
    void library_close(cluster::handler library) {
        if (static_cast<bool>(dlclose(library)))
            throw cluster::ErrorUnloadSharedLibrary();
    }



    // Program handler.
    // Constructor.
    Program::Program(const std::string& filename):
        library(cluster::library_open(filename), cluster::library_close)
    {

    }


    // Run the program.
    void Program::run(cluster::identity id, cluster::arguments args) {
        cluster::library_find<cluster::MAIN_SIGNATURE>(
            library.get(), cluster::ENTRY_POINT
        )(id, args);
    }
}