#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QObject>
#include <QtCore>
#include <QHash>

class NotebookFormat;
class NotebookPage;

class NotebookException : public QtConcurrent::Exception
{
public:
    NotebookException(const QString& message)
        : QtConcurrent::Exception(),
          message(message)
    {}

    virtual ~NotebookException() throw() {}

    void raise() const { throw *this; }
    Exception* clone() const { return new NotebookException(*this); }

    QString message;
};

class Notebook : public QObject
{
    Q_OBJECT
public:
    explicit Notebook(NotebookFormat* fileFormat, QObject* parent = 0);

    ~Notebook();

    bool isPagePersisted(const QString& pageId) const;

    // Not yet thread-safe
    NotebookPage* getFirstPage();

    QString getPageContents(const QString& pageId) const;

    void addPage(const QString& sectionName, NotebookPage* page);

    void savePage(NotebookPage& page, const QString& html);

signals:

public slots:

private:
    NotebookFormat* fileFormat;

    QHash<int,NotebookPage*> loadedPagesByIndex;    // page index <-> loaded page
};

#endif // NOTEBOOK_H
