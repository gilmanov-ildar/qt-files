/**
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 *
 * @file FileProvider.cpp
 * @brief
 * @details
 * @date Created on 06.12.2016
 * @copyright 2016 DDwarf LLC, <dev@ddwarf.org>
 * @author Gilmanov Ildar <dev@ddwarf.org>
 */

#include <QDebug>
#include <QMimeType>
#include <QMimeDatabase>
#include <QFile>
#include <QBuffer>
#include <QPixmap>

#include "FileProvider.h"

namespace DDwarf {
namespace Files {

FileProvider::FileProvider(QObject *parent) :
    QObject(parent)
{
}

QUrl FileProvider::iconUrl() const
{
    return customIconUrl();
}

QString FileProvider::name() const
{
    return customName();
}

QStringList FileProvider::mimeTypes() const
{
    return customMimeTypes();
}

bool FileProvider::isReadable() const
{
    return customIsReadable();
}

bool FileProvider::isWritable() const
{
    return customIsWritable();
}

void FileProvider::getFile(const QString &mimeType)
{
    customGetFile(mimeType);
}

void FileProvider::setFile(const QString &sourceFileName, const QString &destFileName)
{
    QSharedPointer<QFile> file = QSharedPointer<QFile>::create(sourceFileName);

    if(file->open(QIODevice::ReadOnly))
    {
        setFileContent(destFileName, QVariant::fromValue<QSharedPointer<QIODevice> >(file));
    }
    else
    {
        qWarning().noquote() << QString("Could not open file '%1'. %2 (%3)")
                                .arg(sourceFileName)
                                .arg(file->errorString())
                                .arg(qobject_cast<QFile*>(file.data())->error());

        onGetFileFailed(QMimeDatabase().
                        mimeTypeForFile(sourceFileName, QMimeDatabase::MatchExtension).name(),
                        tr("Could not open file '%1'").arg(sourceFileName));
    }
}

void FileProvider::setFileContent(const QString &destFileName, const QVariant &content)
{
    if(content.canConvert<QSharedPointer<QIODevice> >())
    {
        customSetFileContent(destFileName, content.value<QSharedPointer<QIODevice> >());
    }
    else if(content.canConvert<QPixmap>())
    {
        QSharedPointer<QBuffer> buffer = QSharedPointer<QBuffer>::create();
        buffer->open(QIODevice::ReadWrite);
        content.value<QPixmap>().save(buffer.data(), "PNG");

        customSetFileContent(destFileName, buffer);
    }
    else if(content.canConvert<QByteArray>())
    {
        QSharedPointer<QBuffer> buffer = QSharedPointer<QBuffer>::create();
        buffer->setData(content.value<QByteArray>());

        customSetFileContent(destFileName, buffer);
    }
    else
    {
        qWarning().noquote()
                << QString("Could not convert file content of '%1' to supported types")
                   .arg(destFileName);

        onSetFileFailed(destFileName, tr("Could not set file"));
    }
}

QUrl FileProvider::customIconUrl() const
{
    return QUrl();
}

QString FileProvider::customName() const
{
    return QString();
}

QStringList FileProvider::customMimeTypes() const
{
    return QStringList();
}

bool FileProvider::customIsReadable() const
{
    return false;
}

bool FileProvider::customIsWritable() const
{
    return false;
}

void FileProvider::customGetFile(const QString &mimeType)
{
    onGetFileFailed(mimeType, tr("Could not get file"));
}

void FileProvider::customSetFileContent(const QString &fileName,
                                        const QSharedPointer<QIODevice> &content)
{
    Q_UNUSED(content);

    onSetFileFailed(fileName, tr("Could not set file"));
}

void FileProvider::onGetFileCanceled(const QString &mimeType)
{
    emit getFileCanceled(mimeType);
}

void FileProvider::onGetFileFailed(const QString &mimeType, const QString &errorMessage)
{
    qWarning().noquote() << QString("Could not get file with mime type '%1'. %2")
                            .arg(mimeType)
                            .arg(errorMessage);

    emit getFileFailed(mimeType, errorMessage);
}

void FileProvider::onGetFileSucceeded(const QString &mimeType,
                                      const QString &fileName,
                                      const QSharedPointer<QIODevice> &fileContent)
{
    QMimeType actualMimeType = QMimeDatabase().mimeTypeForFileNameAndData(fileName,
                                                                          fileContent.data());

    if(actualMimeType.name() == QLatin1String("application/octet-stream") && !mimeType.isEmpty())
    {
        emit getFileSucceeded(mimeType, fileName, fileContent);
    }
    else
    {
        emit getFileSucceeded(actualMimeType.name(), fileName, fileContent);
    }
}

void FileProvider::onSetFileCanceled(const QString &destFileName)
{
    emit setFileCanceled(destFileName);
}

void FileProvider::onSetFileFailed(const QString &destFileName, const QString &errorMessage)
{
    qWarning().noquote() << QString("Could not set file '%1'. %2")
                            .arg(destFileName)
                            .arg(errorMessage);

    emit setFileFailed(destFileName, errorMessage);
}

void FileProvider::onSetFileSucceeded(const QString &destFileName)
{
    emit setFileSucceeded(destFileName);
}

} // namespace Files
} // namespace DDwarf
