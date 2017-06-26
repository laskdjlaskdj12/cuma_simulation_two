#ifndef CUMA_PEER_SERVER_H
#define CUMA_PEER_SERVER_H

#include <QObject>

class Cuma_Peer_Server : public QObject
{
    Q_OBJECT
public:
    explicit Cuma_Peer_Server(QObject *parent = 0);

signals:

public slots:
};

#endif // CUMA_PEER_SERVER_H