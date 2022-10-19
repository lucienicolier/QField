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
{
  mSocket.connectToHost( address, port, QTcpSocket::ReadOnly );
}
