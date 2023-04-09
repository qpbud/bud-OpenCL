#pragma once

#include <variant>
#include <string_view>
#include <span>
namespace bud::cl::hal {

class Program {
public:
    using SourceVariant = std::variant<std::string_view, std::span<unsigned char>, std::span<unsigned char>>;
private:
    SourceVariant m_sourceVariant;
public:
    static SourceVariant setSource(std::string_view source) { return SourceVariant(std::in_place_index<0>, source); }
    static SourceVariant setIL(std::span<unsigned char> il) { return SourceVariant(std::in_place_index<1>, il); }
    static SourceVariant setBinary(std::span<unsigned char> binary) { return SourceVariant(std::in_place_index<2>, binary); }
    Program(SourceVariant&& sourceVariant) : m_sourceVariant(std::move(sourceVariant)) {}
    virtual ~Program() = default;
};

}
