cmake_minimum_required(VERSION 3.19)

set(DEF_NAME "app")
set(DEF_APP_VERSION "v0.0.1")
set(DEF_IS_INATLL FALSE)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/build/install")

if(NOT DEFINED TARGET_NAME)
    set(TARGET_NAME ${DEF_NAME})
endif()

if(NOT DEFINED APP_VERSION)
    set(APP_VERSION ${DEF_APP_VERSION})
endif()

if(NOT DEFINED IS_INATLL)
    set(IS_INATLL ${DEF_IS_INATLL})
endif()

project(${TARGET_NAME}
    LANGUAGES  CXX
)

if(MSVC)
    add_compile_options(/EHsc)
endif()

set_property(GLOBAL APPEND PROPERTY ALL_SOURCES "")
set_property(GLOBAL APPEND PROPERTY ALL_INCLUDE "")
set_property(GLOBAL APPEND PROPERTY ALL_DEFINE "")
set_property(GLOBAL APPEND PROPERTY ALL_LANGUAGE "")
set_property(GLOBAL APPEND PROPERTY ALL_PACKAGE "")
set_property(GLOBAL APPEND PROPERTY ALL_LINK_LIB "")

include(${CMAKE_CURRENT_LIST_DIR}/scripts/cmake/Package.cmake)
add_subdirectory(src)
add_subdirectory(resources)
add_subdirectory(translations)

get_property(ALL_SOURCES GLOBAL PROPERTY ALL_SOURCES)
get_property(ALL_INCLUDE GLOBAL PROPERTY ALL_INCLUDE)
get_property(ALL_DEFINE  GLOBAL PROPERTY ALL_DEFINE)
get_property(ALL_LANGUAGE  GLOBAL PROPERTY ALL_LANGUAGE)
get_property(ALL_PACKAGE  GLOBAL PROPERTY ALL_PACKAGE)
get_property(ALL_LINK_LIB  GLOBAL PROPERTY ALL_LINK_LIB)

message("项目名称: ${CMAKE_PROJECT_NAME}")
message("版本号: ${APP_VERSION}")
message("是否安装: ${IS_INATLL}")
message("宏定义: ${ALL_DEFINE}")
message("头文件: ${ALL_INCLUDE}")
message("源文件: ${ALL_SOURCES}")
message("语言文件: ${ALL_LANGUAGE}")
message("查找的包: ${ALL_PACKAGE}")
message("链接的库: ${ALL_LINK_LIB}")

find_package(
    Qt6 6.8.3 REQUIRED COMPONENTS 
    ${ALL_PACKAGE}
)

qt_standard_project_setup()

if(WIN32 AND IS_INATLL)
    qt_add_executable(${PROJECT_NAME}
        WIN32
        ${ALL_SOURCES}
    )
elseif(APPLE AND IS_INATLL)
    qt_add_executable(${PROJECT_NAME}
        MACOSX_BUNDLE
        ${ALL_SOURCES}
    )
else()
    qt_add_executable(${PROJECT_NAME}
        ${ALL_SOURCES}
    )
endif()

qt_add_translations(
    TARGETS ${PROJECT_NAME}
    TS_FILES ${ALL_LANGUAGE}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${ALL_DEFINE})

target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDE})

target_link_libraries(${PROJECT_NAME} PRIVATE ${ALL_LINK_LIB})

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

install(DIRECTORY config/
    DESTINATION ${CMAKE_INSTALL_BINDIR}/config
)
