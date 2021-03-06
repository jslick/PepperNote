/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

#ifndef NOTEBOOKFORMAT_H
#define NOTEBOOKFORMAT_H

#include <QObject>

class Notebook;
class NotebookPage;

/**
 * @brief Interface for different Notebook file formats
 *
 * File formats subclass this interface base class to define their own file
 * formats.
 */
class NotebookFormat : public QObject
{
    Q_OBJECT
public:
    /**
     * @param parent    The notebook that this format will save.  Since there
     *                  exists a circular dependency between Notebook and
     *                  NotebookFormat, this will likely be null.
     *
     * @note If the parent notebook is null, it must be set before saving or
     *       retrieving using setNotebook.
     */
    explicit NotebookFormat(Notebook* parent = 0);

    /**
     * @brief Set the Notebook
     *
     * @param notebook  The notebook that this format will save or retrieve
     */
    void setNotebook(Notebook& notebook);

    // Interface methods:

    /**
     * @brief Load the Notebook meta information from the disk
     *
     * This is called before any pages are retrieved or saved so that the file
     * format definition has a chance to load meta data, if it needs to.
     *
     * @note The parent Notebook should be set before this function is called.
     */
    virtual void load() = 0;

    /**
     * @brief Checks to see if the given page is committed
     *
     * @param pageId    The UUID of the page
     *
     * @return True if the page is persisted to the disk, or is committed to be
     *         saved to the disk.  False for new pages.
     */
    virtual bool isPagePersisted(const QString& pageId) const = 0;

    /**
     * @brief Get the HTML content of the given page
     *
     * @param pageId    The UUID of the page
     *
     * @return The HTML contents of the page
     */
    virtual QString getPageContents(const QString& pageId) const = 0;

    /**
     * @brief Save a Notebook page to disk
     *
     * @param notebook  The Notebook that the page belongs to
     * @param page      The page to save
     * @param html      The HTML content of the page to save
     *
     * @note The page can be new or persisted.
     */
    virtual void savePage(Notebook& notebook, NotebookPage& page, const QString& html) = 0;

signals:

public slots:

protected:
    Notebook* notebook;
};

#endif // NOTEBOOKFORMAT_H
