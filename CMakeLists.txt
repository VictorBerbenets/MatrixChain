cmake_minimum_required(VERSION 3.15)

project(matrix_chain)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(MATRIX_DIR ${PROJECT_SOURCE_DIR}/Matrix/include)

aux_source_directory(./src/ SRC_DIR)

add_executable(${PROJECT_NAME} ${SRC_DIR})

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
