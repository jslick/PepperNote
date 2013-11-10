/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEBOOKPAGE_H
#define NOTEBOOKPAGE_H

#include <QObject>

class Notebook;

class NotebookPage : public QObject
{
    Q_OBJECT
public:
    explicit NotebookPage(const QString& pageId = "", const QString& pageName = "");

    QString getId() const;

    QString createId();

    QString getName() const;

    /**
     * @param pageName
     * @note Emits pageNameChanged
     */
    void setName(const QString& pageName);

signals:

    /**
     * @brief Emitted when the pageName changes, but not when the object is
     *        created
     *
     * @param pageName  The new pageName
     */
    void pageNameChanged(QString pageName);

public slots:

private:
    QString pageId;
    QString pageName;
};

#endif // NOTEBOOKPAGE_H
