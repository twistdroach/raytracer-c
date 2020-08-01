add_library(Unity STATIC
    Unity/src/unity.c
)

target_include_directories(Unity PUBLIC
    Unity/src
)

add_library(CException STATIC
    CException/lib/CException.c
)

target_include_directories(CException PUBLIC
    CException/lib
)
