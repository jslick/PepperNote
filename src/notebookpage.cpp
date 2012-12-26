/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#include "notebookpage.h"
#include "utility.h"
#include "notebookexception.h"

#include <QUuid>

NotebookPage::NotebookPage(const QString& pageId, const QString& pageName) :
    QObject(),
    pageId(pageId), pageName(pageName)
{
}

QString NotebookPage::getId() const
{
    return this->pageId;
}

QString NotebookPage::createId()
{
    if (this->pageId.isEmpty() == false)
        throw NotebookException("NotebookPage ID already exists");

    return this->pageId = QUuid::createUuid().toString().mid(1, 36);
}

QString NotebookPage::getName() const
{
    return this->pageName;
}
