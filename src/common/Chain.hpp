#pragma once

#include <functional>
#include <type_traits>

namespace bud::cl {

template<typename T> class Chain;

template<typename Ret, typename ... Targs>
class Chain<Ret(Targs...)> {
    std::function<Ret(Targs...)> m_chain;
public:
    Chain() = default;
    Chain(const std::function<Ret(Targs...)>& fn) : m_chain(fn) {}
    Chain(std::function<Ret(Targs...)>&& fn) : m_chain(std::move(fn)) {}
    template<typename Fn>
    Chain<std::invoke_result_t<Fn, Ret>(Targs...)> chain(Fn&& fn) {
        return Chain<std::invoke_result_t<Fn, Ret>(Targs...)>([m_chain = std::move(m_chain), fn = std::forward<Fn>(fn)](Targs&&... args) {
            return fn(m_chain(std::forward<Targs>(args)...));
        });
    }
    void chainInPlace(const std::function<Ret(Targs...)>& fn) {
        m_chain = [m_chain = std::move(m_chain), fn](Targs&&... args) {
            return fn(m_chain(std::forward<Targs>(args)...));
        };
    }
    void chainInPlace(std::function<Ret(Targs...)>&& fn) {
        m_chain = [m_chain = std::move(m_chain), fn = std::move(fn)](Targs&&... args) {
            return fn(m_chain(std::forward<Targs>(args)...));
        };
    }
};

}