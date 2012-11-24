#ifndef NOTEBOOKPAGE_H
#define NOTEBOOKPAGE_H

#include <QObject>

class Notebook;

class NotebookPage : public QObject
{
    Q_OBJECT
public:
    explicit NotebookPage(Notebook& parent, const QString& pageId = "");

    bool isPersisted() const;

    QString getId() const;

    QString getHtml();

    void saveContent(const QString& html);

signals:

public slots:

private:
    Notebook&   notebook;
    QString     pageId;
};

#endif // NOTEBOOKPAGE_H
