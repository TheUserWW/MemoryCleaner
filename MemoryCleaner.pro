QT       += core gui widgets

CONFIG += c++17
CONFIG += static
CONFIG += strip
TARGET = MemoryCleaner
TEMPLATE = app

QMAKE_LFLAGS_RELEASE += -Wl,-s
QMAKE_CFLAGS_RELEASE -= -O0
QMAKE_CFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE -= -O0
QMAKE_CXXFLAGS_RELEASE += -O3

QMAKE_CXXFLAGS_RELEASE += -ffunction-sections -fdata-sections
QMAKE_LFLAGS_RELEASE += -Wl,--gc-sections

QMAKE_CXXFLAGS_RELEASE += -flto -fno-fat-lto-objects
QMAKE_LFLAGS_RELEASE += -flto

QMAKE_CXXFLAGS_RELEASE += -march=native

QMAKE_CXXFLAGS_RELEASE += -fomit-frame-pointer

QMAKE_CXXFLAGS_RELEASE += -DNDEBUG

win32 {
    QMAKE_LFLAGS_RELEASE += -Wl,--enable-stdcall-fixup
    DEFINES += WIN32_LEAN_AND_MEAN
}

SOURCES += \
    src/aboutdialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/memorycleaner.cpp

HEADERS += \
    include/aboutdialog.h \
    include/mainwindow.h \
    include/memorycleaner.h

INCLUDEPATH += include

FORMS += \
    ui/aboutdialog.ui \
    ui/mainwindow.ui

win32 {
    LIBS += -lpsapi -ladvapi32 -lkernel32
    DEFINES += _WIN32_WINNT=0x0601
    QMAKE_CXXFLAGS += -std=c++11
}

TRANSLATIONS += translations/MemoryCleaner_zh_CN.ts \
                translations/MemoryCleaner_ar_AE.ts \
                translations/MemoryCleaner_de_DE.ts \
                translations/MemoryCleaner_en_US.ts \
                translations/MemoryCleaner_es_ES.ts \
                translations/MemoryCleaner_fr_FR.ts \
                translations/MemoryCleaner_it_IT.ts \
                translations/MemoryCleaner_ja_JP.ts \
                translations/MemoryCleaner_ko_KR.ts \
                translations/MemoryCleaner_mn_CN.ts \
                translations/MemoryCleaner_ru_RU.ts \
                translations/MemoryCleaner_zh_TW.ts \
                translations/MemoryCleaner_ug_UG.ts


CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc

RC_FILE = resources/logo.rc
RC_ICON = resources/app.ico

DISTFILES += \
    resources/logo.rc
