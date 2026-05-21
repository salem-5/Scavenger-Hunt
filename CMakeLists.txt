cmake_minimum_required(VERSION 3.16)
project(ScavengerHunt VERSION 1.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")
    set(CMAKE_INSTALL_RPATH "@executable_path/../Frameworks")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(PRODUCT_BUNDLE_IDENTIFIER "net.scavengerhunt.game")
set(PRODUCT_NAME "Scavenger Hunt")
set(PRODUCT_COMPANY "Net ScavengerHunt")
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
file(GLOB_RECURSE PROJECT_SOURCES
        CONFIGURE_DEPENDS
        "core/*.cpp" "core/*.h"
        "engine/*.cpp" "engine/*.h"
        "entities/*.cpp" "entities/*.h"
        "systems/*.cpp" "systems/*.h"
        "ui/*.cpp" "ui/*.h"
        "data/*.cpp" "data/*.h"
)
list(APPEND PROJECT_SOURCES main.cpp resources.qrc)

if(APPLE)
    set(MACOS_ICON "assets/icons/icon.icns")
    set_source_files_properties(${MACOS_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    list(APPEND PROJECT_SOURCES ${MACOS_ICON})
set(MACOSX_BUNDLE_ICON_FILE "icon.icns")
set(MACOSX_BUNDLE_GUI_IDENTIFIER ${PRODUCT_BUNDLE_IDENTIFIER})
set(MACOSX_BUNDLE_BUNDLE_NAME ${PRODUCT_NAME})
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
elseif(WIN32)
set(RC_FILE "${CMAKE_CURRENT_BINARY_DIR}/scavenger_hunt.rc")
file(WRITE ${RC_FILE} "IDI_ICON1 ICON DISCARDABLE \"${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/icon.ico\"")
list(APPEND PROJECT_SOURCES ${RC_FILE})
endif()

add_executable(ScavengerHunt MACOSX_BUNDLE WIN32 ${PROJECT_SOURCES}
        ui/screen/AboutDialog.cpp
        ui/screen/AboutDialog.h)

set_target_properties(ScavengerHunt PROPERTIES
OUTPUT_NAME "${PRODUCT_NAME}"
)

target_include_directories(ScavengerHunt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ScavengerHunt PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

include(GNUInstallDirs)
install(TARGETS ScavengerHunt
BUNDLE  DESTINATION .
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(APPLE)
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

if(MACDEPLOYQT_EXECUTABLE)
install(CODE "
            message(STATUS \"Running macdeployqt on ${PRODUCT_NAME}.app...\")
            execute_process(
                COMMAND \"${MACDEPLOYQT_EXECUTABLE}\"
                        \"\${CMAKE_INSTALL_PREFIX}/${PRODUCT_NAME}.app\"
                        -always-overwrite
                        -verbose=1
            )
        ")
endif()
endif()

set(CPACK_PACKAGE_NAME ${PRODUCT_NAME})
set(CPACK_PACKAGE_VENDOR ${PRODUCT_COMPANY})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "omarahmedmmsalem@gmail.com")

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
elseif(WIN32)
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE APP_VERSION="${PROJECT_VERSION}")
include(CPack)