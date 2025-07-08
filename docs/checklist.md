[ ] Code adheres to the defined indentation (real tabs) and brace style (trailing braces).
[ ] Line length is within the 120-character limit, or lines are broken thoughtfully.
[ ] Consistent whitespace usage around operators, commas, and keywords.
[ ] Code is clear and easy to follow; complexity is minimized (`clear code > optimal code`).
[ ] Code is self-documenting; comments explain *why*, not *what*.
[ ] Redundancy is minimized (DRY principle); duplicate code is extracted or templated.
[ ] There are no magic numbers; all constants are named and explained.
[ ] The solution appears simple and straightforward, not overly convoluted for compactness.
[ ] Variables and functions use `snake_case`.
[ ] Classes and enums use `CapitalizedCamelCase`.
[ ] Structs use `camelCase` (or `CapitalizedCamelCase` if aligning with common practice).
[ ] Constants and macros use `ALL_CAPS_WITH_UNDERSCORES`.
[ ] Type aliases (`using`) use `CapitalizedCamelCase`.
[ ] Variables are initialized using uniform initialization (`{}`).
[ ] Type-specific literals are used (e.g., `0.5f`, `10ULL`).
[ ] All execution paths of non-void functions have an explicit `return`.
[ ] Lambda-style return types (`auto func() -> int`) are NOT used for regular functions.
[ ] `const` is used as much as possible for variables, parameters, and member functions.
[ ] `constexpr` is used for values known at compile time.
[ ] `consteval` is used for functions/values that MUST be evaluated at compile time.
[ ] `[[nodiscard]]` is applied to functions whose return value must not be ignored.
[ ] `[[maybe_unused]]` is used for intentionally unused parameters/variables.
[ ] `[[likely]]` / `[[unlikely]]` are used appropriately for performance hints (C++20).
[ ] All variables are initialized upon declaration (`always initialize variables`).
[ ] `auto` is used where the type is obvious (`almost always auto`).
[ ] Pointers that are not "optional" are declared as `not_null` (if using a library like `gsl::not_null`).
[ ] Code adheres to C++23 standard.
[ ] Range-based for loops are preferred over index-based for loops (`for (const auto& x : y)`).
[ ] Move semantics (`std::move`) are utilized for efficient resource transfer.
[ ] Smart pointers (`std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`) are used for ownership.
[ ] `std::unique_ptr` is the preferred pointer type for exclusive ownership.
[ ] Raw `new` and `delete` are avoided (`never a naked new`/`delete`).
[ ] `std::make_unique` is used instead of raw `new` for `unique_ptr`.
[ ] `enum class` is used instead of plain `enum`.
[ ] `std::optional`, `std::variant`, `std::any` are used where appropriate.
[ ] Concepts are used to specify template requirements.
[ ] `std::ranges` are utilized for sequence operations.
[ ] STL algorithms are preferred over manual loops (`could an STL Algorithm do the same thing?`).
[ ] `if constexpr(...)` is used for compile-time branching.
[ ] `emplace_back` is preferred over `push_back` for containers.
[ ] Constructors prefer initialization lists over assignment.
[ ] Structured bindings are used for tuple/struct destructuring.
[ ] Stronger types are used for function arguments (e.g., `Area` instead of `int width, int height`).
[ ] RAII (Resource Acquisition Is Initialization) is consistently applied.
[ ] `std::array` is preferred over C-style arrays.
[ ] `std::array` is preferred over `std::vector` when size is fixed at compile time.
[ ] Code follows Separation of Concerns.
[ ] Implementation details are encapsulated (private/protected members).
[ ] SOLID principles are considered.
[ ] Duplication is avoided (DRY).
[ ] Design is simple and easy to understand (KISS).
[ ] Components are modular and loosely coupled, with high cohesion.
[ ] Composition is preferred over inheritance.
[ ] Dependency injection is considered for managing dependencies.
[ ] Error handling uses exceptions for exceptional conditions, `std::optional`/`std::expected` for recoverable errors.
[ ] `std::endl` is not used in loops (prefer `'\n'`).
[ ] `std::bind` and `std::function` are avoided where lambdas or other mechanisms suffice.
[ ] Initialization lists for non-trivial types are used with awareness of potential overhead.
[ ] Functions are split into smaller, single-responsibility units (`split multi step functions`).
[ ] An ownership model is clear (single owner of data where possible).
[ ] No raw pointers owning data.
[ ] Ownership is never transferred by/to raw pointers.
[ ] Null pointers are always checked before dereferencing.
[ ] Raw pointers are not returned from functions (`never return raw pointers`).
[ ] Arrays are not passed as raw pointers (prefer `std::array&`).
[ ] `std::vector` is not used where `std::array` would be more appropriate.
[ ] Stack memory is preferred over heap memory where possible.
[ ] All user input is validated (`never trust user input`).
[ ] `gets()` is never used.
[ ] Assertions (`assert` from `<cassert>`) are used for internal consistency checks.
[ ] `static_assert` is used for compile-time assertions.
[ ] Code is robust against integer overflows/underflows (consider max app runtime, counter overflows).
[ ] Rounding errors accumulation is considered for floating-point arithmetic.
[ ] Self-assignment is handled correctly in assignment operators.
[ ] Compile with at least `-Wall -Wpedantic -Werror -Wconversion` (or equivalent).
[ ] All warnings are treated as errors.
[ ] Explicit conversions are preferred (`always convert explicitly`).
[ ] `const_cast` is not used (`never use const_cast`, `is often a logic error`).
[ ] `dynamic_cast` usage is justified (often indicates a design smell).
[ ] `switch` statements use `[[fallthrough]]` attribute where intended, otherwise have `break` or `return`.
[ ] `operator[]` on `std::map` is used with awareness that it inserts new entries for non-existent keys.
[ ] Design is extendable (`always keep your design extendable`).
[ ] File header comment is present and correctly formatted.
[ ] Functions and methods have Doxygen-style documentation (`@brief`, `@param`, `@return`, etc.) (preferably in headers).
[ ] Inline comments explain non-obvious logic, not what the code does.
[ ] Special comments (`TODO:`, `FIXME:`, `HACK:`, `NOTE:`, `REVIEW:`) are used appropriately.
[ ] Comments are up-to-date with code changes.
[ ] Unit tests are provided for all non-trivial code.
[ ] Tests are isolated and independent.
[ ] Tests are repeatable and run quickly.
[ ] Test coverage is adequate for critical paths and edge cases.
[ ] Test fixtures are used for common setup/teardown.
[ ] Mocks/test doubles are used for external dependencies.
[ ] Tests are integrated with CMake/CTest.

