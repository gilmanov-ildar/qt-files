/**
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 *
 * @file FileProviderList.cpp
 * @brief
 * @details
 * @date Created on 06.12.2016
 * @copyright 2016 DDwarf LLC, <dev@ddwarf.org>
 * @author Gilmanov Ildar <dev@ddwarf.org>
 */

#include <QDebug>
#include <QtQml>

#include "FileProviderList.h"
#include "FileProvider.h"

namespace DDwarf {
namespace Files {

QList<QMetaObject> FileProviderList::m_defaultFileProviders;

void FileProviderList::initStatic()
{
    // org.ddwarf.files

    qmlRegisterType<FileProvider>("org.ddwarf.files", 1, 0, "FileProvider");
    qmlRegisterType<FileProviderList>("org.ddwarf.files", 1, 0, "FileProviderList");
}

void FileProviderList::addDefaultFileProvider(const QMetaObject &fileProviderMetaObject)
{
    m_defaultFileProviders.push_back(fileProviderMetaObject);
}

FileProviderList::FileProviderList(QObject *parent) :
    QAbstractListModel(parent)
{
    m_roles.insert(Qt::UserRole, "modelData");
    int index = Qt::UserRole + 1;

    for(int i = 0; i < FileProvider::staticMetaObject.propertyCount(); ++i)
    {
        QMetaProperty prop = FileProvider::staticMetaObject.property(i);

        if(prop.isReadable())
        {
            m_roles.insert(index, prop.name());
            ++index;
        }
    }

    for(const QMetaObject &metaObject : m_defaultFileProviders)
    {
        QObject *instance = metaObject.newInstance(Q_ARG(QObject*, this));
        FileProvider *fileProvider = qobject_cast<FileProvider*>(instance);

        if(fileProvider)
        {
            m_list.push_back(fileProvider);
        }
        else
        {
            qWarning() << QString("Can not create new instance of '%1' file provider")
                          .arg(metaObject.className());
        }
    }
}

int FileProviderList::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
    {
        return 0;
    }

    return m_list.count();
}

QVariant FileProviderList::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    FileProvider *item = at(index.row());

    if(item)
    {
        if(role == Qt::UserRole)
        {
            return QVariant::fromValue(item);
        }
        else if(m_roles.contains(role))
        {
            int propIndex = item->metaObject()->indexOfProperty(m_roles.value(role));

            if(propIndex >= 0 && propIndex < item->metaObject()->propertyCount())
            {
                return item->metaObject()->property(propIndex).read(item);
            }
            else
            {
                qWarning().noquote() << QString("Can not find role '%1'")
                                        .arg(QString(m_roles.value(role)));
                return QVariant();
            }
        }
        else
        {
            qWarning().noquote() << QString("Can not recognize role '%1'").arg(role);
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}

QHash<int, QByteArray> FileProviderList::roleNames() const
{
    return m_roles;
}

int FileProviderList::length() const
{
    return m_list.count();
}

void FileProviderList::append(FileProvider *item)
{
    add(item);
}

void FileProviderList::add(FileProvider *item)
{
    if(!m_list.contains(item))
    {
        beginInsertRows(QModelIndex(), m_list.count(), m_list.count());

        item->setParent(this);
        m_list.push_back(item);

        endInsertRows();
        emit listChanged();
    }
}

void FileProviderList::insert(int index, FileProvider *item)
{
    if(!m_list.contains(item))
    {
        beginInsertRows(QModelIndex(), index, index);

        item->setParent(this);
        m_list.insert(index, item);

        endInsertRows();
        emit listChanged();
    }
}

void FileProviderList::remove(FileProvider *item)
{
    int index = m_list.indexOf(item);

    if(index >= 0)
    {
        removeAt(index);
    }
    else
    {
        qWarning().noquote() << "Can not find IFileProvider to remove";
    }
}

void FileProviderList::removeAt(int index)
{
    if(index >= 0 && index < m_list.count())
    {
        beginRemoveRows(QModelIndex(), index, index);

        m_list.takeAt(index)->deleteLater();

        endRemoveRows();
        emit listChanged();
    }
    else
    {
        qWarning().noquote() << QString("index %1 is out of range [0, %2]")
                                .arg(index)
                                .arg(m_list.count());
    }
}

void FileProviderList::clear()
{
    if(!m_list.isEmpty())
    {
        beginResetModel();

        while(!m_list.isEmpty())
        {
            m_list.takeFirst()->deleteLater();
        }

        emit listChanged();

        endResetModel();
    }
}

FileProvider *FileProviderList::at(int index) const
{
    if(index >= 0 && index < m_list.count())
    {
        return m_list.at(index);
    }
    else
    {
        qWarning().noquote() << QString("index %1 is out of range [0, %2]")
                                .arg(index)
                                .arg(m_list.count());

        return nullptr;
    }
}

} // namespace Files
} // namespace DDwarf
