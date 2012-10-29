#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QObject>

class NotebookPage;

class Notebook : public QObject
{
    Q_OBJECT
public:
    static const QString FILE_EXTENSION;

    explicit Notebook(QObject* parent = 0);

    NotebookPage* getFirstPage();

signals:

public slots:

};

#endif // NOTEBOOK_H
