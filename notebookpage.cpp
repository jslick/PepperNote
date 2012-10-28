#include "notebookpage.h"

#include <QFile>

NotebookPage::NotebookPage(QObject* parent) :
    QObject(parent)
{
}

QString NotebookPage::getHtml()
{
    QFile noteFile(":/editor/html/new_note.html");
    noteFile.open(QIODevice::ReadOnly);
    QString noteHtml = QString::fromUtf8(noteFile.readAll().constData());
    noteFile.close();

    return noteHtml;
}
