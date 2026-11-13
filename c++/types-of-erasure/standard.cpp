/**
 * @brief Standard object oriented polymorphism/"type erasure"
 */

#include <cstdio>
#include <array>

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

    int dothing() const override {
        std::printf("ThingerA dothing\n");
        return 1;
    }
};

class ThingerB: public ThingerInterface {
    public:

    int dothing() const override {
        std::printf("ThingerB dothing\n");
        return 2;
    }
};

int main(void) {
    ThingerA ta{};
    ThingerB tb{};

    // Java may be annoying, but it has spoiled me here. I feel like I should be able to just upcast to
    // ThingerInterface, but this style makes sense if viewed like an opaque struct in C
    std::array<const ThingerInterface*, 2> thingers{&ta, &tb};
    for(const ThingerInterface* thing: thingers) {
        std::printf("Doing thing: %d\n", thing->dothing());
    }

    return 0;
}