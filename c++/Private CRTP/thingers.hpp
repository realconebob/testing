#ifndef _CPP_TESTING__THINGERS__
#define _CPP_TESTING__THINGERS__

#include "crtp.hpp"

#include <concepts>
#include <vector>
#include <cstdio>
#include <memory>
#include <string>

template<typename T>
class BaseThinger {
    protected:
    BaseThinger() = default;
    ~BaseThinger() = default;

    using string = std::string;

    friend class Thinger<BaseThinger, T>;
    friend class ThingerAny<T>;

    std::shared_ptr<T> value_;

    T& cast_impl() const {return *value_;}
    const T& cast_const_impl() const {return *value_;}
    const T& get_impl() const {return *value_;}
};

class OddIntThinger: public BaseThinger<int>, public Thinger<OddIntThinger, int> {
    private:
    friend class Thinger<OddIntThinger, int>;
    friend class ThingerAny<int>;

    string do_thing_impl() const {
        return string{"Is "} + std::to_string(*value_) + string{" odd?: "}
        + (((*value_) % 2 == 1) ? string{"Yes"} : string{"No"});
    }

    OddIntThinger(): BaseThinger(), Thinger<OddIntThinger, int>() {}

    public:
    OddIntThinger(const OddIntThinger& other) {value_ = std::make_shared<int>(*other.value_);};
    OddIntThinger(OddIntThinger&& other): OddIntThinger() {value_ = other.value_; other.value_ = nullptr;}
    ~OddIntThinger() {}

    OddIntThinger(int&& val): OddIntThinger() {value_ = std::make_shared<int>(val);}
};

class EvenIntThinger: public BaseThinger<int>, public Thinger<EvenIntThinger, int> {
    private:
    friend class Thinger<EvenIntThinger, int>;
    friend class ThingerAny<int>;

    string do_thing_impl() const {
        return string{"Is "} + std::to_string(*value_) + string{" even?: "}
        + (((*value_) % 2 == 0) ? string{"Yes"} : string{"No"});
    }

    EvenIntThinger(): BaseThinger(), Thinger<EvenIntThinger, int>() {}

    public:
    EvenIntThinger(const EvenIntThinger& other): EvenIntThinger() {value_ = std::make_shared<int>(*other.value_);};
    EvenIntThinger(EvenIntThinger&& other): EvenIntThinger() {value_ = other.value_; other.value_ = nullptr;}
    ~EvenIntThinger() {}

    EvenIntThinger(int&& val): EvenIntThinger() {value_ = std::make_shared<int>(val);}
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