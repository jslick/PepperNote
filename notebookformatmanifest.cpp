#include "notebookformatmanifest.h"

#include <QStringList>

NotebookFormatManifest::NotebookFormatManifest(QObject* parent) :
    QObject(parent)
{
}

#define DEFAULT_PAGE "4df45175-af0d-4caf-b682-1ecdbfb6087a"
QString NotebookFormatManifest::getPageId(int sectionIndex, int pageIndex) const
{
    // TODO:  Load pages from manifest from loaded file
    Q_UNUSED(sectionIndex);
    Q_UNUSED(pageIndex);

    return DEFAULT_PAGE;
}

bool NotebookFormatManifest::containsPage(const QString& pageId) const
{
    return this->pages.values().contains(pageId);
}

void NotebookFormatManifest::addPage(const QString& sectionName, const QString& pageId)
{
    this->pages.insert(sectionName, pageId);
}

QString NotebookFormatManifest::serialize() const
{
    QString json = QString("{%1\n}\n")
                   .arg("\n\t\"sections\":\n\t[%1\n\t]")
                   .arg("\n\t\t{\n\t\t\t\"name\": \"General\",\n\t\t\t\"pages\":\n\t\t\t[%1\n\t\t\t]\n\t\t}")
                   .arg([this]()
    {
        QStringList subjsons;
        for (const QString& pageId : this->pages.values())
        {
            QString subjson = QString("\n\t\t\t\t{\n\t\t\t\t\t%1\n\t\t\t\t}")
                              .arg("\"id\": \"%1\",\n\t\t\t\t\t\"name\": \"Note\"")
                              .arg(pageId)
                              ;
            subjsons.append(subjson);
        }
        return subjsons.join(",");
    }()
                       )    // .arg
                   ;

    return json.replace(QString("\t"), QString("    "));
}
