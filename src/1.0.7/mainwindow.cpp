#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "memorycleaner.h"
#include <QMessageBox>
#include <QDateTime>
#include <QCloseEvent>
#include <QDebug>
#include <Psapi.h>
#include <TlHelp32.h>
#include <QDesktopServices>
#include <QUrl>
#include "aboutdialog.h"
#include <winternl.h>
#include <QFile>
#include <QFileInfo>
#include <QTranslator>
#include <QApplication>
#include <QSystemTrayIcon>


// 在文件顶部添加（放在#include之后，MainWindow构造函数之前）
const QString MainWindow::tooltipTemplates[] = {
    // 英文
    "Memory Status:\nPhysical Memory: %1/%2 GB (%3%)\nVirtual Memory: %4/%5 GB\nSystem Working Set: %6/%7 GB",
    // 中文
    "内存状态:\n物理内存: %1/%2 GB (%3%)\n虚拟内存: %4/%5 GB\n系统工作集: %6/%7 GB",
    // 法语
    "État de la mémoire:\nMémoire physique: %1/%2 Go (%3%)\nMémoire virtuelle: %4/%5 Go\nEnsemble de travail système: %6/%7 Go",
    // 俄语
    "Состояние памяти:\nФизическая память: %1/%2 ГБ (%3%)\nВиртуальная память: %4/%5 ГБ\nСистемный рабочий набор: %6/%7 ГБ",
    // 西班牙语
    "Estado de la memoria:\nMemoria física: %1/%2 GB (%3%)\nMemoria virtual: %4/%5 GB\nConjunto de trabajo del sistema: %6/%7 GB",
    // 德语
    "Speicherstatus:\nPhysischer Speicher: %1/%2 GB (%3%)\nVirtueller Speicher: %4/%5 GB\nSystem-Arbeitssatz: %6/%7 GB",
    // 意大利语
    "Stato della memoria:\nMemoria fisica: %1/%2 GB (%3%)\nMemoria virtuale: %4/%5 GB\nSet di lavoro del sistema: %6/%7 GB",
    // 阿拉伯语
    "حالة الذاكرة:\nالذاكرة الفعلية: %1/%2 جيجابايت (%3%)\nالذاكرة الظاهرية: %4/%5 جيجابايت\nمجموعة عمل النظام: %6/%7 جيجابايت",
    // 韩语
    "메모리 상태:\n물리적 메모리: %1/%2 GB (%3%)\n가상 메모리: %4/%5 GB\n시스템 작업 세트: %6/%7 GB",
    // 日语
    "メモリ状態:\n物理メモリ: %1/%2 GB (%3%)\n仮想メモリ: %4/%5 GB\nシステムワーキングセット: %6/%7 GB",

    // 中文    
    "記憶體狀態:\n實體記憶體: %1/%2 GB (%3%)\n虛擬記憶體: %4/%5 GB\n系統工作集: %6/%7 GB",

    //维吾尔语
    "يادداۋىتى ھالىتى:\nجىسمانىي يادداۋەت: %1/%2 GB (%3%)\nۋىرتىئۇل يادداۋەت: %4/%5 GB\nسىستېما خىزمەت جىمى: %6/%7 GB",

    //蒙文 (改为西里尔蒙古文)
    "Санах ойн байдал:\nФизик санах ой: %1/%2 GB (%3%)\nВиртуал санах ой: %4/%5 GB\nСистемийн ажиллах багц: %6/%7 GB"
    
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , physicalMemLabel(nullptr)
    , virtualMemLabel(nullptr)
    , systemWSLabel(nullptr)
{
    ui->setupUi(this);

    // 修改默认文本为中文
    setWindowTitle("MemoryCleaner");
    setWindowIcon(QIcon(":/app.ico"));



    HANDLE hToken;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        TOKEN_PRIVILEGES tp;
        LUID luid;

        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
        }
        CloseHandle(hToken);
    }



    autoCleanTimer = new QTimer(this);
    connect(autoCleanTimer, &QTimer::timeout, this, &MainWindow::on_btnCleanNow_clicked);

    connect(ui->AuthorWebsite, &QAction::triggered, this, &MainWindow::openAuthorWebsite);


    monitorTimer = new QTimer(this);
    connect(monitorTimer, &QTimer::timeout, this, &MainWindow::updateMemoryInfo);
    monitorTimer->start(1000);

    createTrayIcon();
    loadSettings();
    displayMemoryInfo();

    this->setMinimumWidth(350);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);

    // 修改默认文本为中文
    // 修改语言菜单标题为固定"Language"
    QMenu *languageMenu = menuBar()->addMenu("Language");  // 原为 tr("语言")
    QAction *chineseAction = languageMenu->addAction("简体中文");
    QAction *traditionalChineseAction = languageMenu->addAction("繁體中文");
    QAction *mongolianAction = languageMenu->addAction("Монгол");  // 改为西里尔蒙古文
    QAction *uyghurAction = languageMenu->addAction("ئۇيغۇرچە");  // 添加维吾尔语菜单项
    QAction *englishAction = languageMenu->addAction("English");
    QAction *frenchAction = languageMenu->addAction("Français");
    QAction *russianAction = languageMenu->addAction("Русский");
    QAction *spanishAction = languageMenu->addAction("Español");
    QAction *germanAction = languageMenu->addAction("Deutsch");  // 添加德语菜单项
    QAction *italianAction = languageMenu->addAction("Italiano");  // 添加意大利语菜单项
    QAction *arabicAction = languageMenu->addAction("العربية");  // 添加阿拉伯语菜单项
    QAction *koreanAction = languageMenu->addAction("한국어");  // 添加韩语菜单项
    QAction *japaneseAction = languageMenu->addAction("日本語");  // 添加日语菜单项


    connect(chineseAction, &QAction::triggered, this, [=]() {
        changeLanguage("zh_CN");
        retranslateUi();
    });
    connect(englishAction, &QAction::triggered, this, [=]() {
        changeLanguage("en_US");
        retranslateUi();
    });
    connect(frenchAction, &QAction::triggered, this, [=]() {
        changeLanguage("fr_FR");
        retranslateUi();
    });
    connect(russianAction, &QAction::triggered, this, [=]() {
        changeLanguage("ru_RU");
        retranslateUi();
    });
    connect(spanishAction, &QAction::triggered, this, [=]() {
        changeLanguage("es_ES");
        retranslateUi();
    });
    connect(germanAction, &QAction::triggered, this, [=]() {  // 添加德语连接
        changeLanguage("de_DE");
        retranslateUi();
    });
    connect(italianAction, &QAction::triggered, this, [=]() {  // 添加意大利语连接
        changeLanguage("it_IT");
        retranslateUi();
    });
    connect(arabicAction, &QAction::triggered, this, [=]() {  // 添加阿拉伯语连接
        changeLanguage("ar_AE");
        retranslateUi();
    });
    connect(koreanAction, &QAction::triggered, this, [=]() {  // 添加韩语连接
        changeLanguage("ko_KR");
        retranslateUi();
    });

    connect(japaneseAction, &QAction::triggered, this, [=]() {  // 添加日语连接
        changeLanguage("ja_JP");
        retranslateUi();
    });

    connect(uyghurAction, &QAction::triggered, this, [=]() {  // 添加维吾尔语连接
        changeLanguage("ug_UG");
        retranslateUi();
    });

    connect(traditionalChineseAction, &QAction::triggered, this, [=]() {
        changeLanguage("zh_TW");
        retranslateUi();
    });

    connect(mongolianAction, &QAction::triggered, this, [=]() {  // 添加传统蒙古文连接
        changeLanguage("mn_CN");
        retranslateUi();
    });
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::showAboutDialog()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            hide();
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::createTrayIcon() {
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(createTextIcon(0));

    // 创建托盘菜单（全部使用中文默认文本）
    trayMenu = new QMenu(this);
    cleanSubMenu = new QMenu(tr("清理内存区域"), this);

    trayWorkingSetAction = cleanSubMenu->addAction(tr("工作集"));
    trayWorkingSetAction->setCheckable(true);

    traySystemWSAction = cleanSubMenu->addAction(tr("系统工作集"));
    traySystemWSAction->setCheckable(true);

    trayStandbyListAction = cleanSubMenu->addAction(tr("备用列表"));
    trayStandbyListAction->setCheckable(true);

    trayStandbyListLowAction = cleanSubMenu->addAction(tr("低优先级备用列表"));
    trayStandbyListLowAction->setCheckable(true);

    trayModifiedListAction = cleanSubMenu->addAction(tr("修改页面列表"));
    trayModifiedListAction->setCheckable(true);

    trayCombinedListAction = cleanSubMenu->addAction(tr("组合页面列表"));
    trayCombinedListAction->setCheckable(true);

    connect(trayWorkingSetAction, &QAction::triggered, this, &MainWindow::onTrayCleanWorkingSet);
    connect(traySystemWSAction, &QAction::triggered, this, &MainWindow::onTrayCleanSystemWS);
    connect(trayStandbyListAction, &QAction::triggered, this, &MainWindow::onTrayCleanStandbyList);
    connect(trayStandbyListLowAction, &QAction::triggered, this, &MainWindow::onTrayCleanStandbyListLow);
    connect(trayModifiedListAction, &QAction::triggered, this, &MainWindow::onTrayCleanModifiedList);
    connect(trayCombinedListAction, &QAction::triggered, this, &MainWindow::onTrayCleanCombinedList);
    connect(trayWorkingSetAction, &QAction::triggered, this, &MainWindow::saveCurrentOptions);
    connect(traySystemWSAction, &QAction::triggered, this, &MainWindow::saveCurrentOptions);
    connect(trayStandbyListAction, &QAction::triggered, this, &MainWindow::saveCurrentOptions);
    connect(trayStandbyListLowAction, &QAction::triggered, this, &MainWindow::saveCurrentOptions);
    connect(trayModifiedListAction, &QAction::triggered, this, &MainWindow::saveCurrentOptions);
    connect(trayCombinedListAction, &QAction::triggered, this, &MainWindow::saveCurrentOptions);

    trayMenu->addMenu(cleanSubMenu);
    trayMenu->addSeparator();

    QAction* cleanNowAction = new QAction(tr("立即清理"), this);
    connect(cleanNowAction, &QAction::triggered, this, &MainWindow::on_btnCleanNow_clicked);
    trayMenu->addAction(cleanNowAction);

    trayMenu->addSeparator();

    aboutAction = new QAction(tr("关于"), this);
    aboutAction->setIcon(QIcon(":/icons/about.png"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutFromTray);
    trayMenu->addAction(aboutAction);



    trayMenu->addSeparator();

    QAction* showAction = new QAction(tr("显示窗口"), this);
    QAction* exitAction = new QAction(tr("退出"), this);
    connect(showAction, &QAction::triggered, this, &MainWindow::showWindow);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApp);

    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);
    syncTrayMenuWithSettings();
    trayIcon->show();
}

void MainWindow::showWindow()
{
    show();
    activateWindow();
    raise();
}

void MainWindow::hideWindow()
{
    hide();
}

void MainWindow::exitApp()
{
    trayIcon->hide();
    QApplication::quit();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        if (isVisible()) {
            hide();
        } else {
            showWindow();
        }
    }
}

void MainWindow::loadSettings()
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(iniPath, QSettings::IniFormat);

    ui->cbWorkingSet->setChecked(settings.value("cleanOptions/workingSet", true).toBool());
    ui->cbSystemWorkingSet->setChecked(settings.value("cleanOptions/systemWS", false).toBool());
    ui->cbStandbyList->setChecked(settings.value("cleanOptions/standby", true).toBool());
    ui->cbStandbyListLow->setChecked(settings.value("cleanOptions/standbyLow", false).toBool());
    ui->cbModifiedPageList->setChecked(settings.value("cleanOptions/modified", false).toBool());
    ui->cbCombinePageList->setChecked(settings.value("cleanOptions/combined", false).toBool());
    QString lang = settings.value("language/current", "zh_CN").toString();
    changeLanguage(lang);
    ui->cbAutoClean->setChecked(settings.value("autoClean/enabled", false).toBool());
    ui->sbInterval->setValue(settings.value("autoClean/interval", 60).toInt());
}

void MainWindow::saveSettings()
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/settings.ini";
    QFile file(iniPath);

    if (!QFileInfo(QCoreApplication::applicationDirPath()).isWritable()) {
        // 修改警告消息为中文
        QMessageBox::warning(this, "MemoryCleaner", tr("程序目录不可写，无法保存设置！"));  // 原为 tr("警告")
        return;
    }

    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setValue("language/current", currentLanguage);
    settings.setValue("cleanOptions/workingSet", ui->cbWorkingSet->isChecked());
    settings.setValue("cleanOptions/systemWS", ui->cbSystemWorkingSet->isChecked());
    settings.setValue("cleanOptions/standby", ui->cbStandbyList->isChecked());
    settings.setValue("cleanOptions/standbyLow", ui->cbStandbyListLow->isChecked());
    settings.setValue("cleanOptions/modified", ui->cbModifiedPageList->isChecked());
    settings.setValue("cleanOptions/combined", ui->cbCombinePageList->isChecked());
    settings.setValue("autoClean/enabled", ui->cbAutoClean->isChecked());
    settings.setValue("autoClean/interval", ui->sbInterval->value());
}

void MainWindow::cleanSelectedMemory()
{
    bool cleanWorkingSet = ui->cbWorkingSet->isChecked();

    if (cleanWorkingSet) {
        if (!MemoryCleaner::cleanAllProcessesWorkingSet()) {
            // 修改状态消息为中文
            ui->statusbar->showMessage(tr("清理系统工作集失败，可能需要管理员权限"), 3000);
        }
    }

    if (ui->cbSystemWorkingSet->isChecked()) {
        MemoryCleaner::cleanSystemWorkingSet();
    }
    if (ui->cbStandbyList->isChecked()) {
        MemoryCleaner::cleanStandbyList(false);
    }
    if (ui->cbStandbyListLow->isChecked()) {
        MemoryCleaner::cleanStandbyList(true);
    }
    if (ui->cbModifiedPageList->isChecked()) {
        MemoryCleaner::cleanModifiedPageList();
    }
    if (ui->cbCombinePageList->isChecked()) {
        MemoryCleaner::cleanCombinePageList();
    }
}

void MainWindow::displayMemoryInfo()
{
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memStatus);

    QColor bgColor = palette().window().color();
    QString textColor = getContrastColor(bgColor).name();

    double usedPhysGB = (memStatus.ullTotalPhys - memStatus.ullAvailPhys) / (1024.0 * 1024 * 1024);
    double totalPhysGB = memStatus.ullTotalPhys / (1024.0 * 1024 * 1024);
    ui->lbPhysicalValue->setText(
        QString("<span style='color:%1;'>%2%</span> (%3 GB / %4 GB)")
            .arg(textColor)
            .arg(memStatus.dwMemoryLoad)
            .arg(usedPhysGB, 0, 'f', 2)
            .arg(totalPhysGB, 0, 'f', 2));

    double usedVirtGB = (memStatus.ullTotalPageFile - memStatus.ullAvailPageFile) / (1024.0 * 1024 * 1024);
    double totalVirtGB = memStatus.ullTotalPageFile / (1024.0 * 1024 * 1024);
    int virtUsagePercent = (totalVirtGB > 0) ? static_cast<int>((usedVirtGB / totalVirtGB) * 100) : 0;
    ui->lbVirtualValue->setText(
        QString("<span style='color:%1;'>%2%</span> (%3 GB / %4 GB)")
            .arg(textColor)
            .arg(virtUsagePercent)
            .arg(usedVirtGB, 0, 'f', 2)
            .arg(totalVirtGB, 0, 'f', 2));

    quint64 totalWS = 0, sharedWS = 0, privateWS = 0;
    if (MemoryCleaner::getSystemWorkingSetInfo(totalWS, sharedWS, privateWS)) {
        double totalWSGB = totalWS / (1024.0 * 1024 * 1024);
        double privateWSGB = privateWS / (1024.0 * 1024 * 1024);
        int wsUsagePercent = totalWS > 0 ? static_cast<int>((privateWS * 100) / totalWS) : 0;

        ui->lbSystemWSValue->setText(
            QString("<span style='color:%1;'>%2%</span> (%3 GB / %4 GB)")
                .arg(textColor)
                .arg(wsUsagePercent)
                .arg(privateWSGB, 0, 'f', 2)
                .arg(totalWSGB, 0, 'f', 2));
    }

    ui->pbPhysical->setValue(memStatus.dwMemoryLoad);
    ui->pbPhysical->setFormat("");

    ui->pbVirtual->setValue(virtUsagePercent);
    ui->pbVirtual->setFormat("");

    ui->pbSystemWS->setValue((totalWS > 0) ? static_cast<int>((privateWS * 100) / totalWS) : 0);
    ui->pbSystemWS->setFormat("");
}

bool MainWindow::getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS)
{
    totalWS = 0;
    sharedWS = 0;
    privateWS = 0;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (pe32.th32ProcessID == 0) continue;

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess) {
            PROCESS_MEMORY_COUNTERS_EX pmc;
            if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
                totalWS += pmc.WorkingSetSize;
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    sharedWS = totalWS * 0.3;
    privateWS = totalWS - sharedWS;

    return true;
}

void MainWindow::setProgressBarStyle(QProgressBar* bar, int value, QLabel*& label)
{
    bar->setStyleSheet("");
    bar->setFormat("");

    if (!label) {
        label = new QLabel(this);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setStyleSheet("QLabel { padding-right: 5px; color: black; }");
    }

    label->setGeometry(
        bar->x() + bar->width() - 50,
        bar->y(),
        50,
        bar->height()
        );
    label->setText(QString("%1%").arg(value));
    label->raise();
    label->show();
}

QColor MainWindow::getContrastColor(const QColor& background) const
{
    const double luminance = (0.299 * background.redF() +
                              0.587 * background.greenF() +
                              0.114 * background.blueF());
    return luminance > 0.5 ? Qt::black : Qt::white;
}

void MainWindow::on_btnCleanNow_clicked()
{
    MEMORYSTATUSEX memBefore;
    memBefore.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memBefore);

    // 修改状态消息为中文
    ui->statusbar->showMessage(tr("正在清理内存..."));
    cleanSelectedMemory();
    displayMemoryInfo();

    MEMORYSTATUSEX memAfter;
    memAfter.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memAfter);

    double cleaned = (memAfter.ullAvailPhys - memBefore.ullAvailPhys) / (1024.0 * 1024);
    ui->textBrowser->append(tr("[%1] 已清理内存: %2 MB")
                                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                                .arg(cleaned, 0, 'f', 1));

    // 在显示"内存清理完成"消息的地方，应该修改为：
    QString message = tr("内存清理完成");
    trayIcon->showMessage("MemoryCleaner", message, QSystemTrayIcon::Information, 3000);
}

void MainWindow::on_cbAutoClean_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        autoCleanTimer->start(ui->sbInterval->value() * 1000);
    } else {
        autoCleanTimer->stop();
    }
}

void MainWindow::on_sbInterval_valueChanged(int arg1)
{
    if (ui->cbAutoClean->isChecked()) {
        autoCleanTimer->setInterval(arg1 * 1000);
    }
}

void MainWindow::updateMemoryInfo()
{
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memStatus);

    quint64 totalWS = 0, sharedWS = 0, privateWS = 0;
    getSystemWorkingSetInfo(totalWS, sharedWS, privateWS);

    trayIcon->setIcon(createTextIcon(memStatus.dwMemoryLoad));

    double usedPhysGB = (memStatus.ullTotalPhys - memStatus.ullAvailPhys) / (1024.0 * 1024 * 1024);
    double totalPhysGB = memStatus.ullTotalPhys / (1024.0 * 1024 * 1024);
    double usedVirtGB = (memStatus.ullTotalPageFile - memStatus.ullAvailPageFile) / (1024.0 * 1024 * 1024);
    double totalVirtGB = memStatus.ullTotalPageFile / (1024.0 * 1024 * 1024);
    double privateWSGB = privateWS / (1024.0 * 1024 * 1024);
    double totalWSGB = totalWS / (1024.0 * 1024 * 1024);

    int langIndex = 0; // 默认英文
    if(currentLanguage == "zh_CN") langIndex = 1;
    else if(currentLanguage == "fr_FR") langIndex = 2;
    else if(currentLanguage == "ru_RU") langIndex = 3;
    else if(currentLanguage == "es_ES") langIndex = 4;
    else if(currentLanguage == "de_DE") langIndex = 5;
    else if(currentLanguage == "it_IT") langIndex = 6;
    else if(currentLanguage == "ar_AE") langIndex = 7;
    else if(currentLanguage == "ko_KR") langIndex = 8;
    else if(currentLanguage == "ja_JP") langIndex = 9;
    else if(currentLanguage == "zh_TW") langIndex = 10; // 繁体中文索引
    else if(currentLanguage == "ug_UG") langIndex = 11; // 维吾尔语索引
    else if(currentLanguage == "mn_CN") langIndex = 12; // 传统蒙古文索引
    m_trayToolTip = QString(tooltipTemplates[langIndex])
        .arg(usedPhysGB, 0, 'f', 1)
        .arg(totalPhysGB, 0, 'f', 1)
        .arg(memStatus.dwMemoryLoad)
        .arg(usedVirtGB, 0, 'f', 1)
        .arg(totalVirtGB, 0, 'f', 1)
        .arg(privateWSGB, 0, 'f', 1)
        .arg(totalWSGB, 0, 'f', 1);

    if (trayIcon) {
        trayIcon->setToolTip(m_trayToolTip);
        ui->centralwidget->update();
    }

    displayMemoryInfo();
}


QIcon MainWindow::createTextIcon(int usagePercent) {
    QPixmap pixmap(48, 48);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QColor bgColor;
    if (usagePercent > 90)       bgColor = QColor(0xD3, 0x2F, 0x2F);
    else if (usagePercent > 70)  bgColor = QColor(0xF5, 0x7C, 0x00);
    else                        bgColor = QColor(0x0, 0x80, 0x40);
    painter.fillRect(pixmap.rect(), bgColor);

    QString text = QString::number(usagePercent) ;
    QFont font;
    font.setFamily("Arial");
    font.setWeight(QFont::Normal);

    int fontSize = 40;
    font.setPixelSize(fontSize);
    QFontMetrics fm(font);

    while ((fm.horizontalAdvance(text) > pixmap.width() - 2 ||
            fm.height() > pixmap.height()) &&
           fontSize > 10) {
        fontSize--;
        font.setPixelSize(fontSize);
        fm = QFontMetrics(font);
    }

    QRect textRect = pixmap.rect();
    textRect.adjust(1, -2, -1, 0);

    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, text);

    return QIcon(pixmap);
}

void MainWindow::onTrayCleanWorkingSet()
{
    if (MemoryCleaner::cleanWorkingSet()) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("MemoryCleaner", tr("工作集清理成功"), icon, 2000);  // 原为 tr("内存清理工具")
    } else {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Critical);
        trayIcon->showMessage("MemoryCleaner", tr("清理工作集失败"), icon, 2000);  // 原为 tr("内存清理工具")
    }
}

void MainWindow::onTrayCleanSystemWS()
{
    if (MemoryCleaner::cleanSystemWorkingSet()) {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("MemoryCleaner", tr("系统工作集清理成功"), icon, 2000);  // 原为 tr("内存清理工具")
    } else {
        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Critical);
        trayIcon->showMessage("MemoryCleaner", tr("清理系统工作集失败"), icon, 2000);  // 原为 tr("内存清理工具")
    }
}

void MainWindow::onTrayCleanStandbyList() {
    saveCurrentOptions();
    if (trayStandbyListAction->isChecked()) {
        MemoryCleaner::cleanStandbyList(false);
        trayIcon->showMessage(tr("内存清理"), tr("备用列表清理完成"), QSystemTrayIcon::Information, 2000);  // 原为 tr("内存清理")
    }
    updateMemoryInfo();
}

void MainWindow::onTrayCleanStandbyListLow() {
    saveCurrentOptions();
    if (trayStandbyListLowAction->isChecked()) {
        MemoryCleaner::cleanStandbyList(true);
        trayIcon->showMessage(tr("内存清理"), tr("低优先级备用列表清理完成"), QSystemTrayIcon::Information, 2000);  // 原为 tr("内存清理")
    }
    updateMemoryInfo();
}

void MainWindow::onTrayCleanModifiedList() {
    saveCurrentOptions();
    if (trayModifiedListAction->isChecked()) {
        MemoryCleaner::cleanModifiedPageList();
        trayIcon->showMessage(tr("内存清理"), tr("修改页面列表清理完成"), QSystemTrayIcon::Information, 2000);  // 原为 tr("内存清理")
    }
    updateMemoryInfo();
}

void MainWindow::onTrayCleanCombinedList() {
    saveCurrentOptions();
    if (trayCombinedListAction->isChecked()) {
        MemoryCleaner::cleanCombinePageList();
        trayIcon->showMessage(tr("内存清理"), tr("组合页面列表清理完成"), QSystemTrayIcon::Information, 2000);  // 原为 tr("内存清理")
    }
    updateMemoryInfo();
}

void MainWindow::showAboutFromTray()
{
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
    trayIcon->showMessage(tr("关于"), tr("显示关于对话框"), QSystemTrayIcon::Information, 1000);
}

void MainWindow::openAuthorWebsite() {
    QDesktopServices::openUrl(QUrl("https://github.com/TheUserWW"));
}

void MainWindow::syncTrayMenuWithSettings() {
    QString iniPath = QCoreApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(iniPath, QSettings::IniFormat);

    bool workingSet = settings.value("cleanOptions/workingSet", true).toBool();
    bool systemWS = settings.value("cleanOptions/systemWS", false).toBool();
    bool standby = settings.value("cleanOptions/standby", true).toBool();
    bool standbyLow = settings.value("cleanOptions/standbyLow", false).toBool();
    bool modified = settings.value("cleanOptions/modified", false).toBool();
    bool combined = settings.value("cleanOptions/combined", false).toBool();

    trayWorkingSetAction->setChecked(workingSet);
    traySystemWSAction->setChecked(systemWS);
    trayStandbyListAction->setChecked(standby);
    trayStandbyListLowAction->setChecked(standbyLow);
    trayModifiedListAction->setChecked(modified);
    trayCombinedListAction->setChecked(combined);

    ui->cbWorkingSet->setChecked(workingSet);
    ui->cbSystemWorkingSet->setChecked(systemWS);
    ui->cbStandbyList->setChecked(standby);
    ui->cbStandbyListLow->setChecked(standbyLow);
    ui->cbModifiedPageList->setChecked(modified);
    ui->cbCombinePageList->setChecked(combined);
}

void MainWindow::saveCurrentOptions() {
    QString iniPath = QCoreApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(iniPath, QSettings::IniFormat);

    if (!QFileInfo(QCoreApplication::applicationDirPath()).isWritable()) {
        QMessageBox::warning(this, tr("警告"), tr("程序目录不可写，无法保存设置！"));
        return;
    }

    settings.setValue("cleanOptions/workingSet", trayWorkingSetAction->isChecked());
    settings.setValue("cleanOptions/systemWS", traySystemWSAction->isChecked());
    settings.setValue("cleanOptions/standby", trayStandbyListAction->isChecked());
    settings.setValue("cleanOptions/standbyLow", trayStandbyListLowAction->isChecked());
    settings.setValue("cleanOptions/modified", trayModifiedListAction->isChecked());
    settings.setValue("cleanOptions/combined", trayCombinedListAction->isChecked());
    settings.sync();
}



void MainWindow::changeLanguage(const QString& locale) {
    QApplication::removeTranslator(&translator);
    QString qmFilePath = QString("%1/MemoryCleaner_%2.qm")
                             .arg(QCoreApplication::applicationDirPath())
                             .arg(locale);

    if (translator.load(qmFilePath)) {
        QApplication::installTranslator(&translator);
        updateMemoryInfo();
    } else {
        qDebug() << "Failed to load translation file:" << qmFilePath;
    }
    currentLanguage = locale;

    retranslateUi();

}



void MainWindow::retranslateUi()
{

    ui->retranslateUi(this);

    // 确保窗口标题始终为 MemoryCleaner
    setWindowTitle("MemoryCleaner");

    if (trayMenu) {
        trayMenu->deleteLater();
        cleanSubMenu->deleteLater();
        trayIcon->deleteLater();
    }

    createTrayIcon();
    syncTrayMenuWithSettings();


    ui->cbWorkingSet->setText(tr("工作集"));
    ui->cbSystemWorkingSet->setText(tr("系统工作集"));
    ui->cbStandbyList->setText(tr("备用列表"));
    ui->cbStandbyListLow->setText(tr("低优先级备用列表"));
    ui->cbModifiedPageList->setText(tr("修改页面列表"));
    ui->cbCombinePageList->setText(tr("组合页面列表"));
    

    trayIcon->setToolTip("MemoryCleaner");  // 原为 tr("内存清理工具")
    trayMenu->setTitle(tr("托盘菜单"));
    cleanSubMenu->setTitle(tr("清理选项"));
    trayWorkingSetAction->setText(tr("工作集"));
    traySystemWSAction->setText(tr("系统工作集"));
    trayStandbyListAction->setText(tr("备用列表"));
    trayStandbyListLowAction->setText(tr("低优先级备用列表"));
    trayModifiedListAction->setText(tr("修改页面列表"));
    trayCombinedListAction->setText(tr("组合页面列表"));

    
    
    

    connect(trayWorkingSetAction, &QAction::triggered, this, &MainWindow::onTrayCleanWorkingSet);
    connect(traySystemWSAction, &QAction::triggered, this, &MainWindow::onTrayCleanSystemWS);
    connect(trayStandbyListAction, &QAction::triggered, this, &MainWindow::onTrayCleanStandbyList);
    connect(trayStandbyListLowAction, &QAction::triggered, this, &MainWindow::onTrayCleanStandbyListLow);
    connect(trayModifiedListAction, &QAction::triggered, this, &MainWindow::onTrayCleanModifiedList);
    connect(trayCombinedListAction, &QAction::triggered, this, &MainWindow::onTrayCleanCombinedList);

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memStatus);
    
    quint64 totalWS = 0, sharedWS = 0, privateWS = 0;
    getSystemWorkingSetInfo(totalWS, sharedWS, privateWS);
    
    trayIcon->setToolTip(m_trayToolTip);


    ui->centralwidget->update();
                      
    
    // 强制更新托盘提示
    updateMemoryInfo();
    if (trayIcon) {
    trayIcon->setToolTip(m_trayToolTip);
}


}


