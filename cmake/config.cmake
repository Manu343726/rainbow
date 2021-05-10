option(RAINBOW_INSTRUMENTATION "Build with profiling instrumentation enabled" OFF)
option(RAINBOW_ASAN "Build with address sanitizer enabled" OFF)

set(RAINBOW_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(RAINBOW_INCLUDE_DIR "${RAINBOW_ROOT_DIR}/include")

if(NOT DEFINED RAINBOW_CXX_STANDARD)
    set(RAINBOW_CXX_STANDARD 17)
endif()

if(NOT DEFINED RAINBOW_GCC_WARNINGS)
    set(RAINBOW_GCC_WARNINGS "-Wall" "-Wextra" "-pedantic" "-Werror")
endif()

if(NOT DEFINED RAINBOW_GCC_OPTIONS)
    set(RAINBOW_GCC_OPTIONS)
endif()

if(RAINBOW_INSTRUMENTATION)
    list(APPEND RAINBOW_GCC_OPTIONS "-finstrument-functions")
endif()

if(RAINBOW_ASAN)
    list(APPEND RAINBOW_GCC_OPTIONS "-fsanitize=address")
    list(APPEND RAINBOW_GCC_LINK_OPTIONS "-fsanitize=address")
endif()

macro(configure_rainbow_target TARGET)
    set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD ${RAINBOW_CXX_STANDARD}
        INTERPROCEDURAL_OPTIMIZATION ON)

    if(NOT MSVC)
        target_compile_options(${TARGET} PRIVATE ${RAINBOW_GCC_WARNINGS} ${RAINBOW_GCC_OPTIONS})
        target_link_options(${TARGET} PRIVATE ${RAINBOW_GCC_LINK_OPTIONS})
    endif()

    target_include_directories(${TARGET} PUBLIC "${RAINBOW_INCLUDE_DIR}")
endmacro()

macro(add_rainbow_library NAME)
    add_library(${NAME} ${ARGN})
    configure_rainbow_target(${NAME})
endmacro()

macro(add_rainbow_example NAME)
    add_executable(${NAME} ${NAME}.cpp ${ARGN})
    target_link_libraries(${NAME} PRIVATE rainbow)
    configure_rainbow_target(${NAME})

    add_test(${NAME} ${NAME})
endmacro()

macro(add_rainbow_test NAME)
    add_executable(${NAME} ${ARGN})
    target_link_libraries(${NAME} PRIVATE rainbow)
    configure_rainbow_target(${NAME})

    add_test(${NAME} ${NAME})
endmacro()
