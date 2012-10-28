var ENABLE_FIREBUG = false;

document.observe('dom:loaded', function() {
    if (ENABLE_FIREBUG)
        addFirebug();

    $('note_content').focus();

    // TODO:  restore cursor position instead of putting it at the end
    putCursorAtEnd($('note_content'));
});
