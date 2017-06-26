#ifndef CUMA_PEER_CLIENT_H
#define CUMA_PEER_CLIENT_H

#include <QObject>

class Cuma_Peer_Client : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Peer_Client(QObject *parent = 0);

signals:

public slots:
};

#endif // CUMA_PEER_CLIENT_H