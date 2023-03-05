cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ML_MLCS)


include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(lib COMMON_SRCS)

find_package(Torch REQUIRED)

add_executable(main main.cpp ${COMMON_SRCS})
target_link_libraries(main "${TORCH_LIBRARIES}")
set_property(TARGET main PROPERTY CXX_STANDARD 14)
