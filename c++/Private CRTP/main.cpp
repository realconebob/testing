#include "crtp.hpp"
#include "thingers.hpp"

int main(void) {
    ManyThinger<int> things{OddIntThinger{3}.any(), EvenIntThinger{3}.any()};
    std::printf("%s", things.do_things().c_str());
    return 0;
}