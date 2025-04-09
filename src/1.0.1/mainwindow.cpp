#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QCloseEvent>
#include <QDebug>
#include <Psapi.h>
#include <TlHelp32.h>
#include <winternl.h>

typedef enum _SYSTEM_MEMORY_LIST_COMMAND {
    MemoryPurgeStandbyList = 1,
    MemoryPurgeLowPriorityStandbyList,
    MemoryPurgeModifiedPageList,
    MemoryPurgeCombinedPageList
} SYSTEM_MEMORY_LIST_COMMAND;

#define SystemMemoryListInformation 80

extern "C" {
NTSTATUS NTAPI NtSetSystemInformation(
    _In_ INT SystemInformationClass,
    _Inout_ PVOID SystemInformation,
    _In_ ULONG SystemInformationLength
    );
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutDialog);
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

void MainWindow::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/app.ico"));
    trayIcon->setToolTip("内存清理工具");

    trayMenu = new QMenu(this);
    QAction *showAction = new QAction("显示窗口", this);
    QAction *hideAction = new QAction("隐藏窗口", this);
    QAction *cleanAction = new QAction("立即清理", this);
    QAction *aboutAction = new QAction("关于", this);
    QAction *exitAction = new QAction("退出", this);

    connect(showAction, &QAction::triggered, this, &MainWindow::showWindow);
    connect(hideAction, &QAction::triggered, this, &MainWindow::hideWindow);
    connect(cleanAction, &QAction::triggered, this, &MainWindow::on_btnCleanNow_clicked);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApp);

    trayMenu->addAction(showAction);
    trayMenu->addAction(hideAction);
    trayMenu->addSeparator();
    trayMenu->addAction(cleanAction);
    trayMenu->addSeparator();
    trayMenu->addAction(aboutAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
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

void MainWindow::displayMemoryInfo()
{
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memStatus);

    // 物理内存信息
    double usedPhysGB = (memStatus.ullTotalPhys - memStatus.ullAvailPhys) / (1024.0 * 1024 * 1024);
    double totalPhysGB = memStatus.ullTotalPhys / (1024.0 * 1024 * 1024);
    ui->lbPhysicalValue->setText(
        QString("%1 GB / %2 GB")
            .arg(usedPhysGB, 0, 'f', 2)
            .arg(totalPhysGB, 0, 'f', 2));
    ui->pbPhysical->setValue(memStatus.dwMemoryLoad);

    // 虚拟内存信息
    double usedVirtGB = (memStatus.ullTotalPageFile - memStatus.ullAvailPageFile) / (1024.0 * 1024 * 1024);
    double totalVirtGB = memStatus.ullTotalPageFile / (1024.0 * 1024 * 1024);
    int virtUsagePercent = (totalVirtGB > 0) ?
                               static_cast<int>((usedVirtGB / totalVirtGB) * 100) : 0;

    ui->lbVirtualValue->setText(
        QString("%1 GB / %2 GB")
            .arg(usedVirtGB, 0, 'f', 2)
            .arg(totalVirtGB, 0, 'f', 2));
    ui->pbVirtual->setValue(virtUsagePercent);

    // 系统工作集信息
    quint64 totalWS = 0, sharedWS = 0, privateWS = 0;
    if (getSystemWorkingSetInfo(totalWS, sharedWS, privateWS)) {
        double totalWSGB = totalWS / (1024.0 * 1024 * 1024);
        double privateWSGB = privateWS / (1024.0 * 1024 * 1024);
        int wsUsagePercent = totalWS > 0 ? static_cast<int>((privateWS * 100) / totalWS) : 0;

        ui->lbSystemWSValue->setText(
            QString("%1 GB (共享 %2 GB) / %3 GB")
                .arg(privateWSGB, 0, 'f', 2)
                .arg((sharedWS / (1024.0 * 1024 * 1024)), 0, 'f', 2)
                .arg(totalWSGB, 0, 'f', 2));
        ui->pbSystemWS->setValue(wsUsagePercent);
    }

    setProgressBarStyle(ui->pbPhysical, memStatus.dwMemoryLoad);
    setProgressBarStyle(ui->pbVirtual, virtUsagePercent);
    setProgressBarStyle(ui->pbSystemWS, (totalWS > 0) ? static_cast<int>((privateWS * 100) / totalWS) : 0);
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

void MainWindow::setProgressBarStyle(QProgressBar* bar, int value)
{
    QString styleSheet = QString(
                             "QProgressBar {"
                             "    border: 1px solid #BDBDBD;"
                             "    border-radius: 6px;"
                             "    background: %1;"
                             "    text-align: center;"
                             "    font-size: 10pt;"
                             "    font-weight: 500;"
                             "    color: %2;"
                             "    min-height: 22px;"
                             "}"
                             "QProgressBar::chunk {"
                             "    border-radius: 5px;"
                             "    %3"
                             "    border-right: 1px solid rgba(255,255,255,0.3);"
                             "}"
                             ).arg(
                                 value > 90 ? "#FFEBEE" : value > 70 ? "#FFF3E0" : "#E8F5E9",
                                 value > 70 ? "white" : "black",
                                 value > 90 ? "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FF5252, stop:1 #D32F2F);" :
                                     value > 70 ? "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #FFB74D, stop:1 #F57C00);" :
                                     "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #81C784, stop:1 #388E3C);"
                                 );

    bar->setStyleSheet(styleSheet);
    bar->setFormat(QString("%1%").arg(value));
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

    // 更新托盘提示
    trayIcon->setToolTip(QString("内存使用率: %1%").arg(memStatus.dwMemoryLoad));

    displayMemoryInfo();
}

bool MainWindow::cleanWorkingSet()
{
    return EmptyWorkingSet(GetCurrentProcess());
}

void MainWindow::cleanSelectedMemory()
{
    bool cleanWorkingSet = ui->cbWorkingSet->isChecked();

    if (cleanWorkingSet) {
        if (!cleanAllProcessesWorkingSet()) {
            ui->statusbar->showMessage("清理系统工作集失败，可能需要管理员权限", 3000);
        }
    }

    if (ui->cbSystemWorkingSet->isChecked()) {
        cleanSystemWorkingSet();
    }
    if (ui->cbStandbyList->isChecked()) {
        cleanStandbyList(false);
    }
    if (ui->cbStandbyListLow->isChecked()) {
        cleanStandbyList(true);
    }
    if (ui->cbModifiedPageList->isChecked()) {
        cleanModifiedPageList();
    }
    if (ui->cbCombinePageList->isChecked()) {
        cleanCombinePageList();
    }
}

bool MainWindow::cleanSystemWorkingSet()
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        qDebug() << "CreateToolhelp32Snapshot failed:" << GetLastError();
        return false;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        qDebug() << "Process32First failed:" << GetLastError();
        CloseHandle(hProcessSnap);
        return false;
    }

    do {
        if (pe32.th32ProcessID == 0) {
            continue;
        }

        HANDLE hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA | PROCESS_VM_OPERATION | PROCESS_VM_READ,
            FALSE,
            pe32.th32ProcessID);

        if (hProcess != NULL) {
            EmptyWorkingSet(hProcess);
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return true;
}

bool MainWindow::cleanStandbyList(bool clearLowPriority)
{
    SYSTEM_MEMORY_LIST_COMMAND command = clearLowPriority ?
                                             MemoryPurgeLowPriorityStandbyList : MemoryPurgeStandbyList;

    return NT_SUCCESS(NtSetSystemInformation(
        SystemMemoryListInformation,
        &command,
        sizeof(SYSTEM_MEMORY_LIST_COMMAND)
        ));
}

bool MainWindow::cleanModifiedPageList()
{
    SYSTEM_MEMORY_LIST_COMMAND command = MemoryPurgeModifiedPageList;

    return NT_SUCCESS(NtSetSystemInformation(
        SystemMemoryListInformation,
        &command,
        sizeof(SYSTEM_MEMORY_LIST_COMMAND)
        ));
}

bool MainWindow::cleanCombinePageList()
{
    SYSTEM_MEMORY_LIST_COMMAND command = MemoryPurgeCombinedPageList;

    return NT_SUCCESS(NtSetSystemInformation(
        SystemMemoryListInformation,
        &command,
        sizeof(SYSTEM_MEMORY_LIST_COMMAND)
        ));
}

bool MainWindow::cleanAllProcessesWorkingSet()
{
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

        HANDLE hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA | PROCESS_VM_OPERATION | PROCESS_VM_READ,
            FALSE,
            pe32.th32ProcessID);
        if (hProcess) {
            EmptyWorkingSet(hProcess);
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return true;
}
