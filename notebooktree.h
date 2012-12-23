#ifndef NOTEBOOKTREE_H
#define NOTEBOOKTREE_H

#include <QTreeWidget>

class Notebook;

class NotebookTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit NotebookTree(QWidget* parent = 0);

    void addNotebook(Notebook& notebook);

signals:

public slots:

};

#endif // NOTEBOOKTREE_H
