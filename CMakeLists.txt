cmake_minimum_required(VERSION 3.17)
project(libr_spl)

set(CMAKE_C_STANDARD 17)
if(LINUX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

set(R2_INCLUDE_DIR /usr/local/include CACHE PATH "Path containing 'libr' directory")
set(R2_LIB_DIR /usr/local/lib CACHE PATH "Path containing 'libr_*' libraries")

# For creating object libraries
function(object_from_source_header name)
    add_library(${name} OBJECT "${CMAKE_SOURCE_DIR}/src/r_spl_${name}.c")
    target_include_directories(${name} PUBLIC "${CMAKE_SOURCE_DIR}/include" "${R2_INCLUDE_DIR}/libr")
endfunction()
set(SOURCE_HEADERS core rap)

foreach(source_header ${SOURCE_HEADERS})
    object_from_source_header(${source_header})
    list(APPEND SPLLIB $<TARGET_OBJECTS:${source_header}>)
endforeach()


add_library(r_spl SHARED ${SPLLIB})
target_include_directories(r_spl PUBLIC "${CMAKE_SOURCE_DIR}/include" "${R2_INCLUDE_DIR}/libr")
if (APPLE)
    set(LIBRARY_SUFFIX dylib CACHE STRING "File extension for finding shared libraries")
elseif(WIN32)
    set(LIBRARY_SUFFIX dll CACHE STRING "File extension for finding shared libraries")
else()
    set(LIBRARY_SUFFIX so CACHE STRING "File extension for finding shared libraries")
endif()
file(GLOB R2LIBS "${R2_LIB_DIR}/libr_*.${LIBRARY_SUFFIX}")
target_link_libraries(r_spl ${R2LIBS})

set(LIB_OUT_DIR /usr/local/lib/spl/ CACHE PATH "Path to install libraries")
set(INCLUDE_OUT_DIR /usr/local/include/libr_spl/ CACHE PATH "Path to install headers")
install(TARGETS r_spl DESTINATION "${LIB_OUT_DIR}")
install(DIRECTORY include/ DESTINATION "${INCLUDE_OUT_DIR}")

set(BUILD_EXAMPLE OFF CACHE BOOL "Build example")
if(BUILD_EXAMPLE)
    add_executable(example "${CMAKE_SOURCE_DIR}/src/example.c")
    target_include_directories(example PUBLIC "${CMAKE_SOURCE_DIR}/include" "${R2_INCLUDE_DIR}/libr")
    target_link_libraries(example r_spl "${R2_LIB_DIR}/libr_socket.${LIBRARY_SUFFIX}")
endif()
