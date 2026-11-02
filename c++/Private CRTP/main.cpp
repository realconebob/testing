#include "crtp.hpp"
#include "thingers.hpp"

int main(void) {
    ManyThinger<int> things{OddThinger{3}.any(), EvenThinger{3}.any()};
    std::printf("%s", things.do_things().c_str());
    return 0;
}