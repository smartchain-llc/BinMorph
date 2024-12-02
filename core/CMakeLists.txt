file(GLOB_RECURSE CORE_SRCS "./*.cxx")
add_library(
    binmorph 
    SHARED ${CORE_SRCS}
)
target_include_directories(
    binmorph
    PUBLIC ${JSON_INCLUDE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)