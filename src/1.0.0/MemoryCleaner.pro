QT       += core gui
# 添加系统托盘支持
QT += network  # QSystemTrayIcon需要
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = MemoryCleaner
TEMPLATE = app


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    aboutdialog.h \
    mainwindow.h

FORMS += \
    aboutdialog.ui \
    mainwindow.ui


win32 {

    LIBS += -lpsapi -lntdll -ladvapi32
    DEFINES += _WIN32_WINNT=0x0601  # 确保Windows API版本
    QMAKE_CXXFLAGS += -std=c++11
}

TRANSLATIONS += \
    MemoryCleaner_zh_CN.ts


CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_FILE = logo.rc
RC_ICON = app.ico

DISTFILES += \
    logo.rc

