#ifndef _CPP_TESTING__CRTP__
#define _CPP_TESTING__CRTP__

#include <memory>
#include <string>

template<typename T>
class ThingerAny {
    struct Concept {
        virtual ~Concept() = default;
        virtual std::shared_ptr<Concept> clone() const = 0;

        virtual explicit operator T&() = 0;
        virtual explicit operator const T&() const = 0;
        virtual const T& get() const  = 0;

        virtual std::string do_thing() const = 0;
    };

    template<typename Th>
    struct Model: Concept {
        Th obj_;

        explicit Model(Th th): obj_(std::move(th)) {}
        std::shared_ptr<Concept> clone() const override {
            return std::make_shared<Model>(*this);
        }

        explicit operator T&() override {return obj_.cast_impl();}
        explicit operator const T&() const override {return obj_.cast_const_impl();}
        const T& get() const override {return obj_.get_impl();}

        std::string do_thing() const override {return obj_.do_thing_impl();}
    };

    std::shared_ptr<Concept> self_;

    public:
    template<typename Th>
    ThingerAny(Th th): self_(std::make_shared<Model<Th>>(std::move(th))) {}
    ThingerAny(const ThingerAny &o) : self_(o.self_->clone()) {}
    // ThingerAny(ThingerAny &&) noexcept = default;

    explicit operator T&() {return static_cast<T&>(*self_);}
    explicit operator const T&() const {return static_cast<const T&>(*self_);}
    const T& get() const {return self_->get();}
    std::string do_thing() const {return self_->do_thing();}
};

template<typename D, typename T>
class Thinger {
    protected:
    Thinger() = default;
    ~Thinger() = default;

    private:
    friend D;
    friend ThingerAny<T>;

    const D& sc() const {return static_cast<const D&>(*this);}
    D& sc() {return static_cast<D&>(*this);}

    public:
    explicit operator T&() const {return sc().cast_impl();}
    explicit operator const T&() const {return sc().cast_const_impl();}
    const T& get() const {return sc().get_impl();}

    std::string do_thing() const {return sc().do_thing_impl();}

    ThingerAny<T> any() &   {return ThingerAny<T>{sc()};}
    ThingerAny<T> any() &&  {return ThingerAny<T>{std::move(sc())};}
};

#endif