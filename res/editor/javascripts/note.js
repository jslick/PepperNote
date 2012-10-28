var ENABLE_FIREBUG = false;

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

    $('note_content').focus();

    // TODO:  restore cursor position instead of putting it at the end
    putCursorAtEnd($('note_content'));
});
