#ifndef TREENOTEBOOKITEM_H
#define TREENOTEBOOKITEM_H

#include <QTreeWidgetItem>

class Notebook;

class TreeNotebookItem : public QTreeWidgetItem
{
public:
    explicit TreeNotebookItem(Notebook& notebook);

signals:

public slots:

private:
    Notebook& notebook;

};

#endif // TREENOTEBOOKITEM_H
