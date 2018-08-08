#pragma once


#include <string>
#include <stdexcept>
#include <vector>



// `#define CLUSTERIZE_IMPL_LINUX` to force Linux implementation.
// `#define CLUSTERIZE_IMPL_WINDOWS` to force Windows implementation.



// Constants.
namespace cluster {
    using identity = int;
    using arguments = const std::vector<std::string>&;


    constexpr auto ENTRY_POINT = "CLUSTERIZE_MAIN";
    using MAIN_SIGNATURE = int(*)(identity, arguments);
}



// Macros.
#define CLUSTERIZE_MAIN(id, args) extern "C" int CLUSTERIZE_MAIN(cluster::identity id, cluster::arguments args)



// Include the required headers.
#if defined(linux) || defined(__unix__) || defined(CLUSTERIZE_IMPL_LINUX)
    #define CLUSTERIZE_IMPL_LINUX

    extern "C" {
        #include <dlfcn.h>
    }

#elif defined(_WIN32) || defined(WIN32) || defined(CLUSTERIZE_IMPL_WINDOWS)
    #define CLUSTERIZE_IMPL_WINDOWS

    extern "C" {
        #include <windows.h>
    }

#else
    #error UNSUPPORTED PLATFORM

#endif



// Types.
namespace cluster {
    #if defined(CLUSTERIZE_IMPL_LINUX)
        using handler = void*;
    #elif defined(CLUSTERIZE_IMPL_WINDOWS)
        using handler = HINSTANCE;
    #endif
}



// Prototypes.
#include "src/prototypes.h"



// Error reporting implementation.
namespace cluster {
    #if defined(CLUSTERIZE_IMPL_LINUX)
        std::string get_error() {
            return std::string{ dlerror() };
        }

    #elif defined(CLUSTERIZE_IMPL_WINDOWS)
        std::string get_error() {
            return std::string{ GetLastError() };
        }

    #endif
}



// Exception types.
#define NEW_RUNTIME_EXCEPTION_TYPE(name, default_msg) \
    class name: public std::runtime_error { \
        public: \
            name(const std::string& msg): std::runtime_error(msg) {} \
            name(): std::runtime_error(default_msg) {} \
    };


#define NEW_LOGIC_EXCEPTION_TYPE(name, default_msg) \
    class name: public std::logic_error { \
        public: \
            name(const std::string& msg): std::logic_error(msg) {} \
            name(): std::logic_error(default_msg) {} \
    };


namespace cluster {
    NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadSharedLibrary,
        "Could not load the shared library! " + cluster::get_error()
    );


    NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorLoadSymbol,
        "Could not find the specified symbol! " + cluster::get_error()
    );


    NEW_RUNTIME_EXCEPTION_TYPE(
        ErrorUnloadSharedLibrary,
        "Could not unload the shared library! " + cluster::get_error()
    );
}


#undef NEW_LOGIC_EXCEPTION_TYPE
#undef NEW_RUNTIME_EXCEPTION_TYPE



// Implementation files.
#if defined(CLUSTERIZE_IMPL_LINUX)
    #include "src/linux.h"
#elif defined(CLUSTERIZE_IMPL_WINDOWS)
    #include "src/windows.h"
#endif