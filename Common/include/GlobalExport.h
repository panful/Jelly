#pragma once

#if defined(_WIN32)
    #if defined(JELLY_SHARED_LIBRARY)
        #define JELLY_EXPORT __declspec(dllexport)
    #else
        #define JELLY_EXPORT
    #endif
#else
    #define JELLY_EXPORT
#endif
