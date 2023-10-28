cmake_minimum_required(VERSION 3.15)

project(matrix_chain)

set(MATRIX_DIR ${PROJECT_SOURCE_DIR}/Matrix/include)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include ${MATRIX_DIR})

aux_source_directory(./src/ SRC_DIR)
add_executable(${PROJECT_NAME} ${SRC_DIR})

target_include_directories( ${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} )

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

option(END2END_TESTING "build for testing" OFF)
if (END2END_TESTING)
    target_compile_definitions( ${PROJECT_NAME}
                                PRIVATE
                                END2END_TESTING
                              )
endif()

find_package(GTest REQUIRED)
enable_testing()

find_package(Threads REQUIRED)

add_subdirectory(Matrix)
add_subdirectory(tests)
