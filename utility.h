#ifndef UTILITY_H
#define UTILITY_H

#include <QFile>
#include <QMessageBox>

inline QByteArray getFileBytes(const QString& resourceName)
{
    QFile file(resourceName);
    bool success = file.open(QIODevice::ReadOnly);
    if (!success)
        return "";  // TODO:  Throw exception

    QByteArray contents = file.readAll();

    file.close();
    return contents;
}

inline QString getFileUtf8(const QString& fileName)
{
    return QString::fromUtf8(getFileBytes(fileName).constData());
}

// caller responsible for translating messageContents
inline void showMessage(const QString& messageContents)
{
    QMessageBox msgBox;
    msgBox.setText(messageContents);
    msgBox.exec();
}

#define CHECK_POINTER_GUI(pointer,message)  \
    do {                                    \
        if (!pointer) {                     \
            showMessage(message);           \
            return;                         \
        }                                   \
    } while (0);

#endif // UTILITY_H
