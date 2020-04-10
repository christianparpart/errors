# errors - a stable error message library

## Motivation and Goals

- stable error codes for each error being the same code across multiple software releases
- error message arguments are type-checked at compile-time
- error messages can be translatabled

## FAQ

### Why not using `enum class` for error types?

While it is easy to map an `enum class` to a string for constructing the error message,
it is not easy then to have formatting arguments statically typed at compile time.

### Why not `std::error_code` for the error types?

`std::error_code` does not support arguments.

### Why not `std::error_category` for representing the error category?

It's not `constexpr` nor does it map to an integer to construct the unique error Id.
