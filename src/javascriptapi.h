/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef JAVASCRIPTAPI_H
#define JAVASCRIPTAPI_H

#include <QObject>

/**
 * @brief Api exposed the the JavaScript environment
 *
 * Each note should have an instance of JavascriptApi.
 *
 * Q_INVOKABLE methods and public slots are methods that are available to the
 * JavaScript environment.
 */
class JavascriptApi : public QObject
{
    Q_OBJECT
public:
    explicit JavascriptApi(QObject* parent = 0);

    /**
     * @brief Notifies the Qt app that the selection changed
     *
     * @param fontFamily    The font face of the new text selection
     * @param fontSize      The font size of the new text selection
     * @note emits selectionChanged
     */
    Q_INVOKABLE void notifySelectionChanged(const QString& fontFamily, double fontSize);

    /**
     * @brief Notifies the Qt app that the page title (h1#page_title) changed
     *
     * @param pageTitle     New page title
     * @note emits titleChanged
     */
    Q_INVOKABLE void notifyTitleChanged(const QString& pageTitle);

signals:

    /**
     * @brief The document selection changed
     * @param fontFamily    The font face of the new text selection
     * @param fontSize      The font size of the new text selection
     */
    void selectionChanged(const QString& fontFamily, double fontSize);

    void titleChanged(const QString& pageTitle);

public slots:

};

#endif // JAVASCRIPTAPI_H
