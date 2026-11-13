/**
 * @brief Standard object oriented type erasure
 */

#include <cstdio>
#include <vector>

class ThingerInterface {
    protected:
    // Rule of 5. Allows classes to extend but not create raw instances
    ThingerInterface() = default;

    public:
    // Rule of 5
    ThingerInterface(const ThingerInterface&) = delete;
    ThingerInterface(ThingerInterface&&) = delete;
    ThingerInterface& operator=(const ThingerInterface&) = delete;
    virtual ~ThingerInterface() = default;

    virtual int dothing() const = 0;
};

class ThingerA: public ThingerInterface {
    public:
    ThingerA() {}
    ~ThingerA() {}

    int dothing() const override {
        std::printf("ThingerA dothing\n");
        return 1;
    }
};

class ThingerB: public ThingerInterface {
    public:
    ThingerB() {}
    ~ThingerB() {}

    int dothing() const override {
        std::printf("ThingerB dothing\n");
        return 2;
    }
};

int main(void) {
    auto ta = ThingerA{};
    auto tb = ThingerB{};

    for(const auto& thing: (const ThingerInterface* const []){&ta, &tb}) {
        std::printf("Doing thing: %d\n", thing->dothing());
    }

    return 0;
}