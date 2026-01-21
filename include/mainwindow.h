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
#include <QMessageBox>
#include <QDateTime>
#include <QCloseEvent>
#include <QDebug>
#include <Psapi.h>
#include <TlHelp32.h>
#include <QDesktopServices>
#include <QUrl>
#include <winternl.h>
#include <QFile>
#include <QFileInfo>
#include <QTranslator>
#include <QApplication>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void cleanSelectedMemory();
    



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
    void onLanguageChanged(const QString& locale);
    void onChineseTriggered();
    void onTraditionalChineseTriggered();
    void onMongolianTriggered();
    void onUyghurTriggered();
    void onEnglishTriggered();
    void onFrenchTriggered();
    void onRussianTriggered();
    void onSpanishTriggered();
    void onGermanTriggered();
    void onItalianTriggered();
    void onArabicTriggered();
    void onKoreanTriggered();
    void onJapaneseTriggered();
    void onAutoStartTriggered();
    void onEnableMessageTriggered();

private:
    Ui::MainWindow *ui;
    QTimer *autoCleanTimer;
    QTimer *monitorTimer;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    quint64 lastUsedMemory = 0;
    QIcon createTextIcon(int usagePercent);  // 移除const修饰符
    QString m_trayToolTip;  // 新增托盘提示文本属性
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
    void changeLanguage(const QString& locale);
    QTranslator translator;
    QString currentLanguage; // 添加这行
    static const QString tooltipTemplates[13];
    void showAboutDialog();
    void updateMemoryInfo();
    bool autoStartEnabled; // 开机自启动设置
    bool enableMessage;     // 消息通知设置
    void setAutoStart(bool enable);
    bool isAutoStartEnabled();
};

#endif // MAINWINDOW_H
