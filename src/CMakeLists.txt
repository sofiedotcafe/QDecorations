
set(QCatppuccinDecorations_SRCS
    QCatppuccinDecorationsPlugin.cpp
    QCatppuccinDecorations.cpp
)

add_library(QCatppuccinDecorations MODULE ${QCatppuccinDecorations_SRCS})
target_link_libraries(QCatppuccinDecorations
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::GuiPrivate
    Qt${QT_VERSION_MAJOR}::Svg
    Qt${QT_VERSION_MAJOR}::WaylandClientPrivate
    Qt${QT_VERSION_MAJOR}::Widgets
)

if (NOT USE_QT6)
    if (${Qt5XkbCommonSupport_FOUND})
    target_link_libraries(QCatppuccinDecorations
        Qt${QT_VERSION_MAJOR}::XkbCommonSupportPrivate
        Qt${QT_VERSION_MAJOR}::ThemeSupportPrivate
    )
    endif()
endif()

install(TARGETS QCatppuccinDecorations DESTINATION ${QT_PLUGINS_DIR}/wayland-decoration-client)

