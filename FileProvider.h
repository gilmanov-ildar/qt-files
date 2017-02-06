/**
 * @file FileProvider.h
 * @brief
 * @details
 * @date Created on 06.12.2016
 * @copyright 2016 DDwarf LLC, <dev@ddwarf.org>
 * @author Gilmanov Ildar <dev@ddwarf.org>
 */

#ifndef DDWARF_FILES_FILEPROVIDER_H
#define DDWARF_FILES_FILEPROVIDER_H

#include <QObject>
#include <QUrl>
#include <QIODevice>
#include <QMimeType>

namespace DDwarf {
namespace Files {

/**
 * @brief The FileProvider class is the base class for other file providers,
 * such as DropboxFileProvider, FacebookFileProvider and etc.
 * @details The default constructor with one argument (QObject *parent) must be marked by Q_INVOKABLE
 */
class FileProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl iconUrl READ iconUrl NOTIFY iconUrlChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QStringList mimeTypes READ mimeTypes NOTIFY mimeTypesChanged)

    Q_PROPERTY(bool isReadable READ isReadable NOTIFY isReadableChanged)
    Q_PROPERTY(bool isWritable READ isWritable NOTIFY isWritableChanged)

    Q_PROPERTY(QUrl getFileItemUrl READ getFileItemUrl NOTIFY getFileItemUrlChanged)
    Q_PROPERTY(QUrl setFileItemUrl READ setFileItemUrl NOTIFY setFileItemUrlChanged)

public:
    explicit FileProvider(QObject *parent = 0);

    /// Url to icon, svg format is preferable
    QUrl iconUrl() const;

    /// Name of the file provider, for example: "Dropbox", "Facebook"
    QString name() const;

    /// List of mime types, for example: image/*, text/plain and etc.
    QStringList mimeTypes() const;

    bool isReadable() const;
    bool isWritable() const;

    /**
     * @brief getFileItemUrl Url of the QML Item to get file, may be empty
     * @details If exists then the Item must have fileProvider and mimeType properties
     * @return Url for the QML Item
     */
    QUrl getFileItemUrl() const;

    /**
     * @brief setFileItemUrl Url of the QML Item to set file, may be empty
     * @details If exists then the Item must have fileProvider,
     * sourceFileName, destFileName and content properties
     * @return Url for the QML Item
     */
    QUrl setFileItemUrl() const;

public slots:
    /// Open appropriate file dialog to user, and user may choose a file
    void getFile(const QStringList &mimeTypes);

    /// Read the file from sourceFileName and call setFileContent()
    void setFile(const QString &sourceFileName, const QString &destFileName);

    /**
     * @brief Open appropriate file dialog to user,
     * and user may save a file with the content and suggested @a destFileName
     * @param destFileName Suggested file name
     * @param content May be QByteArray, QPixmap or QSharedPointer<QIODevice>
     */
    void setFileContent(const QString &destFileName, const QVariant &fileContent);

signals:
    void iconUrlChanged();
    void nameChanged();
    void mimeTypesChanged();

    void isReadableChanged();
    void isWritableChanged();

    void getFileItemUrlChanged();
    void setFileItemUrlChanged();

    /**
     * @brief getFileCanceled This signal is emitted when user cancel the operaition
     * @details Do not emit this signal from inherited class, use onGetFileCanceled() instead
     * @param mimeTypes Mime types from getFile()
     */
    void getFileCanceled(QStringList mimeTypes);

    /**
     * @brief getFileFailed This signal is emitted when a file can not be fetched
     * @details Do not emit this signal from inherited class, use onGetFileFailed() instead
     * @param mimeTypes Mime types from getFile()
     * @param errorMessage Translated message to show user
     */
    void getFileFailed(QStringList mimeTypes, QString errorMessage);

    /**
     * @brief getFileSucceeded This signal is emitted when file is got
     * @details Do not emit this signal from inherited class, use onGetFileSuceeded() instead
     * @param mimeType A mime type from getFile(), may be more specific
     * @param fileName Actual file name of the file
     * @param fileContent Content of the file
     */
    void getFileSucceeded(QString mimeType,
                          QString fileName,
                          QSharedPointer<QIODevice> fileContent);

    /**
     * @brief setFileCanceled This signal is emitted when user cancel the operaition
     * @details Do not emit this signal from inherited class, use onSetFileCanceled() instead
     * @param destFileName File name from setFile() or setFileContent()
     */
    void setFileCanceled(QString destFileName);

    /**
     * @brief setFileFailed This signal is emitted when a file can not be set
     * @details Do not emit this signal from inherited class, use onSetFileFailed() instead
     * @param destFileName File name from setFile() or setFileContent()
     * @param errorMessage Translated message to show user
     */
    void setFileFailed(QString destFileName, QString errorMessage);

    /**
     * @brief setFileSucceeded This signal is emitted when file is set
     * @details Do not emit this signal from inherited class, use onSetFileSuceeded() instead
     * @param destFileName Actual file name of the file
     */
    void setFileSucceeded(QString destFileName);

protected:
    static QStringList convertMimeTypeListToStringList(const QList<QMimeType> &mimeTypes);
    static QList<QMimeType> convertStringListToMimeTypeList(const QStringList &mimeTypes);

    virtual QUrl customIconUrl() const;
    virtual QString customName() const;
    virtual QStringList customMimeTypes() const;

    virtual bool customIsReadable() const;
    virtual bool customIsWritable() const;

    virtual QUrl customGetFileItemUrl() const;
    virtual QUrl customSetFileItemUrl() const;

    virtual void customGetFile(const QStringList &mimeTypes);
    virtual void customSetFileContent(const QString &fileName,
                                      const QSharedPointer<QIODevice> &content);

    void onGetFileCanceled(const QStringList &mimeTypes);
    void onGetFileFailed(const QStringList &mimeTypes, const QString &errorMessage);
    void onGetFileSucceeded(const QString &mimeType,
                            const QString &fileName,
                            const QSharedPointer<QIODevice> &fileContent);

    void onSetFileCanceled(const QString &destFileName);
    void onSetFileFailed(const QString &destFileName, const QString &errorMessage);
    void onSetFileSucceeded(const QString &destFileName);

private:

};

} // namespace Files
} // namespace DDwarf

#endif // DDWARF_FILES_FILEPROVIDER_H
