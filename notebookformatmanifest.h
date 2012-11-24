#ifndef NOTEBOOKFORMATMANIFEST_H
#define NOTEBOOKFORMATMANIFEST_H

#include <QObject>
#include <QMultiMap>

class NotebookFormatManifest : public QObject
{
    Q_OBJECT
public:
    explicit NotebookFormatManifest(QObject* parent = 0);

    QString getPageId(int sectionIndex, int pageIndex) const;

    bool containsPage(const QString& pageId) const;

    void addPage(const QString& sectionName, const QString& pageId);

    QString serialize() const;

signals:

public slots:

private:
    QMultiMap<QString,QString> pages;   // section name <-> page id's
    // TODO:  Change data structure for pages.  QMultiMap orders by key instead
    //        of user-defined order.
};

#endif // NOTEBOOKFORMATMANIFEST_H
