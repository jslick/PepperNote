#ifndef FILENOTEBOOKFORMAT_H
#define FILENOTEBOOKFORMAT_H

#include "notebookformat.h"
#include "notebookformatmanifest.h"

#include <QObject>

class NotebookFormat;

class FileNotebookFormat : public NotebookFormat
{
    Q_OBJECT
public:
    explicit FileNotebookFormat(const QString& filename, Notebook* parent = 0);

    virtual QString getPageId(int sectionIndex, int pageIndex) const;

    virtual QString getPageContents(const QString& pageId) const;

    virtual void savePage(NotebookPage& page, const QString& html);

    void saveManifest();

signals:

public slots:

private:
    QString getPageFilename(const QString& pageId) const;

    QString filename;
    NotebookFormatManifest manifest;
};

#endif // FILENOTEBOOKFORMAT_H
