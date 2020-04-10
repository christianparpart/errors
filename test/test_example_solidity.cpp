#include <catch2/catch.hpp>
#include <errors/Error.h>

using Token = int; // stub
using Type = int; // stub
using TypePointer = Type const*;

namespace solidity {
    enum class Category {
        DeclarationError = 1,
        DocstringParsingError,
        ParserError,
        TypeError,
        SyntaxError,
        Warning,
    };
}

namespace fmt {
    template <>
    struct formatter<solidity::Category> {
        template <typename ParseContext>
        constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }
        template <typename FormatContext>
        auto format(solidity::Category val, FormatContext& ctx) { return format_to(ctx.out(), int(val)); }
    };
}

namespace solidity::errors {
    // a little helper function to construct a DeclarationError
    template <typename... Args>
    constexpr ::errors::Error<Args...> declarationError(::errors::Code _code, std::string_view const& _message) noexcept {
        return ::errors::Error<Args...>((int)Category::DeclarationError, _code, _message);
    }

    // a little helper function to construct a TypeError
    template <typename... Args>
    constexpr ::errors::Error<Args...> typeError(::errors::Code _code, std::string_view const& _message) noexcept {
        return ::errors::Error<Args...>((int)Category::TypeError, _code, _message);
    }

    // Errors found in ConstantEvaluator.cpp:
    constexpr auto incompatibleOperator = declarationError<Token, Type, Type>(1, "Operator {} not compatible with type {} and {}.");
    constexpr auto cyclicConstantDefinition = declarationError(2, "Cyclic constant definition (or maximum recursion depth exhausted).");

    // Errors found in ContractLevelChecker.cpp:
    constexpr auto moreThanOneDescriptorDefined = declarationError(3, "More than one constructor defined.");
    constexpr auto onlyOneFallbackFunctionAllowed = declarationError(4, "Only one fallback function is allowed.");
    constexpr auto onlyOneReceiveFunctionAllowed = declarationError(5, "Only one receive function is allowed.");
    constexpr auto interfacesDoNotNeedAbstractKeyword = typeError(6, "Interfaces do not need the \"abstract\" keyword, they are abstract implicitly.");
    constexpr auto librariesCannotBeAbstract = typeError(7, "Libraries cannot be abstract.");
    // ...
}

TEST_CASE("example_solidity")
{
    using namespace solidity::errors;

    auto const msg = incompatibleOperator(42, 1, 2);
    CHECK(msg.id == 1001);

    // XXX This is how reporting such errors could look like
    // m_errorRepoerter.report(m_currentSourceLocation, incompatibleOperator(42, 1, 2));
}
