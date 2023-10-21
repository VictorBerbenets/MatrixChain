cmake_minimum_required(VERSION 3.15)

project(matrix_chain)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(MATRIX_DIR ${PROJECT_SOURCE_DIR}/Matrix/include)

aux_source_directory(./src/ SRC_DIR)

option(EFFECTIVE_ORDER "print to stdcout effective multiply order" OFF)
option(EFFECTIVE_MULTIPLY "for fast multiplication of a chain of matrices" OFF)

add_executable(${PROJECT_NAME} ${SRC_DIR})

if (EFFECTIVE_ORDER)
    target_compile_definitions( ${PROJECT_NAME}
                                 PRIVATE
                                 EFFECTIVE_ORDER
                              )
endif()

if (EFFECTIVE_MULTIPLY)
    target_compile_definitions( ${PROJECT_NAME}
                                 PRIVATE
                                 EFFECTIVE_MULTIPLY
                              )
endif()

target_include_directories( ${PROJECT_NAME}
                            PRIVATE
                            ${INCLUDE_DIR}
                            ${MATRIX_DIR}
                            )

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

add_subdirectory(Matrix)
