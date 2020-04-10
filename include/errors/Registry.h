#pragma once
#include <errors/Error.h>

#include <system_error>
#include <string_view>
#include <unordered_map>
#include <any>
#include <stdexcept>

namespace errors {
///
/// This error registry can be used to register errors. Those errors are ensured to be unique.
///
class Registry {
  public:
    /// Constructs an error of type Error<Args...> with given error category and code.
    ///
    /// @returns an Error<Args...> error handle that can be used to construct the final error
    /// message but also read out its unique error code.
    template <typename... Args>
    Error<Args...> registerError(Category _category, Code _code, std::string_view const& _message)
    {
        auto const id = makeId(_category, _code);
        if (registered_.contains(id))
            throw std::invalid_argument("(Category, Code)-pair already registered.");

        registered_[id] = Error<Args...>(_category, _code, _message);
        return Error<Args...>(_category, _code, _message);
    }

    /// Returns the Error<> based on the Error's Id.
    template <typename... Args>
    auto error(Id _id) const
    {
        if (auto const i = registered_.find(_id); i != registered_.end())
            return std::optional{std::any_cast<Error<Args...>>(i->second)};
        else
            return std::nullopt;
    }

  private:
    std::unordered_map<Id, std::any> registered_;
};

} // end namespace
