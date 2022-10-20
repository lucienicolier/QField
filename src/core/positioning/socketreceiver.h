/***************************************************************************
 socketreceiver.h - SocketReceiver

 ---------------------
 begin                : September 2022
 copyright            : (C) 2022 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef SOCKETRECEIVER_H
#define SOCKETRECEIVER_H

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"
#include "nmeagnssreceiver.h"
#include "qgsnmeaconnection.h"

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

/**
 * The sockethreceiver connects to a device and feeds the QgsNmeaConnection over a socket.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 */
class SocketReceiver : public NmeaReceiver
{
    Q_OBJECT

  public:
    explicit SocketReceiver( const QString &address = QString(), const int port = 0, QObject *parent = nullptr );

  private:
    QTcpSocket *mSocket = nullptr;
    QString mAddress;
    int mPort = 0;
    QTimer mReconnectTimer;

  private slots:
    void onStateChanged( QAbstractSocket::SocketState socketState );
};

#endif // SOCKETRECEIVER_H
