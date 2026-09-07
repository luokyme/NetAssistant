#include <QtTest>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QPlainTextEdit>
#include "mainwindow.h"

class NetworkTest : public QObject
{
    Q_OBJECT
    template<class T> T *widget(MainWindow &window, const char *name)
    {
        return window.findChild<T *>(name);
    }
    void endpoint(MainWindow &window, int mode, const QString &host, quint16 port)
    {
        widget<QComboBox>(window, "cBoxNetType")->setCurrentIndex(mode);
        widget<QLineEdit>(window, "lEditIpAddr")->setText(host);
        widget<QLineEdit>(window, "lEditIpPort")->setText(QString::number(port));
    }
    void closeMessage()
    {
        QTimer::singleShot(0, [] {
            for(QWidget *widget : QApplication::topLevelWidgets())
                if(QMessageBox *message = qobject_cast<QMessageBox *>(widget))
                    message->accept();
        });
    }
private slots:
    void addressEntry()
    {
        MainWindow window;
        for(const char *name : {"lEditIpAddr", "lEditUdpIP"}) {
            QLineEdit *edit = widget<QLineEdit>(window, name);
            edit->clear();
            QTest::keyClicks(edit, "192.168.1.10");
            QCOMPARE(edit->text(), QString("192.168.1.10"));
        }
    }
    void udpBindingAndTimerCleanup()
    {
        QUdpSocket receiver;
        QVERIFY(receiver.bind(QHostAddress(QHostAddress::LocalHost), 0));
        MainWindow window;
        endpoint(window, 0, "127.0.0.1", 0);
        widget<QLineEdit>(window, "lEditUdpIP")->setText("127.0.0.1");
        widget<QLineEdit>(window, "lEditUdpPort")->setText(QString::number(receiver.localPort()));
        QPushButton *connect = widget<QPushButton>(window, "pBtnNetCnnt");
        connect->click();
        QVERIFY(connect->isChecked());
        QUdpSocket *socket = window.findChild<QUdpSocket *>();
        QVERIFY(socket);
        QCOMPARE(socket->localAddress(), QHostAddress(QHostAddress::LocalHost));
        QVERIFY(socket->localPort() != 0);
        widget<QPlainTextEdit>(window, "tEditSendText")->setPlainText("udp-test");
        widget<QPushButton>(window, "pBtnSendData")->click();
        QTRY_VERIFY(receiver.hasPendingDatagrams());
        QByteArray received(8, 0);
        QCOMPARE(receiver.readDatagram(received.data(), received.size()), qint64(8));
        QCOMPARE(received, QByteArray("udp-test"));
        widget<QCheckBox>(window, "cBoxLoopSnd")->setChecked(true);
        widget<QPushButton>(window, "pBtnSendData")->click();
        QVERIFY(window.findChild<QTimer *>()->isActive());
        connect->click();
        QVERIFY(!window.findChild<QTimer *>()->isActive());
        QVERIFY(!widget<QPushButton>(window, "pBtnSendData")->isEnabled());
        QVERIFY(widget<QComboBox>(window, "cBoxNetType")->isEnabled());
    }
    void hostnameConnectionAndRemoteDisconnect()
    {
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::Any, 0));
        MainWindow window;
        endpoint(window, 2, "localhost", server.serverPort());
        QPushButton *connect = widget<QPushButton>(window, "pBtnNetCnnt");
        connect->click();
        QVERIFY(connect->isChecked());
        QTRY_VERIFY(server.hasPendingConnections());
        QTcpSocket *peer = server.nextPendingConnection();
        QVERIFY(peer);
        widget<QPlainTextEdit>(window, "tEditSendText")->setPlainText("tcp-test");
        widget<QPushButton>(window, "pBtnSendData")->click();
        QTRY_VERIFY(peer->bytesAvailable() > 0);
        QCOMPARE(peer->readAll(), QByteArray("tcp-test"));
        peer->disconnectFromHost();
        QTRY_VERIFY(!connect->isChecked());
        QVERIFY(widget<QComboBox>(window, "cBoxNetType")->isEnabled());
    }
    void tcpServerRestart()
    {
        MainWindow window;
        endpoint(window, 1, "127.0.0.1", 0);
        QPushButton *connect = widget<QPushButton>(window, "pBtnNetCnnt");
        for(int attempt = 0; attempt < 2; ++attempt) {
            connect->click();
            QVERIFY(connect->isChecked());
            TcpServer *server = window.findChild<TcpServer *>();
            QVERIFY(server);
            QTcpSocket peer;
            peer.connectToHost(QHostAddress::LocalHost, server->serverPort());
            QVERIFY(peer.waitForConnected());
            QTRY_COMPARE(window.TcpClientLinkCnt, 1);
            const QByteArray payload = QString::fromUtf8("中文测试").toUtf8();
            peer.write(payload);
            peer.flush();
            QTRY_VERIFY(widget<QPlainTextEdit>(window, "ReceiveTextEdit")->toPlainText()
                        .contains(QString::fromUtf8(payload)));
            connect->click();
            QCOMPARE(window.TcpClientLinkCnt, 0);
            QCOMPARE(widget<QComboBox>(window, "cBoxClients")->count(), 0);
        }
    }
    void failedTcpConnectionCanRetry()
    {
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost, 0));
        const quint16 port = server.serverPort();
        server.close();
        MainWindow window;
        endpoint(window, 2, "127.0.0.1", port);
        QPushButton *connect = widget<QPushButton>(window, "pBtnNetCnnt");
        closeMessage();
        connect->click();
        QVERIFY(!connect->isChecked());
        QVERIFY(!window.findChild<QTcpSocket *>());
        QVERIFY(server.listen(QHostAddress::LocalHost, port));
        connect->click();
        QVERIFY(connect->isChecked());
        connect->click();
    }
    void invalidInputAndFailedBind()
    {
        MainWindow window;
        endpoint(window, 0, "999.1.2.3", 0);
        QPushButton *connect = widget<QPushButton>(window, "pBtnNetCnnt");
        closeMessage();
        connect->click();
        QVERIFY(!connect->isChecked());
        QVERIFY(!window.findChild<QUdpSocket *>());
        endpoint(window, 0, "127.0.0.1", 0);
        widget<QLineEdit>(window, "lEditUdpPort")->setText("65536");
        closeMessage();
        connect->click();
        QVERIFY(!connect->isChecked());
        QVERIFY(!window.findChild<QUdpSocket *>());
        QUdpSocket occupied;
        QVERIFY(occupied.bind(QHostAddress(QHostAddress::LocalHost), 0, QUdpSocket::DontShareAddress));
        endpoint(window, 0, "127.0.0.1", occupied.localPort());
        widget<QLineEdit>(window, "lEditUdpPort")->setText("8081");
        closeMessage();
        connect->click();
        QVERIFY(!connect->isChecked());
        QVERIFY(!window.findChild<QUdpSocket *>());
    }
};
QTEST_MAIN(NetworkTest)
#include "network_test.moc"
