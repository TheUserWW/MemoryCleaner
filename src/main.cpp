#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QTimer>
#include <QSettings>
#include <Windows.h>
#include <shellapi.h>





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




struct SharedData {
    bool requestShow = false;
};

int main(int argc, char *argv[])
{
    if (!isAdmin()) {
        restartAsAdmin();
        return 0;
    }

    QApplication a(argc, argv);

    QString iniPath = QCoreApplication::applicationDirPath() + "/settings.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    QString language = settings.value("language/current", QLocale::system().name()).toString();

    QTranslator translator;
    if (translator.load(":/translations/MemoryCleaner_" + language)) {
        a.installTranslator(&translator);
    } else {
        qWarning() << "Failed to load translation file for language:" << language;
    }

    const QString memoryKey = "MemoryCleaner_SharedMemory";
    const QString semaphoreKey = "MemoryCleaner_Semaphore";

    QSystemSemaphore semaphore(semaphoreKey, 1, QSystemSemaphore::Create);

    QSharedMemory sharedMemory(memoryKey);
    semaphore.acquire();

    bool isFirstInstance = false;
    if (sharedMemory.attach()) {
        SharedData* data = static_cast<SharedData*>(sharedMemory.data());
        data->requestShow = true;
        qDebug() << "Another instance detected, sending activation signal...";
    } else {
        if (sharedMemory.create(sizeof(SharedData))) {
            isFirstInstance = true;
            SharedData* data = static_cast<SharedData*>(sharedMemory.data());
            data->requestShow = false;
        } else {
            if (sharedMemory.error() == QSharedMemory::AlreadyExists) {
                sharedMemory.attach();
                isFirstInstance = false;
                SharedData* data = static_cast<SharedData*>(sharedMemory.data());
                data->requestShow = true;
            } else {
                QMessageBox::critical(nullptr, "Error", "Failed to create shared memory!");
                semaphore.release();
                return -1;
            }
        }
    }

    semaphore.release();

    if (!isFirstInstance) {
        return 0;
    }

    MainWindow w;
    w.show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        semaphore.acquire();
        SharedData* data = static_cast<SharedData*>(sharedMemory.data());
        if (data->requestShow) {
            data->requestShow = false;
            w.showNormal();
            w.activateWindow();
            w.raise();
        }
        semaphore.release();
    });
    timer.start(500);
    QObject::connect(&a, &QApplication::aboutToQuit, [&]() {
        semaphore.acquire();
        sharedMemory.detach();
        semaphore.release();
    });

    return a.exec();
}
