#pragma once


namespace cluster {
    // Helpers.
    cluster::handler library_open(const std::string& filename) {
        auto handle = LoadLibrary(filename.c_str());

        if (not handle) throw cluster::ErrorLoadSharedLibrary();

        return handle;
    }


    template <typename F>
    std::function<F> library_find(
        cluster::handler library,
        const std::string& symbol
    ) {
        std::function<F> fn = reinterpret_cast<F>(
            GetProcAddress(library, symbol.c_str())
        );

        DWORD error;

        if ((error = GetLastError())) throw cluster::ErrorLoadSymbol();

        return fn;
    }


    void library_close(cluster::handler library) {
        if (not static_cast<bool>(FreeLibrary(library)))
            throw cluster::ErrorUnloadSharedLibrary();
    }



    // Program handler.
    template <>
    Program<std::remove_pointer<cluster::handler>::type>::Program(
        const std::string& filename
    ):
        library(nullptr, cluster::library_close)
    {
        library.reset(cluster::library_open(filename), cluster::library_close);
    }
}

