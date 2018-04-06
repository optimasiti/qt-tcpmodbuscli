#include <QCoreApplication>
#include <QThread>
#include <QUrl>
#include <QDebug>

#include "querier.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if( argc != 7 )
    {
        qInfo()<<"Unexpected parameters count!";
        qInfo()<<"Usage: tcpmodbuscli.exe host:port startAddress quantityToRead slave pollIntervalMseconds fileName";
        return 0;
    }

    // Missing some checks ...

    QString host = argv[1];
    int startAddress = QString("%1").arg(argv[2]).toInt();
    int quantityToRead = QString("%1").arg(argv[3]).toInt();
    int slave = QString("%1").arg(argv[4]).toInt();
    int pollInterval = QString("%1").arg(argv[5]).toInt();
    QString fileName = argv[6];

    if( pollInterval < Querier::TimeOutMs)
    {
        qInfo()<<QString("Minimum poll interval allowed = ms ").arg(Querier::TimeOutMs);
        qInfo()<<"Usage: tcpmodbuscli.exe host:port startAddress quantityToRead slave pollIntervalMseconds fileName";
        return 0;
    }

    Querier querier(QUrl::fromUserInput(host), startAddress, quantityToRead, slave, pollInterval, fileName );
    querier.Start();
    return a.exec();
}
