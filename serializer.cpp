#include "serializer.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

void Serializer::Serialize( QVector<quint16> values, QString fileName )
{
    QFile file( fileName );

    if( !file.open(QIODevice::Append ))
    {
        qInfo()<<"Cannot write file";
        return;
    }

    QTextStream stream( &file );

    stream<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")<<";";

    for( int i = 0; i < values.count(); i++ )
    {
        stream<<values.at(i);
        if( i < values.count()-1)
        {
            stream<<";";
        }
    }

    stream<<"\n";

    file.close();
}
