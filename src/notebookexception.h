/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEBOOKEXCEPTION_H
#define NOTEBOOKEXCEPTION_H

#include <QtCore>

#if QT_VERSION >= 0x050000
typedef QException ExceptionType;
#else
typedef QtConcurrent::Exception ExceptionType;
#endif

class NotebookException : public ExceptionType
{
public:
    NotebookException(const QString& message)
        : ExceptionType(),
          message(message)
    {}

    virtual ~NotebookException() throw() {}

    void raise() const { throw *this; }
    ExceptionType* clone() const { return new NotebookException(*this); }

    QString message;
};

#endif // NOTEBOOKEXCEPTION_H
