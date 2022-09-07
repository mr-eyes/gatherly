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

add_subdirectory("lib/kmerDecoder")
include_directories("${PROJECT_SOURCE_DIR}/lib/cpp-json")
include_directories("${PROJECT_SOURCE_DIR}/lib/zstr/src")



add_library(gatherly STATIC src/gatherly.cpp ${PROJECT_SOURCE_DIR}/lib/parallel-hashmap/)
set_target_properties(gatherly PROPERTIES POSITION_INDEPENDENT_CODE 1 CXX_STANDARD 17)
target_link_libraries (gatherly z kmerDecoder)
target_include_directories(gatherly INTERFACE ${PROJECT_SOURCE_DIR}/lib/parallel-hashmap)

# add_executable(load_index load_index.cpp)
# target_link_libraries(load_index gatherly kmerDecoder z)

# add_executable(test test.cpp)
# target_link_libraries(test gatherly z)