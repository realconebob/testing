#ifndef _CPP_TESTING__THINGERS__
#define _CPP_TESTING__THINGERS__

#include "crtp.hpp"

#include <concepts>
#include <vector>
#include <cstdio>
#include <memory>
#include <string>

class OddThinger: public Thinger<OddThinger, int> {
    private:
    using string = std::string;

    friend class Thinger<OddThinger, int>;
    friend class ThingerAny<int>;

    std::shared_ptr<int> value_;

    int& cast_impl() const {return *value_;}
    const int& cast_const_impl() const {return *value_;}
    const int& get_impl() const {return *value_;}

    std::string do_thing_impl() const {
        return string{"Is "} + std::to_string(*value_) + string{" odd?: "}
        + (((*value_) % 2 == 1) ? string{"Yes"} : string{"No"});
    }

    public:
    OddThinger() = delete;
    OddThinger(const OddThinger& other): Thinger<OddThinger, int>(), value_(std::make_shared<int>(*other.value_)) {};
    OddThinger(OddThinger&& other): Thinger<OddThinger, int>(), value_(other.value_) {other.value_ = nullptr;}
    ~OddThinger() {}

    OddThinger(int&& val): Thinger<OddThinger, int>(), value_(std::make_shared<int>(val)) {}
};

class EvenThinger: public Thinger<EvenThinger, int> {
    private:
    using string = std::string;

    friend class Thinger<EvenThinger, int>;
    friend class ThingerAny<int>;

    std::shared_ptr<int> value_;

    int& cast_impl() const {return *value_;}
    const int& cast_const_impl() const {return *value_;}
    const int& get_impl() const {return *value_;}

    std::string do_thing_impl() const {
        return string{"Is "} + std::to_string(*value_) + string{" even?: "}
        + (((*value_) % 2 == 0) ? string{"Yes"} : string{"No"});
    }

    public:
    EvenThinger() = delete;
    EvenThinger(const EvenThinger& other): Thinger<EvenThinger, int>(), value_(std::make_shared<int>(*other.value_)) {};
    EvenThinger(EvenThinger&& other): Thinger<EvenThinger, int>(), value_(other.value_) {other.value_ = nullptr;}
    ~EvenThinger() {}

    EvenThinger(int&& val): Thinger<EvenThinger, int>(), value_(std::make_shared<int>(val)) {}
};

template<typename T>
class ManyThinger {
    private:
    std::vector<ThingerAny<T>> thingers_{};

    public:
    template<typename... Args>
    requires(std::convertible_to<Args, ThingerAny<T>> && ...)
    ManyThinger(Args... args): thingers_{static_cast<ThingerAny<T>>(args)...} {}

    std::string do_things() const {
        std::string res{};
        for(const auto& thinger: thingers_) {
            res += thinger.do_thing() + "\n";
        }
        return res;
    }
};

#endif