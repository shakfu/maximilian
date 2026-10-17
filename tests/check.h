// Minimal assertions for CTest: a failed check prints and continues; main returns checkResult().
#pragma once
#include <cmath>
#include <iostream>

inline int checkFailures = 0;

#define CHECK(cond) \
    do { if (!(cond)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK(" #cond ") failed\n"; \
        ++checkFailures; } } while (0)

#define CHECK_NEAR(a, b, tol) \
    do { double a_ = (a), b_ = (b); if (!(std::fabs(a_ - b_) <= (tol))) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK_NEAR(" #a ", " #b ") failed: " \
                  << a_ << " vs " << b_ << " (tol " << (tol) << ")\n"; \
        ++checkFailures; } } while (0)

inline int checkResult() { return checkFailures == 0 ? 0 : 1; }
