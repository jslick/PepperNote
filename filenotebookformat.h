#ifndef FILENOTEBOOKFORMAT_H
#define FILENOTEBOOKFORMAT_H

#include "notebookformat.h"
#include "notebookformatmanifest.h"

#include <QObject>

class NotebookFormat;

/**
 * @brief A baseline NotebookFormat implementation
 *
 * This format is very simple:  It just saves the pages to separate files, and
 * maintains a simple JSON manifest file.
 *
 * The directory structure is:
 * - /
 *   - /pages/manifest.json
 *   - /pages/
 *     - /pages/page1.html
 *     - /pages/page2.html
 *
 * The manifest structure is:
 * @code
 * {
 *     "pages": [
 *         {
 *             "id": "e671d492-2b0f-4e2a-85ca-a5669d9e31f3",
 *             "name": "Note"
 *         }
 *     ],
 *     "sections": [
 *         {
 *             "name": "General",
 *             "pages": [
 *                 "e671d492-2b0f-4e2a-85ca-a5669d9e31f3"
 *             ]
 *         }
 *     ]
 * }
 * @endcode
 */
class FileNotebookFormat : public NotebookFormat
{
    Q_OBJECT
public:
    explicit FileNotebookFormat(const QString& filename, Notebook* parent = 0);

    virtual void load();

    virtual bool isPagePersisted(const QString& pageId) const;

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
