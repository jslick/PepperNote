/*
 * Copyright (c) 2012 PepperNote developers
 *
 * See the file license.txt for copying permission.
 */

var ENABLE_FIREBUG = false;

/**
 * Notifies the Qt app that selection changed
 */
function notifySelectionChange() {
    // Put function body in setTimeout so that the selection changes before
    // getting the computed style
    setTimeout(function() {
        var computed = getSelectionStyle();
        if (computed) {
            var fontFamily = computed['font-family'];
            var fontSize = getComputedFontPointSize(computed);
            Api.selectionChanged(fontFamily, fontSize);
        }
    }, 0);
}

/**
 * Notifies the Qt app that the #page_title changed
 */
function notifyTitleChange() {
    var pageTitleElement = $('page_title');
    var pageTitle = pageTitleElement ? pageTitleElement.innerText : '';
    pageTitle = pageTitle.strip();
    if (!pageTitle)
        return;

    Api.notifyTitleChanged(pageTitle);
}

/**
 * If any part of the body is clicked outside of the editable area, the cursor
 * is placed at the end of the #page_content.
 *
 * NOTE:  For this to work, body height must be 100%.
 */
function acceptAllClicks() {
    var noteContentClicked = false;

    document.body.observe('click', function(event) {
        if (!noteContentClicked)
            putCursorAtEnd($('page_content'));
        noteContentClicked = false;
    });

    $('page_content').observe('click', function(event) {
        noteContentClicked = true;
    });
}

document.observe('dom:loaded', function() {
    if (ENABLE_FIREBUG)
        addFirebug();

    // Clicks outside the editable area are caught
    acceptAllClicks();

    // Notify the Qt app when the selection changes.  This allows the app to
    // set its controls based on the current selection (e.g. formatting
    // controls).
    $('page_content').observe('selectstart', notifySelectionChange);

    // TODO:  restore cursor position instead of putting it at the end
    putCursorAtEnd($('page_content'));
});

/**
 * Invoked by the native app when the #page_content is loaded
 */
function pageLoaded() {
    var pageTitleElement = $('page_title');
    if (pageTitleElement) {
        // Check to see if the #page_title should be updated from the default
        // page template by checking for this global variable
        if (typeof initialPageTitle !== 'undefined')
            pageTitleElement.update(initialPageTitle);

        pageTitleElement.observe('DOMCharacterDataModified', notifyTitleChange);
    }
}
