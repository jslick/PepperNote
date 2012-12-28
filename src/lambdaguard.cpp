/*
 * Copyright (c) 2012 PepperNote developers
 *
 * See the file license.txt for copying permission.
 */

#include "lambdaguard.h"

LambdaGuard::LambdaGuard(std::function<void()> finalizer)
    : finalizer(finalizer)
{
}

LambdaGuard::~LambdaGuard()
{
    this->doNow();
}

void LambdaGuard::doNow()
{
    if (this->finalizer)
    {
        try
        {
            this->finalizer();
        }
        catch (...)
        {
            // The finalizer should not throw an exception.  If you get here,
            // fix your finalizer!
        }
    }
    this->finalizer = 0;
}
