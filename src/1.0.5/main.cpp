#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLocalServer>
#include <QLocalSocket>
#include <QMessageBox>
#include <QDebug>  // 添加调试输出

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 加载翻译文件
    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("MemoryCleaner"), QLatin1String("_"), 
        QLatin1String(":/translations"))) {
        a.installTranslator(&translator);
    }

    // 唯一标识符（确保全局唯一）
    const QString serverName = "MemoryCleaner_InstanceControl";

    // 尝试连接已存在的实例
    QLocalSocket socket;
    socket.connectToServer(serverName);
    if (socket.waitForConnected(500)) { // 500ms超时
        qDebug() << "已检测到运行实例，发送激活信号...";
        socket.write("SHOW");       // 发送激活指令
        socket.waitForBytesWritten(); // 确保写入完成
        socket.close();
        return 0; // 当前实例退出
    }

    // 没有其他实例运行，创建服务端
    QLocalServer server;
    if (!server.listen(serverName)) {
        // 如果监听失败，可能是残留的服务器文件未删除（仅在 Unix 系统需要）
        if (server.serverError() == QAbstractSocket::AddressInUseError) {
            QLocalServer::removeServer(serverName); // 强制清理残留
            if (!server.listen(serverName)) {
                QMessageBox::critical(nullptr, "错误", "无法启动单实例服务！");
                return -1;
            }
        }
    }

    // 主窗口初始化
    MainWindow w;
    w.show();

    // 当其他实例请求激活时，显示窗口
    QObject::connect(&server, &QLocalServer::newConnection, [&]() {
        qDebug() << "收到新连接请求...";
        QLocalSocket *clientSocket = server.nextPendingConnection();
        if (clientSocket->waitForReadyRead(1000)) { // 等待数据到达
            QString message = QString::fromUtf8(clientSocket->readAll());
            qDebug() << "收到消息：" << message;
            if (message == "SHOW") {
                // 必须通过主线程调用 UI 操作
                QMetaObject::invokeMethod(&w, [&w]() {
                    w.showNormal();    // 恢复窗口（如果最小化）
                    w.activateWindow(); // 激活到前台
                    w.raise();         // 置顶
                    qDebug() << "窗口已激活！";
                });
            }
        }
        clientSocket->close();
        delete clientSocket;
    });

    return a.exec();
}
