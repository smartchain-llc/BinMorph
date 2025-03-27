file(GLOB_RECURSE TEST_SRCS "*.cxx")

foreach(TESTCASE_SRC ${TEST_SRCS})
    get_filename_component(TCFILE ${TESTCASE_SRC} NAME_WE)
    add_executable(${TCFILE} ${TESTCASE_SRC})
    target_link_libraries(
        ${TCFILE}
        GTest::gtest_main
        bm
    )
    target_include_directories(
        ${TCFILE}
        PUBLIC ../../core
        PUBLIC ../../thirdparty
    )
endforeach()
