#ifndef QUERIER_H
#define QUERIER_H

#include <QObject>
#include <QUrl>
#include <QTimer>
#include <QModbusTcpClient>


class Querier : public QObject
{
    Q_OBJECT

private:
    QTimer *m_pTimer;
    QModbusTcpClient *m_pModbusClient;
    int m_StartAddress;
    int m_QuantityToRead;
    int m_SlaveAddress;
    int m_PollInterval;
    QString m_FileName;

    bool Connect();

private slots:
    void doQuery();

public:
    static const int TimeOutMs = 2000;

    Querier( QUrl server, int startAddress, int quantityToRead, int slaveAddress, int pollInterval, QString fileName );

    void Start();
};

#endif // QUERIER_H
