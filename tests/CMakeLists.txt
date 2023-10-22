cmake_minimum_required(VERSION 3.15)

project(tests)

#enable_testing()

#add_subdirectory(../Matrix/tests/googletest .)

aux_source_directory(./unit UNIT)
aux_source_directory(./end2end/src END2END)
aux_source_directory(./compare PERFORMANCE_COMPARING)

add_executable(unit2 ${UNIT})
add_executable(end2end2 ${END2END})
add_executable(performance ${PERFORMANCE_COMPARING})

target_include_directories( unit2
    #                          PRIVATE googletest/
                            PRIVATE ${INCLUDE_DIR}
                          )

#target_link_libraries(unit gtest_main gmock_main)

target_include_directories( end2end2
                            PRIVATE end2end/include
                            PRIVATE ${INCLUDE_DIR}
                          )

target_include_directories( performance
                            PRIVATE ${INCLUDE_DIR}
                          )

option(EFFECTIVE_MULTIPLY "for choosing effective matrix chain multiply algorithm" OFF)

if (EFFECTIVE_MULTIPLY)
    target_compile_definitions( performance
                                PRIVATE
                                EFFECTIVE_MULTIPLY
                              )
endif()

set_target_properties(
    unit2 end2end2 performance PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

