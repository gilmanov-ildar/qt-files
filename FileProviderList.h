/**
 * @file FileProviderList.h
 * @brief
 * @details
 * @date Created on 06.12.2016
 * @copyright 2016 DDwarf LLC, <dev@ddwarf.org>
 * @author Gilmanov Ildar <dev@ddwarf.org>
 */

#ifndef DDWARF_FILES_FILEPROVIDERLIST_H
#define DDWARF_FILES_FILEPROVIDERLIST_H

#include <QObject>
#include <QAbstractListModel>

class QQmlEngine;
class QJSEngine;

namespace DDwarf {
namespace Files {

class FileProvider;

class FileProviderList : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int length READ length NOTIFY listChanged)

public:
    static void initStatic();
    static void addDefaultFileProvider(const QMetaObject &fileProviderMetaObject);

    explicit FileProviderList(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int length() const;

    /// FileProviderList takes ownership of the item
    void append(FileProvider *item);
    void add(FileProvider *item);

    /// FileProviderList takes ownership of the item
    void insert(int index, FileProvider *item);

    /// FileProviderList deletes the item
    void remove(FileProvider *item);

    /// FileProviderList deletes the item
    void removeAt(int index);

    /// FileProviderList deletes all items
    void clear();

    Q_INVOKABLE DDwarf::Files::FileProvider *at(int index) const;

public slots:

signals:
    void listChanged();

protected:

private:
    static QList<QMetaObject> m_defaultFileProviders;

    QHash<int, QByteArray> m_roles;
    QList<FileProvider*> m_list;
};

} // namespace Files
} // namespace DDwarf

#endif // DDWARF_FILES_FILEPROVIDERLIST_H
