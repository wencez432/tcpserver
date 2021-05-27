#include "tcpserver.h"
#include "ui_tcpserver.h"

TcpServer::TcpServer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    // Init the server
    initServer();
    // Random messages
    for(int i = 1; i <= 20; i++){
        randomMessages << "Mensaje aliatorio " + QString::number(i);
    }
    // connects
    connect(ui->btn_close, &QAbstractButton::clicked, this, &QWidget::close);
    // connect for the server socket
    connect(qTcpServer, &QTcpServer::newConnection, this, &TcpServer::sendRandomMessage);
}

TcpServer::~TcpServer()
{
    delete ui;
}

/**
 * @brief TcpServer::initServer
 */
void TcpServer::initServer()
{
    qInfo() << "init server";
    qTcpServer = new QTcpServer(this);
    if (!qTcpServer->listen()) {
        qInfo() << "Server error: " + qTcpServer->errorString();
        return;
    }
    QString port = QString::number(qTcpServer->serverPort(), 10);
    QString ipAddress;
    QList<QHostAddress> ipAddresslist = QNetworkInterface::allAddresses();
    for (int i = 0; ipAddresslist.size(); i++) {
        if (ipAddresslist.at(i) != QHostAddress::LocalHost && ipAddresslist.at(i).toIPv4Address()) {
            ipAddress = ipAddresslist.at(i).toString();
            qInfo() << "ip: " << ipAddress;
            break;
        }
    }

    if (ipAddresslist.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }
    QString info = "ip: " + ipAddress + ", puerto: " + port;
    ui->lbl_info->setText(info);
}

void TcpServer::sendRandomMessage()
{
    qInfo() << "Send random message";
    QByteArray qByteArray;
    QDataStream qDataStream(&qByteArray, QIODevice::WriteOnly);
    QString randomMessage = randomMessages[QRandomGenerator::global()->bounded(randomMessages.size())];
    qDataStream << randomMessage;
    QTcpSocket *clientConnection = qTcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);
    clientConnection->write(qByteArray);
    clientConnection->disconnectFromHost();
    ui->lbl_message->setText(randomMessage);
}
