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
 * If any part of the body is clicked outside of the editable area, the cursor
 * is placed at the end of the #note_content.
 *
 * NOTE:  For this to work, body height must be 100%.
 */
function acceptAllClicks() {
    var noteContentClicked = false;

    document.body.observe('click', function(event) {
        if (!noteContentClicked)
            putCursorAtEnd($('note_content'));
        noteContentClicked = false;
    });

    $('note_content').observe('click', function(event) {
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
    $('note_content').observe('selectstart', notifySelectionChange);

    // TODO:  restore cursor position instead of putting it at the end
    putCursorAtEnd($('note_content'));
});
