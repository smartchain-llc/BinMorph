file(GLOB_RECURSE CORE_SRCS "./*.cxx")
set(srclist)

# NOTE: No `${}` around CORE_SRCS and use `IN LISTS`
# Adding `${}` breaks the loop
# `IN LISTS` ensures the file glob expands the glob into a list
foreach(CXX_SRC IN LISTS CORE_SRCS) 
    get_filename_component(CXX_SRC_NAME ${CXX_SRC} NAME_WE)
    list(APPEND srclist "${CXX_SRC_NAME}")
    # set(CXX_SRC_NAMES ${CXX_SRC_NAME})
endforeach()
list(LENGTH srclist LIST_SIZE)
message(STATUS "Glob Size:  ${CORE_SRCS}")
message(STATUS "List size:  ${srclist}")


add_library(
    binmorph_objs
    OBJECT ${CORE_SRCS}    
)
target_compile_options(binmorph_objs PUBLIC -fPIC)
target_include_directories(
    binmorph_objs
    PUBLIC ${JSON_INCLUDE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
    binmorph 
    SHARED $<TARGET_OBJECTS:binmorph_objs>
)
target_include_directories(
    binmorph
    PUBLIC ${JSON_INCLUDE_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)