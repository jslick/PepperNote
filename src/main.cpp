/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "mainwindow.h"

#include <QtCore>
#include <QApplication>
#include <QSettings>
#include <QThread>
#include <QLocalServer>
#include <QLocalSocket>

const QString APP_ORGNAME = "PepperNote";
const QString APP_APPNAME = "PepperNote";

const QString APP_SINGLE_INSTANCE_NAME = APP_APPNAME + "-instance";
// TODO:  Not compatible for mulitple users --^

/**
 * @brief Write text to a socket
 *
 * @param socket
 * @param what
 *
 * @note Does not flush the socket
 */
static void writeSocket(QLocalSocket* socket, const QString& what)
{
    if (!socket)
#if QT_VERSION >= 0x050000
        throw QException();
#else
        throw QtConcurrent::Exception();
#endif

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_8);
    out << what;
    out.device()->seek(0);
    socket->write(block);
}

class SingleInstanceAppWorker : public QObject
{
    Q_OBJECT
private:
    QLocalServer* server;

signals:
    void signalledToFront();    // Client wants to show the main window

public slots:
    // Connection; only one at a time; in a worker thread
    void talk()
    {
        QLocalSocket* clientConnection = this->server->nextPendingConnection();
        qDebug() << "Someone connected";

        // NOTE:  If the client closes early, this `while` can infinite loop
        //while (clientConnection->bytesAvailable() < (int)sizeof(quint32))
        clientConnection->waitForReadyRead();

        connect(clientConnection, SIGNAL(disconnected()),
                clientConnection, SLOT(deleteLater())
                );

        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_4_8);
        if (clientConnection->bytesAvailable() < (int)sizeof(quint16))
        {
            qDebug() << "No bytes available";
            return;
        }

        QString message;
        in >> message;
        qDebug() << "Message:  " + message;

        writeSocket(clientConnection, QString::fromLatin1("ok"));

        if (message == "to-front")
            emit signalledToFront();
    }

    // Listen for connections; in a worker thread
    void listen()
    {
        this->server = new QLocalServer(this);
        bool success = this->server->listen(APP_SINGLE_INSTANCE_NAME);
        if (!success)
        {
            qDebug() << "Unable to start server";
            return;
        }

        connect(this->server, SIGNAL(newConnection()),
                SLOT(talk())
                );
    }
};
#include "main.moc"

QThread* startServer(QApplication& app, MainWindow& w)
{
    SingleInstanceAppWorker* worker = new SingleInstanceAppWorker;
    QThread* thread = new QThread(&app);
    QObject::connect(thread, SIGNAL(started()),
                     worker, SLOT(listen())
                     );
    QObject::connect(thread, SIGNAL(finished()),
                     worker, SLOT(deleteLater())
                     );
    QObject::connect(worker, SIGNAL(signalledToFront()),
                     &w, SLOT(bringToFront())
                     );
    worker->moveToThread(thread);

    thread->start();

    return thread;
}

/**
 * @param app
 * @return true if connection is made; otherwise false
 */
bool connectToServer(QApplication& app)
{
    QLocalSocket* socket = new QLocalSocket(&app);
    socket->connectToServer(APP_SINGLE_INSTANCE_NAME);
    bool connected = socket->waitForConnected(0);
    if (!connected)
        return false;

    qDebug() << "Found instance";

    writeSocket(socket, QString::fromLatin1("to-front"));
    socket->flush();

    socket->waitForReadyRead();

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_8);

    QString response;
    in >> response;
    qDebug() << response;

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(APP_ORGNAME);
    QCoreApplication::setApplicationName(APP_APPNAME);
    QSettings::setDefaultFormat(QSettings::IniFormat);

    QApplication a(argc, argv);

    // Enforce only one running instance at a time.  If instance is found via
    // socket, return early
    if (connectToServer(a))
        return 0;

    MainWindow w;

    // Listen for multiple simultaneous instances
    QThread* serverThread = startServer(a, w);

    w.show();
    int rv = a.exec();

    serverThread->quit();
    return rv;
}
