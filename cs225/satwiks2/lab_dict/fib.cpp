/**
 * @file fib.cpp
 * Contains Fibonacci-number generating functions both utilizing and not
 * utilizing memoization.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "fib.h"

#include <map>
#include <unordered_map>

using std::map;

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
unsigned long fib(unsigned long n)
{
    /* Your code goes here! */
    if (n == 0 || n == 1)
        return n;

    return fib(n-1) + fib(n-2);
}

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0. This version utilizes memoization.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
#include <iostream>
static std::unordered_map<unsigned long, unsigned long> m = { {0, 0}, {1, 1} };
unsigned long memoized_fib(unsigned long n)
{
    /* Your code goes here! */
    if (m.count(n))
        return m[n];
    unsigned long res = memoized_fib(n-1) + memoized_fib(n-2);
    if (!m.count(n))
        m[n] = res;
    return res;
}
