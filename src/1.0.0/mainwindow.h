#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QProgressBar>
#include <QSettings>
#include <QTextBrowser>
#include <windows.h>
#include <psapi.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:  // 添加这个部分
    void showAboutDialog();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    void on_btnCleanNow_clicked();
    void on_cbAutoClean_stateChanged(int arg1);
    void on_sbInterval_valueChanged(int arg1);
    void updateMemoryInfo();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::MainWindow *ui;
    QTimer *autoCleanTimer;
    QTimer *monitorTimer;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    quint64 lastUsedMemory = 0;

    void createTrayIcon();
    void showWindow();
    void hideWindow();
    void exitApp();
    void loadSettings();
    void saveSettings();
    void displayMemoryInfo();
    bool getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS);
    void setProgressBarStyle(QProgressBar* bar, int value);
    bool cleanWorkingSet();
    void cleanSelectedMemory();
    bool cleanSystemWorkingSet();
    bool cleanStandbyList(bool clearLowPriority);
    bool cleanModifiedPageList();
    bool cleanCombinePageList();
    bool cleanAllProcessesWorkingSet();
};

#endif // MAINWINDOW_H
