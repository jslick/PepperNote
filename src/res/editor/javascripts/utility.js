/*
 * Copyright (c) 2012 PepperNote developers
 * 
 * See the file license.txt for copying permission.
 */

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

/**
 * @brief Recursively finds all text nodes in a container
 *
 * @param container     An Element, not a text node
 *
 * @return Array of text nodes
 */
function getTextNodes(container) {
    if (container.nodeType === 3)
        return [container];

    var textNodes = [];

    var _getTextNodes = function(element) {
        for (var i = 0; i < element.childNodes.length; i++) {
            var node = element.childNodes[i];
            if (node.nodeType === 3)
                textNodes.push(node);
            else
                _getTextNodes(node);
        }
    }
    _getTextNodes(container);

    return textNodes;
}

// TODO:  Unit test
/**
 * @brief Format Range with styles hash
 *
 * @param range     Range to format
 * @param styles    Hash of CSS styles to format the range with
 *
 * @return true if something changed, otherwise falsey
 */
function formatRange(range, styles) {
    // Not the best algorithm - it creates unnecessary messy HTML

    var changed = false;

    var styleElement = new Element('span');
    styleElement.setStyle(styles);

    var container = range.commonAncestorContainer;
    var textNodes = getTextNodes(container);

    // Prototype doesn't extend text nodes, so this must use the Element.method
    // methods.
    textNodes.each(function(node) {
        var nodeRange = document.createRange();
        nodeRange.selectNodeContents(node);

        // If node comes after range, don't format it.
        if (nodeRange.compareBoundaryPoints(Range.START_TO_END, range) < 0)
            return;
        // If node comes before range, don't format it.
        if (nodeRange.compareBoundaryPoints(Range.END_TO_START, range) > 0)
            return;

        var toFormatRange = range.cloneRange();

        var rangeAtLeftEnd = nodeRange.compareBoundaryPoints(Range.START_TO_START, range) > 0;
        var rangeAtRightEnd = nodeRange.compareBoundaryPoints(Range.END_TO_END, range) < 0;

        if (rangeAtLeftEnd && rangeAtRightEnd &&
        Element.up(node).innerText === node.nodeValue) {
            // Tidyness optimization:  Catches few cases where the text node is
            // fully contained in the selection.  The node's style is set
            // instead of creating a new span.
            Element.up(node).setStyle(styles);
        }
        else {
            // If the range continues into this node, move the toFormatRange
            // startpoint back to the node
            if (nodeRange.compareBoundaryPoints(Range.START_TO_START, range) > 0)
                toFormatRange.setStartBefore(node);
            // If the range continues past the end of the node, move the
            // toFormatRange endpoint back to the node
            if (nodeRange.compareBoundaryPoints(Range.END_TO_END, range) < 0)
                toFormatRange.setEndAfter(node);

            toFormatRange.surroundContents(styleElement.clone(true));
        }

        changed = true;
    });

    return changed;
}

/**
 * @brief Gets the computed style of the current selection
 *
 * @return CSSStyleDeclaration of the current selection
 */
function getSelectionStyle() {
    var sel = window.getSelection();
    if (sel.rangeCount) {
        var container = sel.getRangeAt(0).commonAncestorContainer;
        if (container.nodeType === 3)
            container = container.parentNode;

        var computed = document.defaultView.getComputedStyle(container, null);
        return computed;
    }

    return null;
}

/**
 * @brief Gets the computed font size in points of the CSSStyleDeclaration
 *
 * @param computed  A CSSStyleDeclaration
 *
 * @return The font size, in points, as a floating point number
 *
 * @note This function may not work on all browsers.
 */
function getComputedFontPointSize(computed) {
    var fontSize = computed.getPropertyCSSValue('font-size');
    var value = fontSize.getFloatValue(CSSPrimitiveValue.CSS_PT);
    return value;
}

/**
 * @brief Sets the font of the current selection
 *
 * @param fontSize  Font size, in points
 *
 * @return true if something changed, otherwise falsey
 */
function setSelectionFontSize(fontSize) {
    var sel = window.getSelection();
    if (sel.rangeCount) {
        var selectedRange = sel.getRangeAt(0);
        var changed = formatRange(selectedRange, {
            fontSize:  '' + fontSize + 'pt'
        });

        return changed;
    }
    else
        return false;
}

/**
 * @brief Sets focus to an element
 *
 * @param element   The element to set focus
 * @param focusPosition
 *                  If 1, the cursor will be placed at the end of the element
 * @param withTimeout
 *                  If true, execution will be defered
 * @param callback  Function to invoke after setting the focus
 */
function setFocus(element, focusPosition, withTimeout, callback) {
    element = $(element);
    var id = element.id;

    var _setFocus = function() {
        element = $(id);
        element.focus();

        if (focusPosition === 1)
            putCursorAtEnd(element);

        if (callback)
            callback();
    }

    if (withTimeout)
        setTimeout(_setFocus, 0);
    else
        _setFocus();
}

/**
 * @brief Places the cursor at the end of an element
 *
 * @param element
 */
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
