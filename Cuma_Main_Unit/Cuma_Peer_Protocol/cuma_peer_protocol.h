#ifndef CUMA_PEER_PROTOCOL_H
#define CUMA_PEER_PROTOCOL_H

#include <QObject>

class Cuma_Peer_Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Peer_Protocol(QObject *parent = 0);

signals:

public slots:
};

#endif // CUMA_PEER_PROTOCOL_H