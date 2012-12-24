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

signals:

public slots:

private:
    QString     pageId;
    QString     pageName;
};

#endif // NOTEBOOKPAGE_H
