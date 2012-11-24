if (typeof console === 'undefined')
    window['console'] = {};
if (!console.log)
    console.log = function() {}

function addFirebug() {
    var firebugLite = document.createElement("script");
    firebugLite.src = "https://getfirebug.com/firebug-lite.js";
    firebugLite.id = "firebug_lite";
    firebugLite.textContent = "{ startOpened: true }";
    document.getElementsByTagName("head")[0].appendChild(firebugLite);
}

function setFocus(element, focusPosition, withTimeout) {
    element = $(element);
    var id = element.id;

    var _setFocus = function() {
        element = $(id);
        element.focus();

        if (focusPosition === 1)
            putCursorAtEnd(element);
    }

    if (withTimeout)
        setTimeout(_setFocus, 0);
    else
        _setFocus();
}

function putCursorAtEnd(element) {
    if (document.createRange) {
        var range = document.createRange();
        range.selectNodeContents(element);
        range.collapse(false);  // move range to end of element

        // Make selection the same as range
        var selection = window.getSelection();
        selection.removeAllRanges();
        selection.addRange(range);
    }
}
