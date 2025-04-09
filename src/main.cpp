#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 根据系统语言加载翻译
    QTranslator translator;
    QString systemLang = QLocale::system().name(); // 如 "zh_CN", "en_US"
    QString langPath = ":/"; // .qm文件在资源中的路径

    // 支持的语言列表
    QStringList supportedLangs = {"zh_CN", "en_US", "fr_FR", "de_DE", "es_ES", "ja_JP"};

    if (supportedLangs.contains(systemLang)) {
        translator.load(langPath + "MemoryCleaner_" + systemLang + ".qm");
    } else {
        // 默认英文
        translator.load(langPath + "MemoryCleaner_en_US.qm");
    }

    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
