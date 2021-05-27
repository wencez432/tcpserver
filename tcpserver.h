#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class TcpServer; }
QT_END_NAMESPACE

class TcpServer : public QMainWindow
{
    Q_OBJECT

public:
    TcpServer(QWidget *parent = nullptr);
    ~TcpServer();

private:
    Ui::TcpServer *ui;
    // Socket server
    QTcpServer *qTcpServer = nullptr;
    // Message Array
    QVector<QString> randomMessages;

    // functions
    void initServer();

private slots:
    void sendRandomMessage();
};
#endif // TCPSERVER_H
