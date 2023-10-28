cmake_minimum_required(VERSION 3.15)

project(tests)

aux_source_directory(./unit UNIT)
aux_source_directory(./end2end/src END2END)
aux_source_directory(./compare MULTIPLY_COMPARING)
aux_source_directory(./deque_vector_compare CONTAINERS_COMPARING)

add_executable(unit_chain ${UNIT})
add_executable(end2end2 ${END2END})
add_executable(mul_performance ${MULTIPLY_COMPARING})
add_executable(containers_performance ${CONTAINERS_COMPARING})

target_include_directories( unit_chain PRIVATE ${INCLUDE_DIR} )
target_include_directories( end2end2 PRIVATE end2end/include ${MATRIX_DIR} )
target_include_directories( mul_performance PRIVATE ${INCLUDE_DIR} )
target_include_directories( containers_performance PRIVATE ${MATRIX_DIR} ${INCLUDE_DIR} include)

target_link_libraries( unit_chain PRIVATE ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )

option(EFFECTIVE_MULTIPLY "for choosing effective matrix chain multiply algorithm" OFF)
if (EFFECTIVE_MULTIPLY)
    target_compile_definitions( mul_performance
                                PRIVATE
                                EFFECTIVE_MULTIPLY
                              )
endif()

set_target_properties(
    unit_chain end2end2 mul_performance containers_performance PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
