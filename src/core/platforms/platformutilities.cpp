/***************************************************************************
                            platformutilities.cpp  -  utilities for qfield

                              -------------------
              begin                : Wed Dec 04 10:48:28 CET 2015
              copyright            : (C) 2015 by Marco Bernasocchi
              email                : marco@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "picturesource.h"
#include "platformutilities.h"
#include "projectsource.h"
#include "qfield.h"
#include "qgismobileapp.h"
#include "qgsmessagelog.h"

#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTimer>
#include <QUrl>

#if WITH_SENTRY
#include <sentry.h>
#endif

#if defined( Q_OS_ANDROID )
#include "androidplatformutilities.h"
Q_GLOBAL_STATIC( AndroidPlatformUtilities, sPlatformUtils )
#elif defined( Q_OS_IOS )
#include "ios/iosplatformutilities.h"
Q_GLOBAL_STATIC( IosPlatformUtilities, sPlatformUtils )
#else
Q_GLOBAL_STATIC( PlatformUtilities, sPlatformUtils )
#endif

PlatformUtilities::~PlatformUtilities()
{
}

PlatformUtilities::Capabilities PlatformUtilities::capabilities() const
{
  PlatformUtilities::Capabilities capabilities = NoCapabilities;
#if WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void PlatformUtilities::initSystem()
{
}

QString PlatformUtilities::systemGenericDataLocation() const
{
  return QStandardPaths::standardLocations( QStandardPaths::GenericDataLocation ).first();
}

QString PlatformUtilities::qgsProject() const
{
  return QString();
}

QStringList PlatformUtilities::qfieldAppDataDirs() const
{
  return QStringList() << QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).first() + QStringLiteral( "/QField/" );
}

QStringList PlatformUtilities::availableGrids() const
{
  QStringList dataDirs = qfieldAppDataDirs();
  QStringList grids;
  for ( const QString &dataDir : dataDirs )
  {
    QDir gridsDir( dataDir + "proj/" );
    if ( gridsDir.exists() )
    {
      grids << gridsDir.entryList( QStringList() << QStringLiteral( "*.tif" ) << QStringLiteral( "*.gtx" ) << QStringLiteral( "*.gsb" ) << QStringLiteral( "*.byn" ) );
    }
  }
  return grids;
}

bool PlatformUtilities::createDir( const QString &path, const QString &dirname ) const
{
  QDir parentDir( path );
  return parentDir.mkdir( dirname );
}

bool PlatformUtilities::rmFile( const QString &filename ) const
{
  QFile file( filename );
  return file.remove( filename );
}

bool PlatformUtilities::renameFile( const QString &filename, const QString &newname ) const
{
  QFile file( filename );
  return file.rename( newname );
}

QString PlatformUtilities::applicationDirectory() const
{
  return QString();
}

QStringList PlatformUtilities::additionalApplicationDirectories() const
{
  return QStringList() << QString();
}

QStringList PlatformUtilities::rootDirectories() const
{
  return QStringList() << QString();
}

PictureSource *PlatformUtilities::getCameraPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath, const QString &suffix )
{
  Q_UNUSED( parent )
  Q_UNUSED( prefix )
  Q_UNUSED( pictureFilePath )
  Q_UNUSED( suffix )
  return nullptr;
}

PictureSource *PlatformUtilities::getGalleryPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath )
{
  Q_UNUSED( parent )
  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select Media File" ), prefix, tr( "JPEG images (*.jpg *.jpeg)" ) );

  if ( QFileInfo::exists( fileName ) )
  {
    // if the file is already in the prefixed path, no need to copy
    if ( fileName.startsWith( prefix ) )
    {
      return new PictureSource( nullptr, prefix, fileName );
    }
    else
    {
      QString destinationFile = prefix + pictureFilePath;
      QFileInfo destinationInfo( destinationFile );
      QDir prefixDir( prefix );
      if ( prefixDir.mkpath( destinationInfo.absolutePath() ) && QFile::copy( fileName, destinationFile ) )
      {
        return new PictureSource( nullptr, prefix, destinationFile );
      }
    }

    QgsMessageLog::logMessage( tr( "Failed to save gallery picture" ), "QField", Qgis::Critical );
  }

  return new PictureSource( nullptr, prefix, QString() );
}

ViewStatus *PlatformUtilities::open( const QString &uri, bool )
{
  QDesktopServices::openUrl( QStringLiteral( "file://%1" ).arg( uri ) );
  return nullptr;
}

ProjectSource *PlatformUtilities::openProject( QObject *parent )
{
  Q_UNUSED( parent );
  QSettings settings;
  ProjectSource *source = new ProjectSource();
  QString fileName { QFileDialog::getOpenFileName( nullptr,
                                                   tr( "Open File" ),
                                                   settings.value( QStringLiteral( "QField/lastOpenDir" ), QString() ).toString(),
                                                   QStringLiteral( "%1 (*.%2);;%3 (*.%4);;%5 (*.%6);;%7 (*.%8)" ).arg( tr( "All Supported Files" ), ( SUPPORTED_PROJECT_EXTENSIONS + SUPPORTED_VECTOR_EXTENSIONS + SUPPORTED_RASTER_EXTENSIONS ).join( QStringLiteral( " *." ) ), tr( "QGIS Project Files" ), SUPPORTED_PROJECT_EXTENSIONS.join( QStringLiteral( " *." ) ), tr( "Vector Datasets" ), SUPPORTED_VECTOR_EXTENSIONS.join( QStringLiteral( " *." ) ), tr( "Raster Datasets" ), SUPPORTED_RASTER_EXTENSIONS.join( QStringLiteral( " *." ) ) ) ) };
  if ( !fileName.isEmpty() )
  {
    settings.setValue( QStringLiteral( "/QField/lastOpenDir" ), QFileInfo( fileName ).absolutePath() );
    QTimer::singleShot( 0, this, [source, fileName]() { emit source->projectOpened( fileName ); } );
  }
  return source;
}

bool PlatformUtilities::checkPositioningPermissions() const
{
  return true;
}

bool PlatformUtilities::checkCameraPermissions() const
{
  return true;
}

bool PlatformUtilities::checkWriteExternalStoragePermissions() const
{
  return true;
}

void PlatformUtilities::initiateSentry()
{
#if WITH_SENTRY
  sentry_options_t *options = sentry_options_new();
  sentry_options_set_dsn( options, qfield::sentryDsn.toUtf8().constData() );
  sentry_options_set_environment( options, qfield::sentryEnv.toUtf8().constData() );
  sentry_options_set_debug( options, 1 );
  sentry_init( options );
#endif
}

void PlatformUtilities::closeSentry()
{
#if WITH_SENTRY
  sentry_close();
#endif
}

void PlatformUtilities::copyTextToClipboard( const QString &string ) const
{
  QGuiApplication::clipboard()->setText( string );
}

QString PlatformUtilities::getTextFromClipboard() const
{
  return QGuiApplication::clipboard()->text();
}

PlatformUtilities *PlatformUtilities::instance()
{
  return sPlatformUtils;
}
