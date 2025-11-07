configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/version.h.in
    ${CMAKE_CURRENT_LIST_DIR}/version.h
    @ONLY
)

file(GLOB_RECURSE FIND_FILE
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

set_property(GLOBAL APPEND PROPERTY ALL_INCLUDE 
    ${CMAKE_CURRENT_LIST_DIR}
)

set_property(
    GLOBAL APPEND PROPERTY ALL_SOURCES 
    ${FIND_FILE}
)
