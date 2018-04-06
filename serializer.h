#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QVector>

class Serializer
{
public:
    static void Serialize( QVector<quint16> values, QString fileName );
};

#endif // SERIALIZER_H
