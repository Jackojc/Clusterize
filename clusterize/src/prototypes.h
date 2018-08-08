#pragma once


#include <memory>
#include <string>
#include <type_traits>
#include <functional>


namespace cluster {
    // Error handling.
    std::string get_error();



    // Helpers.
    // Open a handle to a library.
    cluster::handler library_open(const std::string&);

    // Find a symbol within the library.
    template <typename F>
    F library_find(cluster::handler, const std::string&);

    // Close the library handle.
    void library_close(cluster::handler);



    // Program handler.
    struct Program {
        // Pointer to shared library.
        std::shared_ptr<std::remove_pointer<cluster::handler>::type> library;


        // Constructor.
        Program(const std::string&);


        // Run the program via it's entry point.
        void run(cluster::identity, cluster::arguments);
    };
}

