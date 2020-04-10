#pragma once

#include <string_view>
#include <fmt/format.h>

namespace errors {

using Category = unsigned int;
using Code = unsigned int;
using Id = unsigned int;

constexpr Id makeId(Category _category, Code _code) noexcept
{
    return Id(static_cast<unsigned>(_category) * 1000 + static_cast<unsigned>(_code));
}

struct Message {
    Id id;
    std::string message;
};

/// Message is the general descriptor of an error message.
///
/// Message contains informations such as error cateogry, error code, error message (may need
/// formatting arguments) and its unique error ID.
template <typename... Args>
class Error {
  public:
    constexpr Error(Category const& _category, Code _code, std::string_view const& _text) :
        category_{_category},
        code_{_code},
        text_{_text}
    {}

    /// @returns the Message's category.
    constexpr Category category() const noexcept { return category_; }

    /// @returns the Message's code. This code is dependant on the category.
    constexpr Code code() const noexcept { return code_; }

    /// @returns a stable unique Id representing this Message.
    constexpr Id id() const noexcept { return makeId(category(), code()); }

    /// @returns the message (may need formatting).
    constexpr std::string_view text() const noexcept { return text_; }

    /// @returns an error message contructed out of this error and the given formatting arguments.
    [[nodiscard]] Message message(Args&&... _args) const
    {
        return Message{id(), fmt::format(text_, std::forward<Args>(_args)...)};
    }

    /// @returns an error message contructed out of this error and the given formatting arguments.
    [[nodiscard]] Message operator()(Args&&... _args) const
    {
        return message(std::forward<Args>(_args)...);
    }

  private:
    Category const category_;
    Code const code_;
    std::string_view const text_;
};

} // end namespace
