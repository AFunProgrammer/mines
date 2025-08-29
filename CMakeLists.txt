cmake_minimum_required(VERSION 3.16)
project(mines VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui OpenGLWidgets Svg Widgets)

qt_standard_project_setup()

qt_add_executable(mines WIN32 MACOSX_BUNDLE
    main.cpp
    minefield.cpp minefield.h
    mines.cpp mines.h mines.ui
    utility.h
    mines.qrc
    MANUAL_FINALIZATION
    android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/gradle/wrapper/gradle-wrapper.jar android/gradle/wrapper/gradle-wrapper.properties android/gradle.properties android/gradlew android/gradlew.bat
)

target_link_libraries(mines PRIVATE
    Qt::Core
    Qt::Gui
    Qt::OpenGLWidgets
    Qt::Svg
    Qt6::Widgets
)

# --- Android-specific Configuration (following the tutorial) ---
if(ANDROID)
    # Specify the Android package source directory using a target property.
    # This is the method the tutorial uses for the manifest.
    set_property(TARGET mines APPEND PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")

    # QT_ANDROID_MIN_SDK_VERSION:
    # Set to recommended minimum for Qt 6.5.3. (26 is actually but trying 24)
    set_property(TARGET mines PROPERTY QT_ANDROID_MIN_SDK_VERSION 24)

    # QT_ANDROID_TARGET_SDK_VERSION:
    # Set to current recommended target.
    set_property(TARGET mines PROPERTY QT_ANDROID_TARGET_SDK_VERSION 35)

    # Manually finalize the executable for Android deployment.
    # This pairs with MANUAL_FINALIZATION in qt_add_executable.
    qt_finalize_executable(mines) # <--- Added from tutorial
endif()
# --- End Android-specific Configuration ---

# Install only on non-Android platforms
if(NOT ANDROID)
    install(TARGETS mines
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION .
    )
endif()

# Deployment script
qt_generate_deploy_app_script(
    TARGET mines
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})
