#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "memorycleaner.h" // 添加头文件
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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , physicalMemLabel(nullptr)
    , virtualMemLabel(nullptr)
    , systemWSLabel(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("内存清理工具");
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
    QAction *actionAuthor = new QAction("作者主页", this);
    ui->menuHelp->addAction(actionAuthor);  // 添加到帮助菜单
    connect(actionAuthor, &QAction::triggered, this, &MainWindow::openAuthorWebsite);



    autoCleanTimer = new QTimer(this);
    connect(autoCleanTimer, &QTimer::timeout, this, &MainWindow::on_btnCleanNow_clicked);

    monitorTimer = new QTimer(this);
    connect(monitorTimer, &QTimer::timeout, this, &MainWindow::updateMemoryInfo);
    monitorTimer->start(1000);

    createTrayIcon();
    loadSettings();
    displayMemoryInfo();

    // Connect about action
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
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

    // 创建主菜单
    trayMenu = new QMenu(this);

    // 创建二级菜单（清理选项）
    cleanSubMenu = new QMenu("清理内存区域", this);

    // 添加清理选项到二级菜单
    trayWorkingSetAction = cleanSubMenu->addAction("工作集(Working Set)");
    trayWorkingSetAction->setCheckable(true);

    traySystemWSAction = cleanSubMenu->addAction("系统工作集(System WS)");
    traySystemWSAction->setCheckable(true);

    trayStandbyListAction = cleanSubMenu->addAction("备用列表(Standby)");
    trayStandbyListAction->setCheckable(true);

    trayStandbyListLowAction = cleanSubMenu->addAction("低优先级备用列表");
    trayStandbyListLowAction->setCheckable(true);

    trayModifiedListAction = cleanSubMenu->addAction("修改页面列表");
    trayModifiedListAction->setCheckable(true);

    trayCombinedListAction = cleanSubMenu->addAction("组合页面列表");
    trayCombinedListAction->setCheckable(true);

    // 连接信号槽
    connect(trayWorkingSetAction, &QAction::triggered, this, &MainWindow::onTrayCleanWorkingSet);
    // ...连接其他动作...

    // 将二级菜单添加到主菜单
    trayMenu->addMenu(cleanSubMenu);
    trayMenu->addSeparator();

    // 添加立即清理动作
    QAction* cleanNowAction = new QAction("立即清理选中项", this);
    connect(cleanNowAction, &QAction::triggered, this, &MainWindow::on_btnCleanNow_clicked);
    trayMenu->addAction(cleanNowAction);

    trayMenu->addSeparator();

    // 在菜单分隔线后添加新选项
    trayMenu->addSeparator();

    // 1. 关于选项
    aboutAction = new QAction("关于", this);
    aboutAction->setIcon(QIcon(":/icons/about.png")); // 可选图标
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutFromTray);
    trayMenu->addAction(aboutAction);

    // 2. 作者选项
    authorAction = new QAction("作者", this);
    authorAction->setIcon(QIcon(":/icons/github.png")); // 可选图标
    connect(authorAction, &QAction::triggered, this, &MainWindow::openAuthorWebsite);
    trayMenu->addAction(authorAction);

    trayMenu->addSeparator(); // 最后再加一个分隔线

    // 添加其他常规菜单项
    QAction* showAction = new QAction("显示主窗口", this);
    QAction* exitAction = new QAction("退出", this);
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
    QSettings settings;
    ui->cbWorkingSet->setChecked(settings.value("cleanOptions/workingSet", true).toBool());
    ui->cbSystemWorkingSet->setChecked(settings.value("cleanOptions/systemWS", false).toBool());
    ui->cbStandbyList->setChecked(settings.value("cleanOptions/standby", true).toBool());
    ui->cbStandbyListLow->setChecked(settings.value("cleanOptions/standbyLow", false).toBool());
    ui->cbModifiedPageList->setChecked(settings.value("cleanOptions/modified", false).toBool());
    ui->cbCombinePageList->setChecked(settings.value("cleanOptions/combined", false).toBool());
    ui->cbAutoClean->setChecked(settings.value("autoClean/enabled", false).toBool());
    ui->sbInterval->setValue(settings.value("autoClean/interval", 60).toInt());

    if (ui->cbAutoClean->isChecked()) {
        autoCleanTimer->start(ui->sbInterval->value() * 1000);
    }
}

void MainWindow::saveSettings()
{
    QSettings settings;
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
            ui->statusbar->showMessage("清理系统工作集失败，可能需要管理员权限", 3000);
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

    // 获取当前背景颜色
    QColor bgColor = palette().window().color();
    QString textColor = getContrastColor(bgColor).name();

    // 物理内存显示
    double usedPhysGB = (memStatus.ullTotalPhys - memStatus.ullAvailPhys) / (1024.0 * 1024 * 1024);
    double totalPhysGB = memStatus.ullTotalPhys / (1024.0 * 1024 * 1024);
    ui->lbPhysicalValue->setText(
        QString("<span style='color:%1;'>%2%</span> (%3 GB / %4 GB)")
            .arg(textColor)
            .arg(memStatus.dwMemoryLoad)
            .arg(usedPhysGB, 0, 'f', 2)
            .arg(totalPhysGB, 0, 'f', 2));

    // 虚拟内存显示
    double usedVirtGB = (memStatus.ullTotalPageFile - memStatus.ullAvailPageFile) / (1024.0 * 1024 * 1024);
    double totalVirtGB = memStatus.ullTotalPageFile / (1024.0 * 1024 * 1024);
    int virtUsagePercent = (totalVirtGB > 0) ? static_cast<int>((usedVirtGB / totalVirtGB) * 100) : 0;
    ui->lbVirtualValue->setText(
        QString("<span style='color:%1;'>%2%</span> (%3 GB / %4 GB)")
            .arg(textColor)
            .arg(virtUsagePercent)
            .arg(usedVirtGB, 0, 'f', 2)
            .arg(totalVirtGB, 0, 'f', 2));

    // 系统工作集显示
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



    // 设置进度值（但不显示文本）
    ui->pbPhysical->setValue(memStatus.dwMemoryLoad);
    ui->pbPhysical->setFormat("");  // 清空格式

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

    // 简化计算：假设共享工作集占总工作集的30%
    sharedWS = totalWS * 0.3;
    privateWS = totalWS - sharedWS;

    return true;
}




// 在 MainWindow 类中添加一个 QLabel* 成员变量用于显示百分比
// 例如：QLabel* memoryPercentLabel;

void MainWindow::setProgressBarStyle(QProgressBar* bar, int value, QLabel*& label)
{
    // 恢复默认样式
    bar->setStyleSheet("");
    bar->setFormat("");

    // 创建或更新 QLabel
    if (!label) {
        label = new QLabel(this);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setStyleSheet("QLabel { padding-right: 5px; color: black; }");
    }

    // 更新位置和文本
    label->setGeometry(
        bar->x() + bar->width() - 50,  // 右侧位置
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
    // 通过计算亮度值决定使用深色还是浅色文本
    const double luminance = (0.299 * background.redF() +
                              0.587 * background.greenF() +
                              0.114 * background.blueF());
    return luminance > 0.5 ? Qt::black : Qt::white;
}



void MainWindow::on_btnCleanNow_clicked()
{
    // 记录清理前内存
    MEMORYSTATUSEX memBefore;
    memBefore.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memBefore);

    // 执行清理
    ui->statusbar->showMessage("正在清理内存...");
    cleanSelectedMemory();
    displayMemoryInfo();

    // 计算清理量
    MEMORYSTATUSEX memAfter;
    memAfter.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memAfter);

    double cleaned = (memAfter.ullAvailPhys - memBefore.ullAvailPhys) / (1024.0 * 1024);
    ui->textBrowser->append(QString("[%1] 已清理内存: %2 MB")
                                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                                .arg(cleaned, 0, 'f', 1));

    ui->statusbar->showMessage("内存清理完成", 3000);
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

    // 获取系统工作集信息
    quint64 totalWS = 0, sharedWS = 0, privateWS = 0;
    getSystemWorkingSetInfo(totalWS, sharedWS, privateWS);

        // 更新托盘图标
    trayIcon->setIcon(createTextIcon(memStatus.dwMemoryLoad));


    // 计算内存使用量(GB)
    double usedPhysGB = (memStatus.ullTotalPhys - memStatus.ullAvailPhys) / (1024.0 * 1024 * 1024);
    double totalPhysGB = memStatus.ullTotalPhys / (1024.0 * 1024 * 1024);
    double usedVirtGB = (memStatus.ullTotalPageFile - memStatus.ullAvailPageFile) / (1024.0 * 1024 * 1024);
    double totalVirtGB = memStatus.ullTotalPageFile / (1024.0 * 1024 * 1024);
    double privateWSGB = privateWS / (1024.0 * 1024 * 1024);
    double totalWSGB = totalWS / (1024.0 * 1024 * 1024);

    // 更新托盘提示，显示更详细的内存信息
    QString toolTip = QString(
                          "内存状态:\n"
                          "物理内存: %1/%2 GB (%3%)\n"
                          "虚拟内存: %4/%5 GB\n"
                          "系统工作集: %6/%7 GB")
                          .arg(usedPhysGB, 0, 'f', 1)
                          .arg(totalPhysGB, 0, 'f', 1)
                          .arg(memStatus.dwMemoryLoad)
                          .arg(usedVirtGB, 0, 'f', 1)
                          .arg(totalVirtGB, 0, 'f', 1)
                          .arg(privateWSGB, 0, 'f', 1)
                          .arg(totalWSGB, 0, 'f', 1);

    trayIcon->setToolTip(toolTip);

    displayMemoryInfo();
}


QIcon MainWindow::createTextIcon(int usagePercent) {
    // 使用48x48画布（系统托盘允许的最大尺寸之一）
    QPixmap pixmap(48, 48);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    // 设置背景色（保持进度条颜色逻辑）
    QColor bgColor;
    if (usagePercent > 90)       bgColor = QColor(0xD3, 0x2F, 0x2F);
    else if (usagePercent > 70)  bgColor = QColor(0xF5, 0x7C, 0x00);
    else                        bgColor = QColor(0x0, 0x80, 0x40);
    painter.fillRect(pixmap.rect(), bgColor);

    // 设置最大化字体
    QString text = QString::number(usagePercent) ;
    QFont font;
    font.setFamily("Arial");
    font.setWeight(QFont::Normal);

    // 动态计算最大可能字号
    int fontSize = 40; // 初始尝试值
    font.setPixelSize(fontSize);
    QFontMetrics fm(font);

    // 双重约束：宽度不超过画布，高度不超过画布
    while ((fm.horizontalAdvance(text) > pixmap.width() - 2 ||
            fm.height() > pixmap.height()) &&
           fontSize > 10) {
        fontSize--;
        font.setPixelSize(fontSize);
        fm = QFontMetrics(font);
    }

    // 精确计算绘制位置（完全顶格）
    QRect textRect = pixmap.rect();
    textRect.adjust(1, -2, -1, 0); // 微调边距

    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(textRect, Qt::AlignCenter | Qt::AlignVCenter, text);

    return QIcon(pixmap);
}

void MainWindow::onTrayCleanWorkingSet() {
    saveCurrentOptions();
    if (trayWorkingSetAction->isChecked()) {
        MemoryCleaner::cleanAllProcessesWorkingSet();
    }
}

void MainWindow::onTrayCleanSystemWS() {
    saveCurrentOptions();
    if (traySystemWSAction->isChecked()) {
        MemoryCleaner::cleanSystemWorkingSet();
    }
}

void MainWindow::saveCurrentOptions() {
    QSettings settings;

    // 保存当前所有选项状态
    settings.setValue("cleanOptions/workingSet", trayWorkingSetAction->isChecked());
    settings.setValue("cleanOptions/systemWS", traySystemWSAction->isChecked());
    settings.setValue("cleanOptions/standby", trayStandbyListAction->isChecked());
    settings.setValue("cleanOptions/standbyLow", trayStandbyListLowAction->isChecked());
    settings.setValue("cleanOptions/modified", trayModifiedListAction->isChecked());
    settings.setValue("cleanOptions/combined", trayCombinedListAction->isChecked());

    // 确保主窗口同步
    syncTrayMenuWithSettings();
}

void MainWindow::syncTrayMenuWithSettings() {
    QSettings settings;

    trayWorkingSetAction->setChecked(settings.value("cleanOptions/workingSet", true).toBool());
    traySystemWSAction->setChecked(settings.value("cleanOptions/systemWS", false).toBool());
    // 从设置同步到托盘菜单
    trayWorkingSetAction->setChecked(settings.value("cleanOptions/workingSet", true).toBool());
    traySystemWSAction->setChecked(settings.value("cleanOptions/systemWS", false).toBool());
    trayStandbyListAction->setChecked(settings.value("cleanOptions/standby", true).toBool());
    trayStandbyListLowAction->setChecked(settings.value("cleanOptions/standbyLow", false).toBool());
    trayModifiedListAction->setChecked(settings.value("cleanOptions/modified", false).toBool());
    trayCombinedListAction->setChecked(settings.value("cleanOptions/combined", false).toBool());

    // 同时同步到主窗口UI
    ui->cbWorkingSet->setChecked(trayWorkingSetAction->isChecked());
    ui->cbSystemWorkingSet->setChecked(traySystemWSAction->isChecked());
    ui->cbStandbyList->setChecked(trayStandbyListAction->isChecked());
    ui->cbStandbyListLow->setChecked(trayStandbyListLowAction->isChecked());
    ui->cbModifiedPageList->setChecked(trayModifiedListAction->isChecked());
    ui->cbCombinePageList->setChecked(trayCombinedListAction->isChecked());


     ui->cbWorkingSet->setChecked(trayWorkingSetAction->isChecked());
}


void MainWindow::onTrayCleanStandbyList() {
    saveCurrentOptions();
    if (trayStandbyListAction->isChecked()) {
        MemoryCleaner::cleanStandbyList(false);  // Changed to MemoryCleaner call
        trayIcon->showMessage("内存清理", "备用列表清理完成", QSystemTrayIcon::Information, 2000);
    }
    updateMemoryInfo();
}

void MainWindow::onTrayCleanStandbyListLow() {
    saveCurrentOptions();
    if (trayStandbyListLowAction->isChecked()) {
        MemoryCleaner::cleanStandbyList(true);  // Changed to MemoryCleaner call
        trayIcon->showMessage("内存清理", "低优先级备用列表清理完成", QSystemTrayIcon::Information, 2000);
    }
    updateMemoryInfo();
}

void MainWindow::onTrayCleanModifiedList() {
    saveCurrentOptions();
    if (trayModifiedListAction->isChecked()) {
        MemoryCleaner::cleanModifiedPageList();  // Changed to MemoryCleaner call
        trayIcon->showMessage("内存清理", "修改页面列表清理完成", QSystemTrayIcon::Information, 2000);
    }
    updateMemoryInfo();
}

void MainWindow::onTrayCleanCombinedList() {
    saveCurrentOptions();
    if (trayCombinedListAction->isChecked()) {
        MemoryCleaner::cleanCombinePageList();  // Changed to MemoryCleaner call
        trayIcon->showMessage("内存清理", "组合页面列表清理完成", QSystemTrayIcon::Information, 2000);
    }
    updateMemoryInfo();
}

void MainWindow::showAboutFromTray() {
    AboutDialog aboutDialog(this);
    aboutDialog.exec();
}

void MainWindow::openAuthorWebsite() {
    QDesktopServices::openUrl(QUrl("https://github.com/TheUserWW"));}
