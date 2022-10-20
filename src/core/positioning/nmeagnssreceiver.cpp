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
  mNmeaConnection = std::make_unique<QgsNmeaConnection>( ioDevice );

  //QgsGpsConnection state changed (received location string)
  connect( mNmeaConnection.get(), &QgsGpsConnection::stateChanged, this, &NmeaReceiver::stateChanged );
}

void NmeaReceiver::setEllipsoidalElevation( const bool ellipsoidalElevation )
{
  if ( mEllipsoidalElevation == ellipsoidalElevation )
    return;

  mEllipsoidalElevation = ellipsoidalElevation;
}

void NmeaReceiver::stateChanged( const QgsGpsInformation &info )
{
  qInfo() << " state changed " << info.isValid();
  if ( mLastGnssPositionValid && std::isnan( info.latitude ) )
  {
    //we already sent a valid position, stick to last valid position
    return;
  }
  mLastGnssPositionValid = !std::isnan( info.latitude );

  // QgsGpsInformation's speed is served in km/h, translate to m/s
  mLastGnssPositionInformation = GnssPositionInformation( info.latitude, info.longitude, mEllipsoidalElevation ? info.elevation + info.elevation_diff : info.elevation,
                                                          info.speed * 1000 / 60 / 60, info.direction, info.satellitesInView, info.pdop, info.hdop, info.vdop,
                                                          info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType, info.quality, info.satellitesUsed, info.status,
                                                          info.satPrn, info.satInfoComplete, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
                                                          0, QStringLiteral( "nmea" ) );
  emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
}
