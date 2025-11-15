#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QMessageBox>
#include <QDebug>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QTimer>
#include <QSettings>
#include <Windows.h> // 添加Windows头文件
#include <shellapi.h> // 用于ShellExecute





bool isAdmin() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}

// 重启应用程序并请求管理员权限
void restartAsAdmin() {
    wchar_t szPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, szPath, ARRAYSIZE(szPath))) {
        SHELLEXECUTEINFOW sei = {
            sizeof(sei),
            SEE_MASK_NOCLOSEPROCESS,
            NULL,
            L"runas",
            szPath,
            NULL,
            NULL,
            SW_SHOW
        };
        ShellExecuteExW(&sei);
    }
}




// 共享内存数据结构
struct SharedData {
    bool requestShow = false; // 激活窗口请求标志
};

int main(int argc, char *argv[])
{
    // 检查是否以管理员权限运行
    if (!isAdmin()) {
        // 如果不是管理员权限，重启应用程序
        restartAsAdmin();
        return 0;
    }

    QApplication a(argc, argv);

    // 加载保存的语言设置
    // 修改：使用应用程序目录下的settings.ini文件
    QString iniPath = QCoreApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    QString language = settings.value("language/current", QLocale::system().name()).toString();

    QTranslator translator;
    if (translator.load(":/translations/MemoryCleaner_" + language)) {
        a.installTranslator(&translator);
    } else {
        qWarning() << "Failed to load translation file for language:" << language;
    }

    // 唯一标识符
    const QString memoryKey = "MemoryCleaner_SharedMemory";
    const QString semaphoreKey = "MemoryCleaner_Semaphore";

    // 系统信号量（用于共享内存访问控制）
    QSystemSemaphore semaphore(semaphoreKey, 1, QSystemSemaphore::Create);

    // 尝试附加到共享内存
    QSharedMemory sharedMemory(memoryKey);
    semaphore.acquire(); // 锁定共享内存

    bool isFirstInstance = false;
    if (sharedMemory.attach()) {
        // 已有实例运行：发送激活请求
        SharedData* data = static_cast<SharedData*>(sharedMemory.data());
        data->requestShow = true;
        qDebug() << "已检测到运行实例，发送激活信号...";
    } else {
        // 创建共享内存
        if (sharedMemory.create(sizeof(SharedData))) {
            isFirstInstance = true;
            // 初始化共享内存数据
            SharedData* data = static_cast<SharedData*>(sharedMemory.data());
            data->requestShow = false;
        } else {
            // 处理创建失败（可能是残留内存）
            if (sharedMemory.error() == QSharedMemory::AlreadyExists) {
                sharedMemory.attach();
                isFirstInstance = false;
                SharedData* data = static_cast<SharedData*>(sharedMemory.data());
                data->requestShow = true;
            } else {
                QMessageBox::critical(nullptr, "错误", "无法创建共享内存！");
                semaphore.release();
                return -1;
            }
        }
    }

    semaphore.release(); // 解锁共享内存

    // 如果已有实例运行，直接退出
    if (!isFirstInstance) {
        return 0;
    }

    // 主窗口初始化（仅第一实例执行）
    MainWindow w;
    w.show();

    // 创建定时器检查激活请求
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        semaphore.acquire();
        SharedData* data = static_cast<SharedData*>(sharedMemory.data());
        if (data->requestShow) {
            data->requestShow = false; // 重置标志
            w.showNormal();
            w.activateWindow();
            w.raise();
        }
        semaphore.release();
    });
    timer.start(500); // 每500ms检查一次

    // 程序退出时清理共享内存
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        semaphore.acquire();
        sharedMemory.detach();
        semaphore.release();
    });

    return a.exec();
}
