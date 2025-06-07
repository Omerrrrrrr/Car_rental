cmake_minimum_required(VERSION 3.5)

project(Car_rental VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Multimedia MultimediaWidgets)


set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    car.cpp
    car.h
    car.ui
    customer.cpp
    customer.h
    customer.ui
    rentacar.cpp
    rentacar.h
    rentacar.ui
)

if(Qt6_FOUND)
    qt_add_executable(Car_rental
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        backgound.qrc
    )
    target_link_libraries(Car_rental PRIVATE Qt6::Widgets Qt6::Sql Qt6::Multimedia Qt6::MultimediaWidgets)
elseif(Qt5_FOUND)
    add_executable(Car_rental
        ${PROJECT_SOURCES}
    )
    target_link_libraries(Car_rental PRIVATE Qt5::Widgets Qt5::Sql Qt5::Multimedia Qt5::MultimediaWidgets)
endif()


set_target_properties(Car_rental PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Car_rental
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Car_rental)
endif()
