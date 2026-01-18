set(PACKAGE
    Core 
    Widgets 
    Charts 
    SerialPort 
    PrintSupport
    LinguistTools
    WebEngineWidgets
)

set(LINIK_LIB
    Qt::Core
    Qt::Widgets
    Qt::Charts
    Qt::SerialPort
    Qt::PrintSupport
    Qt::WebEngineWidgets
)

if (WIN32)
    list(APPEND LINIK_LIB opengl32)
endif()

set_property(GLOBAL APPEND PROPERTY ALL_PACKAGE 
    ${PACKAGE}
)

set_property(GLOBAL APPEND PROPERTY ALL_LINK_LIB 
    ${LINIK_LIB}
)
