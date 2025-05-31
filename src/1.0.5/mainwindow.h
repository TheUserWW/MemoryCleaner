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
#include <QTranslator>  // 添加这行
#include <windows.h>
#include <psapi.h>
#include <QLabel>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:  // Modified this section
    void cleanSelectedMemory();
    
private:
    void showAboutDialog();  // Add this declaration
    void updateMemoryInfo();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    void on_btnCleanNow_clicked();
    void on_cbAutoClean_stateChanged(int arg1);
    void on_sbInterval_valueChanged(int arg1);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onTrayCleanWorkingSet();
    void onTrayCleanSystemWS();
    void onTrayCleanStandbyList();
    void onTrayCleanStandbyListLow();
    void onTrayCleanModifiedList();
    void onTrayCleanCombinedList();
    void showAboutFromTray();
    void openAuthorWebsite();

private:
    Ui::MainWindow *ui;
    QTimer *autoCleanTimer;
    QTimer *monitorTimer;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    quint64 lastUsedMemory = 0;
    QIcon createTextIcon(int usagePercent);
    void retranslateUi();  // 添加这行声明
    void createTrayIcon();
    QMenu* cleanSubMenu;
    void showWindow();
    void hideWindow();
    void exitApp();
    void loadSettings();
    void saveSettings();
    void displayMemoryInfo();
    bool getSystemWorkingSetInfo(quint64& totalWS, quint64& sharedWS, quint64& privateWS);
    void setProgressBarStyle(QProgressBar* bar, int value,QLabel*& label);
    QAction* trayWorkingSetAction;
    QAction* traySystemWSAction;
    QAction* trayStandbyListAction;
    QAction* trayStandbyListLowAction;
    QAction* trayModifiedListAction;
    QAction* trayCombinedListAction;
    QLabel* physicalMemLabel;    // 物理内存百分比标签
    QLabel* virtualMemLabel;     // 虚拟内存百分比标签
    QLabel* systemWSLabel;       // 系统工作集百分比标签
    // 同步选项状态
    void syncTrayMenuWithSettings();
    QColor getContrastColor(const QColor& background) const;
    void saveCurrentOptions();
    QAction* aboutAction;
    QAction* authorAction;
    private:
        void changeLanguage(const QString& locale);
        QTranslator translator;
    private:
        QString currentLanguage; // 添加这行

};

#endif // MAINWINDOW_H
