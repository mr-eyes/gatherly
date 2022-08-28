# ----------------------------------------------------------------------------
# Gatherly project
# ----------------------------------------------------------------------------

# Minimum cmake version
cmake_minimum_required (VERSION 3.14...3.20)
# Name of project and that it is C/C++.
project (
        Gatherly
        LANGUAGES CXX C
        VERSION 2.0.0
)

set(default_build_type "RELEASE")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fPIC -lgomp -lrt -fopenmp -Ofast")

include_directories("${PROJECT_SOURCE_DIR}/lib/pybind11/include")
include_directories("${PROJECT_SOURCE_DIR}/lib/parallel-hashmap")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(gatherly STATIC src/gatherly.cpp ${PROJECT_SOURCE_DIR}/lib/parallel-hashmap/)
set_target_properties(gatherly PROPERTIES POSITION_INDEPENDENT_CODE 1 CXX_STANDARD 17)
target_link_libraries (gatherly z)
target_link_libraries (gatherly z)
target_include_directories(gatherly INTERFACE ${PROJECT_SOURCE_DIR}/lib/parallel-hashmap)

add_subdirectory("${PROJECT_SOURCE_DIR}/lib/pybind11")
pybind11_add_module(cmake_example src/pygatherly/main.cpp)

add_executable(test test.cpp)
target_link_libraries(test gatherly z)