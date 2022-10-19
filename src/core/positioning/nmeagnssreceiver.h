/***************************************************************************
 bluetoothreceiver.h - BluetoothReceiver

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
#ifndef NMEARECEIVER_H
#define NMEARECEIVER_H

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"
#include "qgsnmeaconnection.h"

#include <QObject>

/**
 * The nmeareceiver connects to a device and feeds the QgsNmeaConnection.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 */
class NmeaReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit NmeaReceiver( QObject *parent = nullptr );
    ~NmeaReceiver() override = default;

    void initNmeaConnection( QIODevice *ioDevice );

    /**
     * Sets whether the elevation value provided will be ellipsoidal or, if not, orthometric
     */
    void setEllipsoidalElevation( const bool ellipsoidalElevation );

    /**
     * Returns whether the elevation value provided will be ellipsoidal or orthometric
     */
    bool ellipsoidalElevation() const { return mEllipsoidalElevation; }

  protected:
    std::unique_ptr<QgsNmeaConnection> mGpsConnection;

    bool mEllipsoidalElevation = true;
};

#endif // BLUETOOTHRECEIVER_H
