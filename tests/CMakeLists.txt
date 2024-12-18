set(BINMORPH_TEST_UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

file(GLOB_RECURSE TEST_INCLUDES "./*.h")
foreach(HEADER ${TEST_INCLUDES})
    get_filename_component(HEADER_NAME ${HEADER} NAME_WE)
    set(HEADER_${HEADER_NAME} ${HEADER})
    message("== [BinMorph]<Tests> HEADER_${HEADER_NAME}")
endforeach()


configure_file(TestUtils.h.in ./TestUtils.h)

file(GLOB TEST_CATEGORY "./*")

foreach(DIR ${TEST_CATEGORY})
    file(GLOB_RECURSE TEST_SRCS "${DIR}/*.cxx")

    foreach(TEST ${TEST_SRCS})
        if(NOT IS_DIRECTORY ${TEST})
            get_filename_component(EXEC_NAME ${TEST} NAME_WE)
            set(EXEC_TARGET "TEST_${EXEC_NAME}")
            message("TEST: ${TEST} => ${EXEC_TARGET}")

            add_executable(
                "${EXEC_TARGET}"
                ${TEST}
            )
            target_link_libraries(
                ${EXEC_TARGET}
                binmorph
                GTest::gtest_main
            )
            target_include_directories(
                ${EXEC_TARGET}
                PUBLIC ${CMAKE_BINARY_DIR}/include
                PUBLIC ${BINMORPH_TEST_UTILS_DIR}
                PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
                PUBLIC ${Boost_INCLUDE_DIR}
            )
        endif()
        
    endforeach()
    

endforeach()
