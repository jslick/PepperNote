/*
 * Copyright (c) 2012 PepperNote developers
 *
 * See the file license.txt for copying permission.
 */

#ifndef LAMBDAGUARD_H
#define LAMBDAGUARD_H

#include <functional>

/**
 * @brief Invokes a function upon destruction
 *
 * This is for use with RAII.  This is similar to the try-catch "finally"
 * functionality of languages:  It should invoke a function even if other code
 * returns early, or throws an exception.
 *
 * Example usage:
 * @code
 * this->disableSomething();
 * LambdaGuard reenableSomething([this]{
 *     this.enableSomething();
 * });
 *
 * something_that_might_throw_exception();
 *
 * // Re-enable early
 * reenableSomething.doNow();
 *
 * @endcode
 *
 * The finalizer will only ever be invoked once, even if doNow is invoked.
 */
class LambdaGuard
{
public:
    /**
     * @brief Creates an object that will invoke its finalizer upon destruction
     *
     * @param finalizer     The function/lambda to invoke upon destruction
     * @pre The finalizer should not try to throw an exception
     */
    LambdaGuard(std::function<void()> finalizer);

    /**
     * @brief Calls the finalizer, if it has not yet been invoked
     */
    virtual ~LambdaGuard();

    /**
     * @brief Invokes the finalizer.
     *
     * If this is called, the finalizer will not be invoked upon destruction.
     */
    void doNow();

private:
    std::function<void()> finalizer;
};

#endif // LAMBDAGUARD_H
