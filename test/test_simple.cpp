#include <catch2/catch.hpp>
#include <errors/Error.h>
#include <errors/Registry.h>

TEST_CASE("simple")
{
    errors::Registry reg;

    auto const wrongFile = reg.registerError<std::string, int>(1, 2, "Wrong include in file {} at line {}.");

    auto const wrongFileMessage = wrongFile("Hello.txt", 42);

    CHECK(wrongFileMessage.message == "Wrong include in file Hello.txt at line 42.");
    CHECK(wrongFileMessage.id == 1002);
}
