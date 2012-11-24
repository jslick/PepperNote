#ifndef NOTEBOOKMANAGER_H
#define NOTEBOOKMANAGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>

class Notebook;
class NotebookFormat;

class NotebookManager : public QObject
{
    Q_OBJECT
public:
    static NotebookManager instance;

    explicit NotebookManager(QObject* parent = 0);

    void loadNotebook(const QString& filename);

    void createNotebook(NotebookFormat* fileFormat);

signals:

    void notebookLoaded(Notebook* notebook);

public slots:

private:
    QQueue<QString> loadQueue;
    QMutex          loadQueueMutex;

};

#endif // NOTEBOOKMANAGER_H
