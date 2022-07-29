/***************************************************************************
                            platformutilities.h  -  utilities for qfield

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

#ifndef PLATFORMUTILITIES_H
#define PLATFORMUTILITIES_H

#include "qfield_core_export.h"
#include "viewstatus.h"

#include <QObject>
#include <qgsfield.h>

class ProjectSource;
class PictureSource;

class QQuickItem;

class QFIELD_CORE_EXPORT PlatformUtilities : public QObject
{
    Q_OBJECT

    Q_PROPERTY( PlatformUtilities::Capabilities capabilities READ capabilities CONSTANT )

  public:
    enum Capability
    {
      NoCapabilities = 0,             //!< No capabilities
      NativeCamera = 1,               //!< Native camera handling support
      AdjustBrightness = 1 << 1,      //!< Capable of adjusting screen brightness
      SentryFramework = 1 << 2,       //!< Sentry framework support
      CustomLocalDataPicker = 1 << 3, //!< Custom QML local data picker support
      CustomImport = 1 << 4,          //!< Import project and dataset support
      CustomExport = 1 << 5,          //!< Export project and dataset support
      CustomSend = 1 << 6,            //!< Native send/share files support
    };
    Q_DECLARE_FLAGS( Capabilities, Capability )
    Q_FLAGS( Capabilities )

    virtual ~PlatformUtilities();

    /**
     * Returns flags containing the supported capabilities of the platform.
     */
    virtual PlatformUtilities::Capabilities capabilities() const;

    virtual void initSystem();

    /**
     * The source path to generic data location.
     * Under this path, there should be the app specific directories qgis/ proj/ qfield/ ...
     * Refers to /share or /usr/share on Linux
     */
    virtual QString systemGenericDataLocation() const;
    virtual QString qgsProject() const;
    Q_INVOKABLE virtual QStringList qfieldAppDataDirs() const;
    Q_INVOKABLE QStringList availableGrids() const;
    Q_INVOKABLE bool createDir( const QString &path, const QString &dirname ) const;
    Q_INVOKABLE bool rmFile( const QString &filename ) const;
    Q_INVOKABLE bool renameFile( const QString &filename, const QString &newname ) const;


    /**
     * The main application directory within which projects and datasets can be imported.
     */
    virtual QString applicationDirectory() const;

    /**
     * Secondary application directories which can be used by individual platforms.
     */
    virtual QStringList additionalApplicationDirectories() const;

    /**
     * A list of root directories (e.g. root of an external storage) when system-wide access is available.
     */
    virtual QStringList rootDirectories() const;

    //! Requests and imports a project folder into QField's application directory action
    Q_INVOKABLE virtual void importProjectFolder() const {};
    //! Requests and imports a project archive into QField's application directory action
    Q_INVOKABLE virtual void importProjectArchive() const {};
    //! Requests and imports one or more datasets into QField's application directory action
    Q_INVOKABLE virtual void importDatasets() const {};

    //! Exports a folder \a path to a user-specified location
    Q_INVOKABLE virtual void exportFolderTo( const QString &path ) const {};
    //! Exports a dataset \a path to a user-specified location
    Q_INVOKABLE virtual void exportDatasetTo( const QString &path ) const {};

    //! Sends a dataset \a path via the platform native API
    Q_INVOKABLE virtual void sendDatasetTo( const QString &path ) const {};
    //! Compresses a folder \a path and sends it via the platform native API
    Q_INVOKABLE virtual void sendCompressedFolderTo( const QString &path ) const {};

    //! Removes a given dataset \a path
    Q_INVOKABLE virtual void removeDataset( const QString &path ) const {};

    //! Removes a given folder \a path
    Q_INVOKABLE virtual void removeFolder( const QString &path ) const {};

    /**
     * Get a picture from camera and copy it to the requested prefix
     * @param prefix The project folder
     * @param pictureFilePath The path (including subfolders and name) of the file
     * @return The name of the picture or null
     */
    Q_INVOKABLE virtual PictureSource *getCameraPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath, const QString &suffix );

    /**
     * Get a picture from gallery and copy it to the requested prefix
     * @param prefix The project folder
     * @param pictureFilePath The path (including subfolders and name) of the file
     * @return The name of the picture or null
     */
    Q_INVOKABLE virtual PictureSource *getGalleryPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath );

    /**
     * Open the resource (file, image, ...) that is available under \a uri.
     * The mimetype is detected to indicate the system how the file should
     * be opened. An optional \a editing parameter can be set to true to indicate
     * to supported systems the resource is expected to be edited.
     */
    Q_INVOKABLE virtual ViewStatus *open( const QString &uri, bool editing = false );

    /**
     * Indicates the system that we want to open a project.
     * The system shall show a suitable user interface element (like a filebrowser)
     * to let the user select a project.
     * The call returns immediately and the returned ProjectSource will notify
     * when the project has actually been chosen.
     */
    Q_INVOKABLE virtual ProjectSource *openProject( QObject *parent = nullptr );

    /**
     * Checks for positioning (GPS etc) permissions on the device.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     * It will return true, if at least coarse permissions are granted. It will
     * ask for fine permissions if none are granted.
     */
    Q_INVOKABLE virtual bool checkPositioningPermissions() const;

    /**
     * Checks for camera permissions on the device.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     */
    Q_INVOKABLE virtual bool checkCameraPermissions() const;

    /**
     * Checks for permissions to write exeternal storage.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     */
    Q_INVOKABLE virtual bool checkWriteExternalStoragePermissions() const;

    /**
     * Sets whether the device screen is allowed to go in lock mode.
     * @param allowLock if set to FALSE, the screen will not be allowed to lock.
     */
    Q_INVOKABLE virtual void setScreenLockPermission( const bool allowLock ) { Q_UNUSED( allowLock ); }

    /**
     * Dims the brightness of the screen on supported devices.
     */
    Q_INVOKABLE virtual void dimBrightness() { return; };

    /**
     * Restores the brightness of the screen to its original value on supported devices.
     */
    Q_INVOKABLE virtual void restoreBrightness() { return; };

    /**
     * Initalizes sentry connection.
     */
    Q_INVOKABLE virtual void initiateSentry();

    /**
     * Closes active sentry connection.
     */
    Q_INVOKABLE virtual void closeSentry();

    /**
     * Copies a text \a string to the system clipboard.
     */
    Q_INVOKABLE virtual void copyTextToClipboard( const QString &string ) const;

    /**
     * Returns the current text from the system clipboard.
     */
    Q_INVOKABLE virtual QString getTextFromClipboard() const;

    static PlatformUtilities *instance();
};
#endif // PLATFORMUTILITIES_H
