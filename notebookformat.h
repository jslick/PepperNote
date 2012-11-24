#ifndef NOTEBOOKFORMAT_H
#define NOTEBOOKFORMAT_H

#include <QObject>

class Notebook;
class NotebookPage;

class NotebookFormat : public QObject
{
    Q_OBJECT
public:
    explicit NotebookFormat(Notebook* parent = 0);

    void setNotebook(Notebook* notebook);

    virtual void load() = 0;

    virtual bool isPagePersisted(const QString& pageId) const = 0;

    virtual QString getPageContents(const QString& pageId) const = 0;

    virtual QString getPageId(int sectionIndex, int pageIndex) const = 0;
    // Can overload to make first parameter section name

    virtual void savePage(NotebookPage& page, const QString& html) = 0;

signals:

public slots:

protected:
    Notebook* notebook;
};

#endif // NOTEBOOKFORMAT_H
