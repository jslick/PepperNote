#include "javascriptapi.h"

JavascriptApi::JavascriptApi(QObject* parent) :
    QObject(parent)
{
}

void JavascriptApi::notifySelectionChanged(const QString& fontFamily, double fontSize)
{
    emit selectionChanged(fontFamily, fontSize);
}
