/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "javascriptapi.h"

JavascriptApi::JavascriptApi(QObject* parent) :
    QObject(parent)
{
}

void JavascriptApi::notifySelectionChanged(const QString& fontFamily, double fontSize)
{
    emit selectionChanged(fontFamily, fontSize);
}

void JavascriptApi::notifyTitleChanged(const QString& pageTitle)
{
    emit titleChanged(pageTitle);
}
