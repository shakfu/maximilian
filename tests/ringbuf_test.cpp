#include "maximilian.h"
#include "check.h"

int main() {
    maxiRingBuf rb;
    rb.setup(10);
    CHECK(rb.size() == 10);

    // Push 0..14 so the write index wraps once.
    for (int i = 0; i < 15; ++i) rb.push(i);
    CHECK(rb.head() == 14);
    CHECK(rb.tail(3) == 12);
    CHECK(rb.tail(10) == 5);

    auto sum = [](double acc, double x) { return acc + x; };
    CHECK(rb.reduce(3, sum, 0) == 12 + 13 + 14);
    // The window spans the wrap point: 6..9 then 10..14.
    CHECK(rb.reduce(9, sum, 0) == 6 + 7 + 8 + 9 + 10 + 11 + 12 + 13 + 14);
    CHECK(rb.reduce(3, sum, 100) == 100 + 12 + 13 + 14);

    return checkResult();
}
