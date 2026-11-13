/**
 * @brief Half type erasure on crtp implementing classes via typical interface implementation
 */

#include <array>
#include <concepts>
#include <cstdio>
#include <type_traits>

template<typename T>
concept Floating = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, long double>;

template<Floating T>
class ThingerInterface {
    protected:
    ThingerInterface() = default;

    public:
    ThingerInterface& operator=(const ThingerInterface&) = delete;
    ThingerInterface(const ThingerInterface&) = delete;
    ThingerInterface(ThingerInterface&&) = delete;
    virtual ~ThingerInterface() = default;

    virtual void store(T input) = 0;
    virtual const T& get() const = 0;

    virtual void dothing() const = 0;
};

template<Floating T>
class ThingerA: public ThingerInterface<T> {
    private:
    T storage_;

    public:
    ThingerA(T input): storage_{input} {}

    void store(T input) override {storage_ = input;};
    const T& get() const override {return storage_;};

    void dothing() const override {
        std::printf("ThingerA dothing: %2.2f", 1 / static_cast<float>(storage_));
    }
};

template<Floating T>
class ThingerB: public ThingerInterface<T> {
    private:
    T storage_;

    public:
    ThingerB(T input): storage_{input} {}

    void store(T input) override {storage_ = input;}
    const T& get() const override {return storage_;}

    void dothing() const override {
        std::printf("ThingerB dothing: %2.2f", static_cast<float>(storage_) * 10.0f);
    }
};

int main(void) {
    ThingerA<double> ta{10.0};
    ThingerB<double> tb{7.0};

    std::array<const ThingerInterface<double>*, 2> thingers{&ta, &tb};
    for(const ThingerInterface<double> * const thing: thingers) {
        std::printf("Doing thing:\n");
        thing->dothing();
        std::printf("\n");
    }

    return 0;
}