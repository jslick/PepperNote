#ifndef NOTEBOOKEXCEPTION_H
#define NOTEBOOKEXCEPTION_H

#include <QtCore>

class NotebookException : public QtConcurrent::Exception
{
public:
    NotebookException(const QString& message)
        : QtConcurrent::Exception(),
          message(message)
    {}

    virtual ~NotebookException() throw() {}

    void raise() const { throw *this; }
    Exception* clone() const { return new NotebookException(*this); }

    QString message;
};

#endif // NOTEBOOKEXCEPTION_H
