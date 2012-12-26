/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <QFile>
#include <QMessageBox>

/**
 * @brief Get contents of file
 *
 * @param fileName
 *
 * @return Byte array of file
 */
inline QByteArray getFileBytes(const QString& fileName)
{
    QFile file(fileName);
    bool success = file.open(QIODevice::ReadOnly);
    if (!success)
        return "";  // TODO:  Throw exception

    QByteArray contents = file.readAll();

    file.close();
    return contents;
}

/**
 * @brief Get the contents of the UTF-8 encoded file
 *
 * @param fileName
 *
 * @return String of text file
 */
inline QString getFileUtf8(const QString& fileName)
{
    return QString::fromUtf8(getFileBytes(fileName).constData());
}

/**
 * @brief Show message box
 *
 * @param messageContents   Caller is responsible for translating messageContents
 */
inline void showMessage(const QString& messageContents)
{
    QMessageBox msgBox;
    msgBox.setText(messageContents);
    msgBox.exec();
}

/**
 * @brief If the pointer is null, show a message box and return from the void
 *        function.
 */
#define CHECK_POINTER_GUI(pointer,message)  \
    do {                                    \
        if (!pointer) {                     \
            showMessage(message);           \
            return;                         \
        }                                   \
    } while (0);

#endif // UTILITY_H
