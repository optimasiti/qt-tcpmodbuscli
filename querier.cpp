#include <QModbusReply>
#include <QDateTime>
#include <QThread>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>

#include "querier.h"
#include "serializer.h"

Querier::Querier( QUrl server, int startAddress, int quantityToRead, int slaveAddress, int pollInterval, QString fileName )
{
    m_StartAddress = startAddress;
    m_QuantityToRead = quantityToRead;
    m_SlaveAddress = slaveAddress;
    m_PollInterval = pollInterval;
    m_FileName = fileName;

    m_pTimer = new QTimer(this);
    m_pModbusClient = new QModbusTcpClient(this);

    m_pModbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, server.port() );
    m_pModbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, server.host());
    m_pModbusClient->setTimeout(TimeOutMs);
    m_pModbusClient->setNumberOfRetries(0);

    Connect();
}

void Querier::Start()
{
    doQuery();
    connect( m_pTimer, SIGNAL(timeout()), this, SLOT(doQuery()));
    m_pTimer->start(m_PollInterval);
}

void Querier::doQuery()
{
    if( !Connect() )
    {
        return;
    }

    if( QModbusReply *pModbusReply = m_pModbusClient->sendReadRequest(
                                     QModbusDataUnit( QModbusDataUnit::HoldingRegisters,
                                                      m_StartAddress,
                                                      m_QuantityToRead),
                                                      m_SlaveAddress ) )
    {
        QDateTime endTs = QDateTime::currentDateTime().addMSecs(TimeOutMs);
        while(QDateTime::currentDateTime() < endTs)
        {
            QThread::usleep(50000);
            QCoreApplication::processEvents();
            if( pModbusReply->isFinished())
            {
                break;
            }
        }

        if( pModbusReply->isFinished() && pModbusReply->error() == QModbusDevice::NoError)
        {
            const QModbusDataUnit unit = pModbusReply->result();
            Serializer::Serialize(unit.values(), m_FileName );
        }
        else
        {
            qInfo()<<pModbusReply->errorString();
        }

        delete pModbusReply;
    }
}

bool Querier::Connect()
{
    if( m_pModbusClient->state() == QModbusDevice::ConnectedState )
    {
        return true;
    }

    m_pModbusClient->connectDevice();

    QDateTime endTs = QDateTime::currentDateTime().addMSecs(TimeOutMs);
    while(QDateTime::currentDateTime() < endTs)
    {
        QThread::usleep(50000);
        QCoreApplication::processEvents();
    }

    return m_pModbusClient->state() == QModbusDevice::ConnectedState;
}
