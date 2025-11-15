QT       += core gui
# 添加系统托盘支持
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += static
TARGET = MemoryCleaner
TEMPLATE = app


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    memorycleaner.cpp

HEADERS += \
    aboutdialog.h \
    mainwindow.h \
    memorycleaner.h

FORMS += \
    aboutdialog.ui \
    mainwindow.ui

win32 {
    LIBS += -lpsapi -ladvapi32 -lkernel32
    DEFINES += _WIN32_WINNT=0x0601  # 确保Windows API版本
    QMAKE_CXXFLAGS += -std=c++11
}

TRANSLATIONS += MemoryCleaner_zh_CN.ts \
                MemoryCleaner_ar_AE.ts \
                MemoryCleaner_de_DE.ts \
                MemoryCleaner_en_US.ts \
                MemoryCleaner_es_ES.ts \
                MemoryCleaner_fr_FR.ts \
                MemoryCleaner_it_IT.ts \
                MemoryCleaner_ja_JP.ts \
                MemoryCleaner_ko_KR.ts \
                MemoryCleaner_mn_CN.ts \
                MemoryCleaner_ru_RU.ts \
                MemoryCleaner_zh_TW.ts \
                MemoryCleaner_ug_UG.ts


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
