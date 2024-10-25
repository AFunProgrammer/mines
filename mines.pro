QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svg

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    minefield.cpp \
    mines.cpp

HEADERS += \
    minefield.h \
    mines.h \
    utility.h

FORMS += \
    mines.ui

win32:RC_ICONS += mines.ico
# macx:RC_ICONS += mines.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mines.qrc
