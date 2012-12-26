#ifndef NOTEBOOKTREE_H
#define NOTEBOOKTREE_H

#include <QTreeWidget>

#include <QHash>

class Notebook;
class NotebookPage;
class TreeNotebookItem;

class NotebookTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit NotebookTree(QWidget* parent = 0);

    void addNotebook(Notebook& notebook);

    void addPageItem(Notebook& notebook, const QString& sectionName, NotebookPage& page);

signals:

public slots:

    void selectPage(Notebook* notebook, NotebookPage* page);

private:
    QHash<Notebook*,TreeNotebookItem*> notebookTrees;
};

#endif // NOTEBOOKTREE_H
