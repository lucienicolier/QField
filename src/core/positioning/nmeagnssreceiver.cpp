/***************************************************************************
 bluetoothreceiver.cpp - BluetoothReceiver

 ---------------------
 begin                : 18.11.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "nmeagnssreceiver.h"

#include <QSettings>

NmeaReceiver::NmeaReceiver( QObject *parent )
  : AbstractGnssReceiver( parent )
{
}

void NmeaReceiver::initNmeaConnection( QIODevice *ioDevice )
{
  mGpsConnection = std::make_unique<QgsNmeaConnection>( ioDevice );
}

void NmeaReceiver::setEllipsoidalElevation( const bool ellipsoidalElevation )
{
  if ( mEllipsoidalElevation == ellipsoidalElevation )
    return;

  mEllipsoidalElevation = ellipsoidalElevation;
}
