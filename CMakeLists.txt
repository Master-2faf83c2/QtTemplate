cmake_minimum_required(VERSION 3.19)

set(DEFAULT_TARGET "app")

if(DEFINED TARGET_NAME)
    set(CMAKE_PROJECT_NAME ${TARGET_NAME})
else()
    set(CMAKE_PROJECT_NAME ${DEFAULT_TARGET})
endif()

project(${CMAKE_PROJECT_NAME} LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

set_property(GLOBAL APPEND PROPERTY ALL_SOURCES "")
set_property(GLOBAL APPEND PROPERTY ALL_INCLUDE "")
set_property(GLOBAL APPEND PROPERTY ALL_DEFINE "")
set_property(GLOBAL APPEND PROPERTY ALL_LANGUAGE "")

add_subdirectory(src)
add_subdirectory(translations)

get_property(ALL_SOURCES GLOBAL PROPERTY ALL_SOURCES)
get_property(ALL_INCLUDE GLOBAL PROPERTY ALL_INCLUDE)
get_property(ALL_DEFINE  GLOBAL PROPERTY ALL_DEFINE)
get_property(ALL_LANGUAGE  GLOBAL PROPERTY ALL_LANGUAGE)

message("宏定义: ${ALL_DEFINE}")
message("头文件: ${ALL_INCLUDE}")
message("源文件: ${ALL_SOURCES}")
message("语言文件: ${ALL_LANGUAGE}")

qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    ${ALL_SOURCES}
)

qt_add_translations(
    TARGETS ${PROJECT_NAME}
    TS_FILES ${ALL_LANGUAGE}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${ALL_INCLUDE}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
