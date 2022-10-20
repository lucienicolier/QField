/***************************************************************************
 socketreceiver.cpp - SocketReceiver

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

#include "socketreceiver.h"

#include <QSettings>

SocketReceiver::SocketReceiver( const QString &address, const int port, QObject *parent )
  : NmeaReceiver( parent )
  , mAddress( address )
  , mPort( port )
{
  mSocket = new QTcpSocket();
  connect( mSocket, &QTcpSocket::stateChanged, this, &SocketReceiver::onStateChanged );
  mReconnectTimer.setSingleShot( true );
  connect( &mReconnectTimer, &QTimer::timeout, this, [this]() { mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadOnly ); } );
  mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadOnly );
  setValid( true );
  initNmeaConnection( mSocket );
}

void SocketReceiver::onStateChanged( QAbstractSocket::SocketState socketState )
{
  qInfo() << "State changed " << socketState;
  switch ( socketState )
  {
    case QAbstractSocket::UnconnectedState:
      mReconnectTimer.start( 2000 );
  }
}
