#ifndef NOTEBOOKPAGE_H
#define NOTEBOOKPAGE_H

#include <QObject>

class NotebookPage : public QObject
{
    Q_OBJECT
public:
    explicit NotebookPage(QObject* parent = 0);

    QString getHtml();

    void saveContent(const QString& html);

signals:

public slots:

};

#endif // NOTEBOOKPAGE_H
