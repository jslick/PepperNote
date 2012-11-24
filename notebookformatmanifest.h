#ifndef NOTEBOOKFORMATMANIFEST_H
#define NOTEBOOKFORMATMANIFEST_H

#include <QObject>
#include <QList>
#include <QHash>

class NotebookFormatManifest : public QObject
{
    Q_OBJECT
public:
    explicit NotebookFormatManifest(QObject* parent = 0);

    void parseJson(const QString& manifestJson);

    QString serialize() const;

    QString getPageId(int sectionIndex, int pageIndex) const;

    bool containsPage(const QString& pageId) const;

    void addPage(const QString& sectionName, const QString& pageId);

signals:

public slots:

private:
    typedef struct {
        QString     id;
        QString     name;
    } Page;

    typedef struct {
        QString         name;
        QList<Page*>    pages;
    } Section;

    Section& findOrCreateSection(const QString& sectionName);

    QList<Page>             pages;
    QList<Section>          sections;
    QHash<QString,Section*> sectionIndex;   // section name <-> struct Section
    QHash<QString,Page*>    pageIndex;      // page id <-> struct Page
};

#endif // NOTEBOOKFORMATMANIFEST_H
