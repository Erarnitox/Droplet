# Erarnitox's C++ Style Guide

__**Author:** Erarnitox (David L. Partsch) - david@erarnitox.de__

# C++ Project Style Guide

-----

## Introduction

This style guide provides comprehensive guidelines for writing high-quality C++ code within our projects.
It aims to foster consistency, readability, maintainability, and efficiency across the codebase.
These guidelines are built upon the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines), established industry best practices, and our team's specific requirements.

Adherence to this style guide is mandatory for all new code and highly encouraged for refactoring existing code.
Consistent application of these rules significantly reduces cognitive load for developers,
accelerates onboarding for new team members, and minimizes the risk of introducing bugs.

## Coding Style

Consistency in coding style is paramount for readability and understanding.
This section outlines specific rules for formatting, naming, and general code structure.

### General Formatting

  * **Indentation**: Use **real tabs** for indentation. Configure your editor to display tabs as 4 spaces for optimal readability.
  * **Braces**: Use **trailing braces** (`Allman style`) for all control flow statements (`if`, `else`, `for`, `while`, `do-while`, `switch`), functions, classes, structs, enums, namespaces, and lambda expressions.
    ```cpp
    if (condition) {
        // ...
    } else {
        // ...
    }

    void myFunction() {
        // ...
    }

    class MyClass {
    public:
        // ...
    };
    ```
  * **Line Length**: Aim for a maximum line length of **120 characters**. Break long lines thoughtfully for readability.
  * **Whitespace**:
      * Use spaces around binary operators (`=`, `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `&`, `|`, `^`, `<<`, `>>`).
      * No space between a function name and its opening parenthesis (`myFunction()`).
      * Space after commas in argument lists and template parameter lists (`func(arg1, arg2)`).
      * No space between unary operators and their operand (`*ptr`, `&ref`, `!flag`, `++i`).
      * One space after keywords like `if`, `for`, `while`, `switch`, `return`, `catch`, `throw`.
  * **Includes**:
      * Group includes logically: project-specific headers, then third-party headers, then standard library headers.
      * Sort includes alphabetically within each group.
      * Use angle brackets (`<>`) for standard library and system headers.
      * Use double quotes (`""`) for project-specific headers.
      * Avoid relative paths in includes where absolute paths (relative to include root) are possible.
      * Prefer `#pragma once` over include guards where compiler support allows.

### Naming Conventions

Consistent naming makes code self-documenting.

  * **Variables (local and member)**: Use `snake_case`.
    ```cpp
    int user_age;
    std::string user_name;
    int m_member_variable; // For member variables, consider 'm_' prefix
    ```
  * **Functions and Methods**: Use `snake_case`.
    ```cpp
    void calculate_total_amount();
    bool is_valid_input();
    ```
  * **Classes and Enums**: Use `CapitalizedCamelCase`.
    ```cpp
    class TaskList;
    enum class TaskState;
    ```
  * **Structs**: Use `CapitalizedCamelCase`.
    ```cpp
    struct TaskDetails;
    ```
  * **Constants and Macros**: Use `ALL_CAPS_WITH_UNDERSCORES`.
    ```cpp
    const int MAX_USERS = 100;
    #define BUFFER_SIZE 1024
    ```
  * **Namespaces**: Use `snake_case` or `lower_case`.
    Consider nested namespaces for larger projects.
    ```cpp
    namespace project_name { namespace sub_module { ... } }
    ```
  * **Type Aliases (`using`)**: Use `CapitalizedCamelCase` for new types.
    ```cpp
    using UserId = int;
    using TaskCollection = std::vector<Task>;
    ```

### Initialization

  * **Uniform Initialization**: Always use list initialization (brace-initialization `{}`) for variables to prevent narrowing conversions and promote consistency.
    ```cpp
    auto f{0.5f}; // Correct: float literal
    int count{0};  // Correct
    std::vector<int> numbers{1, 2, 3}; // Correct

    // Instead of:
    // float = 0.5; // Implicit cast, potentially double to float
    // int count = 0; // Copy initialization
    // std::vector<int> numbers = {1, 2, 3};
    ```
  * **Literals**: Make full use of type-specific literals (e.g., `0.5f` for float, `10ULL` for unsigned long long, `123_km` for user-defined literals if applicable) to avoid implicit casts and improve clarity.

### Return Types

  * **Explicit Returns**: Every execution path of a non-void function **must** have an explicit `return` statement.
  * **Void Functions**: For `void` functions, an explicit `return;` at the end of the function body is recommended for clarity, but not strictly mandatory unless required for early exit.
  * **Lambda Style Return Types**: **DO NOT** use `auto function_name() -> ReturnType` syntax for regular functions. Stick to the traditional `ReturnType function_name()` syntax for readability and consistency.
    ```cpp
    // Use:
    int main() {
        //...
        return 0;
    }

    // Instead of:
    auto main() -> int {
        //...
        return 0;
    }
    ```
### Qualifiers and Attributes

  * **`const` Correctness**: Use `const` as much as possible for variables, parameters (pass by `const&`), and member functions.
      * `const` member functions guarantee they do not modify the object's state.
      * `const` references or pointers prevent modification of the referred-to object.
      * `const` variables indicate values that should not change after initialization.
  * **`[[nodiscard]]`**: Use `[[nodiscard]]` for functions or methods whose return value *must* be used, e.g., error codes, new objects, or results of computations. This helps prevent silent bugs from ignored return values.
    ```cpp
    [[nodiscard]] bool connect_to_db();
    ```
  * **`[[maybe_unused]]`**: Use `[[maybe_unused]]` for parameters or variables that might not be used in all configurations or debug modes to silence compiler warnings without removing potentially useful code.
  * **`[[likely]]` / `[[unlikely]]` (C++20)**: Use these attributes to provide hints to the compiler for optimization in performance-critical paths, indicating whether a branch is likely or unlikely to be taken.

### Comments

Comments should explain *why* code is written, not *what* it does (unless the "what" is non-obvious). Good code is self-documenting; comments supplement that.

#### File Header Comment

Every source (`.cpp`, `.hpp`) file must begin with a standardized header comment.

```cpp
/*
 * (c) Copyright [Your Company Name/Author Name] - All rights reserved
 * Author: Your Name <your.email@example.com>
 *
 * License: [e.g., MIT License, Proprietary]
 *
 * Description: A concise, one-line summary of the file's purpose.
 * Expand on this if a single line is insufficient,
 * providing context and main responsibilities.
 *
 * Documentation: [Optional] Link to relevant Doxygen documentation,
 * design documents, or usage examples.
 */
```

#### Function/Method Documentation

Every function and method implementation (preferably in the header declaration for Doxygen parsing) should have an introductory comment. Use Doxygen-style comments (`/** ... */` or `///`) for automatic documentation generation.

```cpp
/**
 * @brief Handles button clicks in form elements.
 *
 * This function processes click events generated by user interaction with
 * interactive form buttons. It iterates through registered command handlers
 * to dispatch the event to the appropriate logic.
 *
 * @param event The button click event data initiated by the user.
 * @param bot   The cluster representing the current bot instance, used for
 * responding to the user or accessing global bot state.
 * @return      This function does not return a value but may trigger side
 * effects, such as sending a message back to the user in chat.
 */
static inline
auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void {
    // Function body...
}
```

#### Inline Comments

  * Use inline comments sparingly to explain non-obvious logic, tricky algorithms, or rationale behind specific choices.
  * Avoid commenting on obvious code.
  * Keep comments up-to-date with code changes. Outdated comments are worse than no comments.

#### Special Purpose Comments

  * `// TODO:`: Use for functionality that needs to be implemented or completed. Include a brief description and optionally your name/initials and date.
    `// TODO(erarnitox): Implement error handling for database connection (2025-07-07)`
  * `// FIXME:`: Use for code that is broken, partially working, or known to have issues that need fixing.
    `// FIXME: This mutex might cause a deadlock in high-concurrency scenarios.`
  * `// HACK:`: Use for a temporary or ugly solution that needs refactoring. Explain why it's a hack and what the proper solution would be.
  * `// NOTE:`: Use for important observations, caveats, or design decisions that readers should be aware of.
  * `// REVIEW:`: Use to flag code that needs a specific review or discussion point.

## Modern C++ Features

We embrace modern C++ features to write more concise, expressive, and efficient code. The chosen standard ensures a balance between modern capabilities and compiler support.

### C++ Standard

  * The C++ standard in use is **C++23**. All new code should leverage C++23 features where appropriate.
  * Familiarize yourself with features like concepts, ranges, coroutines, and new standard library additions.

### Core Language Features

  * **`auto` Type Deduction**: Use `auto` almost always for variable declarations where the type is obvious from the initializer. This reduces verbosity and avoids redundancy.
    ```cpp
    auto name = get_user_name(); // Preferred
    // std::string name = get_user_name(); // Less preferred
    ```
      * **Caveat**: Be explicit when `auto` could obscure the actual type or lead to unexpected behavior (e.g., `auto&&` for forwarding references, `auto` for return types of complex expressions).
  * **Range-Based For Loops**: Always prefer `for (auto& item : container)` or `for (const auto& item : container)` for iterating over containers.
  * **Move Semantics**: Utilize move constructors, move assignment operators, and `std::move` to transfer ownership of resources efficiently.
  * **Smart Pointers**:
      * Use `std::unique_ptr` for exclusive ownership of dynamically allocated objects.
      * Use `std::shared_ptr` for shared ownership (rarely needed if `unique_ptr` can suffice).
      * Use `std::weak_ptr` to break circular dependencies when using `std::shared_ptr`.
      * **Never** use `new` and `delete` directly unless managing raw resources (e.g., in a custom smart pointer).
  * **Lambda Expressions**: Use lambdas for concise inline functions, especially with algorithms and for event handling.
  * **Enums and `enum class`**: Always prefer `enum class` (scoped enums) to avoid name collisions and provide strong typing.
    ```cpp
    enum class TaskState { NotStarted, InProgress, Completed };
    ```
  * **`std::optional`, `std::variant`, `std::any` (C++17)**: Use these for expressing optional values, discriminated unions, and heterogeneous types, respectively, in a type-safe manner.
  * **Concepts (C++20)**: Use concepts to specify requirements on template parameters, improving compile-time error messages and making generic code more readable.
  * **Ranges (C++20)**: Leverage `std::ranges` for elegant and composable operations on sequences.
  * **Coroutines (C++20)**: Consider coroutines for asynchronous programming patterns where appropriate, but understand their complexity.
  * **Modules (C++20)**: **The usage of C++20 Modules is currently NOT ALLOWED** due to insufficient compiler support on the build server. Revisit this policy when toolchain support matures.

### Standard Library Features

  * **Algorithms**: Prefer `std::algorithms` (e.g., `std::sort`, `std::find`, `std::transform`) over manual loops when applicable. With C++20 Ranges, this becomes even more powerful.
  * **Containers**: Use appropriate standard containers (`std::vector`, `std::map`, `std::unordered_map`, `std::set`, `std::string`, etc.) for your data structures. Understand their performance characteristics.
  * **Utilities**: Utilize `std::chrono` for time, `std::filesystem` for file system operations, `std::tuple` for grouping elements, etc.

### Forbidden Features

  * **Raw Pointers for Ownership**: Never use raw pointers (`T*`) for managing ownership of dynamically allocated memory. Always use smart pointers. Raw pointers are acceptable for non-owning references.
  * **C-style Arrays**: Prefer `std::array` or `std::vector` over C-style arrays.
  * **`#define` for Constants**: Prefer `const`, `constexpr`, or `enum class` over `#define` for defining constants.
  * **`using namespace std;` in Headers**: Never place `using namespace std;` in header files. In `.cpp` files, it's generally discouraged but can be used in narrow scopes (e.g., within a function) if it significantly improves readability.
  * **Global Variables (mutable)**: Avoid mutable global variables. Use dependency injection or singletons (with caution) if global state is absolutely necessary.
  * **Magic Numbers**: Avoid using unexplained numerical literals directly in code. Define them as named constants.

## Design Practices

Good design principles are foundational for creating maintainable, scalable, and robust software.

### General Principles

  * **Separation of Concerns**: Each class, module, or function should have a single, well-defined responsibility.
  * **Encapsulation**: Hide implementation details and expose only necessary interfaces. Use `private` and `protected` appropriately.
  * **SOLID Principles**: Strive to adhere to the SOLID principles (Single Responsibility, Open/Closed, Liskov Substitution, Interface Segregation, Dependency Inversion).
  * **DRY (Don't Repeat Yourself)**: Avoid duplicating code logic. Extract common patterns into reusable functions or classes.
  * **KISS (Keep It Simple, Stupid)**: Favor simple, straightforward solutions over overly complex ones.
  * **Modularity**: Design components that can be developed, tested, and maintained independently.
  * **Loose Coupling, High Cohesion**: Components should have minimal dependencies on each other (loose coupling) and elements within a component should be strongly related (high cohesion).
  * **Composition Over Inheritance**: Favor composition (`has-a` relationship) over inheritance (`is-a` relationship) to achieve flexibility and avoid the complexities of deep inheritance hierarchies.
  * **Dependency Injection**: Use dependency injection to manage dependencies between objects, improving testability and flexibility.

### Error Handling

  * **Exceptions**: Use exceptions for truly exceptional conditions that prevent a function from fulfilling its contract.
      * Throw exceptions by value, catch by `const&`.
      * Avoid throwing exceptions from destructors.
      * Provide meaningful exception messages.
  * **Error Codes/`std::optional`/`std::expected`**: For expected error conditions or return values that might be absent, consider using:
      * `std::optional<T>` (C++17) for functions that might return a valid value or nothing.
      * A custom error code enum (for simple cases).
      * `std::expected<T, E>` (C++23 or via third-party library) for functions that can return a value on success or an error object on failure. This is often preferred over exceptions for recoverable errors.
  * **Assertions**: Use `assert` (from `<cassert>`) for checking preconditions and postconditions that indicate programming errors (bugs) that should never happen in release builds. Assertions are typically removed in release builds.

### Concurrency

  * **Thread Safety**: Explicitly design for thread safety where necessary.
      * Protect shared mutable data with mutexes (`std::mutex`, `std::recursive_mutex`).
      * Use `std::atomic` for atomic operations on fundamental types.
      * Employ `std::condition_variable` for thread synchronization.
      * Prefer higher-level concurrency primitives (e.g., `std::async`, `std::jthread`, thread pools) over raw `std::thread` when possible.
  * **Avoid Deadlocks/Race Conditions**: Be extremely careful when dealing with multiple locks. Follow consistent locking order and minimize lock hold times.
  * **Immutable Data**: Favor immutable data structures to reduce the need for synchronization.

## Unit Testing

Unit tests are a critical part of our development process, ensuring code correctness and preventing regressions.

### Principles

  * **Thoroughness**: Write comprehensive unit tests for all non-trivial code. Aim for high code coverage, but prioritize testing logic and edge cases over merely reaching a coverage percentage.
  * **Isolation**: Each unit test should be isolated, testing a single unit (class or function) independently of other units or external dependencies. Use mocks or test doubles for dependencies.
  * **Repeatability**: Tests must produce the same result every time they are run, regardless of environment or execution order.
  * **Speed**: Unit tests should run quickly to facilitate frequent execution.
  * **Maintainability**: Tests should be easy to understand and update as the codebase evolves.

### Framework

  * **Catch2**: We use [Catch2](https://github.com/catchorg/Catch2) as our primary unit testing framework.
      * Leverage its flexible `TEST_CASE` and `SECTION` structure for clear test organization.
      * Utilize its powerful assertion macros (`REQUIRE`, `CHECK`).
      * Employ test fixtures (`TEST_CASE_METHOD`) for common setup/teardown logic (e.g., for database tests).

### Test Structure

  * **Test Files**: Create a dedicated test file (e.g., `MyClassTests.cpp`) for each major class or module.
  * **Test Cases**: Each `TEST_CASE` should represent a specific scenario or aspect of the unit being tested.
  * **Sections**: Use `SECTION`s within `TEST_CASE`s to test different branches of logic or distinct behaviors.
  * **Dummy/Mock Objects**: For dependencies (like database connections, network services, or complex objects), use test doubles (mocks, stubs, fakes) to maintain isolation. Consider libraries like [Google Mock](https://github.com/google/googletest/blob/main/googlemock/README.md) if Catch2's built-in capabilities aren't sufficient.
  * **Test Data**: Use realistic but minimized test data. Create helper functions or fixtures for generating common test data.

### Integration and Reporting

  * **CMake and CTest**: Integrate unit tests with CMake using `add_test()` and run them with `ctest`.
  * **Continuous Integration (CI)**: Ensure unit tests are run automatically as part of the CI pipeline (e.g., GitHub Actions, GitLab CI). A failing test should block merging.
  * **CDash (Optional)**: For larger projects, integrate with CDash for a web-based dashboard of test results, build status, and code coverage. (Refer to the separate CDash integration guide).

## Documentation

Proper documentation is crucial for collaboration, onboarding, and long-term maintainability.

### Code Comments

(Refer to the [Comments](#Comments) section for detailed guidelines on file headers, function documentation, and inline comments.)

### Doxygen

  * We use [Doxygen](https://www.doxygen.nl/) to generate comprehensive API documentation from source code comments.
  * Ensure all public classes, methods, enums, and significant functions are documented using Doxygen-compatible syntax (`/** ... */` or `///`).
  * Use `@brief`, `@param`, `@return`, `@pre`, `@post`, `@note`, `@warning`, etc., tags appropriately.
  * Maintain the Doxygen configuration file (`Doxyfile`) to generate accurate and useful documentation.

### External Documentation

  * Maintain separate design documents, architectural overviews, and user manuals as needed, typically in Markdown format within the repository or a dedicated documentation system.
  * Keep `README.md` files in repositories and subdirectories up-to-date with project setup, build instructions, and key information.

## Version Control

We use Git as our primary version control solution. Consistent practices ensure a clean, traceable history.

### Git Workflow

  * **Commit Often**: Make small, logical, self-contained commits. Each commit should address a single concern or introduce a single feature/fix.
  * **Atomic Commits**: Ensure each commit leaves the codebase in a working state (passes tests).
  * **Push Regularly**: Push your changes to the remote repository frequently, especially when working on a shared branch.

### Commit Messages

Follow a consistent commit message format:

1.  **Subject Line (50-72 chars max)**: Concise summary, imperative mood, no period.
      * Examples: `feat: Add user authentication module`
      * `fix: Resolve null pointer dereference in parser`
      * `refactor: Extract common validation logic to helper`
      * `docs: Update installation guide`
      * `test: Add unit tests for TaskList class`
      * `chore: Update build dependencies`
2.  **Blank Line**: One blank line after the subject.
3.  **Body (Optional)**: Detailed explanation of *why* the change was made, *what* problem it solves, and *how* it was solved. Wrap lines at 72 characters. Reference issue tracker IDs if applicable.

Example:

```
feat: Implement user profile management

This commit introduces the initial user profile management
functionality, allowing users to view and edit their basic
profile information.

- Added UserProfile class with fields for name, email, and bio.
- Implemented API endpoints for GET /profile and PUT /profile.
- Included basic input validation for profile updates.

Resolves #123
```

### Branching Strategy

Adopt a clear branching strategy (e.g., Git Flow, GitHub Flow, GitLab Flow) for managing features, releases, and hotfixes. GitHub Flow (or a variation) is generally recommended for its simplicity and continuous delivery focus:

  * **`main` Branch**: Production-ready code. Only merge stable branches into `main`.
  * **Feature Branches**: Create short-lived branches off `main` (or `develop` if used) for each new feature or bug fix. Name them descriptively (e.g., `feature/add-dark-mode`, `bugfix/fix-login-error`).
  * **Pull Requests (PRs)**: All changes must go through a pull request (or merge request) process.
      * Require at least one reviewer.
      * Ensure all CI checks (build, tests, linting) pass before merging.
      * Use squashed or rebase merges to maintain a clean history on `main`.

## CMake

CMake is our build system generator. Adhere to these guidelines for maintainable and robust build scripts.

### General Principles

  * **Modern CMake**: Use modern CMake practices (target-based approach, `target_link_libraries` with `PUBLIC`/`PRIVATE`/`INTERFACE` keywords, generator expressions) over older, variable-based methods.
  * **Readability**: Organize `CMakeLists.txt` files logically, using comments and blank lines for clarity.
  * **Modularity**: Break down complex projects into smaller, manageable `CMakeLists.txt` files using `add_subdirectory()`.

### Naming Conventions

  * **Functions and Macros**: Use `snake_case`.
    ```cmake
    function(add_project_tests)
    macro(configure_compiler_flags)
    ```
  * **Variables**: Prefix custom project-specific variables with `DROP_` (e.g., `DROP_BUILD_WARNINGS_AS_ERRORS`). This helps avoid collisions with CMake's internal variables.
    ```cmake
    set(DROP_SOURCE_FILES src/main.cpp src/utils.cpp)
    ```
  * **Internal Variables**: Variables used only within a single `CMakeLists.txt` or function can follow standard CMake conventions (e.g., `_my_local_var`).

### Usage Guidelines

  * **Status Messages**: Use `message(STATUS "...")` frequently to inform the user about the build process state, detected features, or important configuration choices.
    ```cmake
    message(STATUS "Configuring project ${PROJECT_NAME} version ${PROJECT_VERSION}")
    ```
  * **Policy Warnings**: Resolve all CMake policy warnings. Set `cmake_minimum_required(VERSION X.Y FATAL_ERROR)` to enforce modern policies.
  * **Source Files**: Explicitly list all source files in `add_library()` or `add_executable()`. Avoid globbing (`file(GLOB ...)`) where possible as it doesn't handle new files automatically.

### Dependencies and Package Management

  * **CPM.cmake**: Use `CPM.cmake` for managing third-party dependencies. This ensures consistent dependency resolution across different development environments.
  * **`find_package()`**: For system-wide libraries (e.g., system-installed SQLite3), use `find_package()` with `REQUIRED` or `COMPONENTS` as appropriate.

### Platform Agnosticism

  * **Built-in Commands**: Utilize CMake's built-in platform-agnostic commands and variables (e.g., `CMAKE_SYSTEM_NAME`, `WIN32`, `UNIX`, `APPLE`) to handle platform-specific logic.
    ```cmake
    if(WIN32)
        target_compile_definitions(MyTarget PRIVATE WIN_SPECIFIC_FEATURE)
    else()
        target_compile_definitions(MyTarget PRIVATE UNIX_SPECIFIC_FEATURE)
    endif()
    ```
  * **Avoid OS-specific Shell Commands**: Minimize the use of `execute_process()` with platform-specific shell commands. If necessary, wrap them in platform checks.

### Generator Expressions

  * **Full Notation**: Always use the full notation for generator expressions, even for simple conditions. This improves readability and reduces potential ambiguity.
    ```cmake
    # Use this:
    target_link_libraries(MyTarget PRIVATE
        $<IF:$<CONFIG:Debug>,MyDebugLib,MyReleaseLib>
    )

    # Instead of:
    # target_link_libraries(MyTarget PRIVATE
    #     $<CONFIG:Debug:MyDebugLib,MyReleaseLib>
    # )
    ```
  * **Clarity**: Use generator expressions judiciously. If an expression becomes too complex, consider breaking it down or using variables.

## Debugging and Profiling

Effective debugging and profiling are essential for identifying and resolving issues and optimizing performance.

### Debugging Techniques

  * **IDE Debuggers**: Utilize your IDE's debugger (e.g., GDB in CLion/VS Code, Visual Studio Debugger). Learn to set breakpoints, inspect variables, step through code, and evaluate expressions.
  * **Assertions**: Use `assert()` for internal consistency checks. These are removed in release builds.
  * **Logging**: Implement a robust logging system (see below) for tracing execution flow and variable states.
  * **Debugging Builds**: Always use debug builds (`CMAKE_BUILD_TYPE=Debug`) during development to enable debug symbols and disable optimizations that can obscure debugging.
  * **Valgrind/AddressSanitizer (ASan)/UndefinedBehaviorSanitizer (UBSan)**: Use these tools frequently during development to detect memory errors, undefined behavior, and potential bugs. Integrate them into your CI pipeline.

### Logging

  * **Dedicated Logging Library**: Use a dedicated logging library (e.g., `spdlog`, `Boost.Log`, custom simple logger) rather than `std::cout` for production code.
  * **Log Levels**: Use different log levels (e.g., `DEBUG`, `INFO`, `WARNING`, `ERROR`, `FATAL`) to control verbosity.
  * **Contextual Information**: Include relevant contextual information in log messages (timestamps, thread IDs, function names, source file/line numbers).
  * **Performance**: Be mindful of logging overhead in performance-critical sections.

### Profiling

  * **Profiling Tools**: Use performance profiling tools (e.g., `perf`, `gprof`, `Valgrind --tool=callgrind`, Visual Studio Profiler, Intel VTune) to identify performance bottlenecks.
  * **Measure Before Optimizing**: Don't optimize prematurely. Profile your code to pinpoint actual bottlenecks before attempting optimizations.
  * **Targeted Optimization**: Focus optimization efforts on the identified hotspots.

## Code Review

Code review is a crucial practice for maintaining code quality, sharing knowledge, and catching defects early.

  * **Mandatory Reviews**: All code changes (via pull requests) must undergo a review by at least one other team member before merging.
  * **Constructive Feedback**: Provide constructive, respectful, and actionable feedback. Focus on the code, not the person.
  * **Reviewer Responsibilities**: Reviewers are responsible for checking adherence to this style guide, correctness, design flaws, potential bugs, test coverage, and documentation.
  * **Author Responsibilities**: Authors are responsible for addressing feedback, explaining decisions, and iterating on their changes.

## Security Considerations

Security is a primary concern in all development efforts.

  * **Input Validation**: Always validate all external inputs (user input, network data, file contents) to prevent vulnerabilities like injection attacks, buffer overflows, and invalid states.
  * **Least Privilege**: Design components to operate with the minimum necessary privileges.
  * **Secure Defaults**: Favor secure-by-default configurations.
  * **Error Handling (Security)**: Implement robust error handling to prevent information leakage or unexpected behavior that could be exploited.
  * **Known Vulnerabilities**: Stay informed about common C++ vulnerabilities (e.g., buffer overflows, integer overflows, use-after-free, race conditions) and practice defensive programming.
  * **Third-Party Libraries**: Be judicious when selecting third-party libraries. Prefer well-maintained, reputable libraries and regularly update them to patch security vulnerabilities.
  * **Memory Safety Tools**: Utilize ASan and UBSan during testing and development to catch memory-related issues.

## Not Listed

For any coding style, design, or implementation detail not explicitly covered in this document, please refer to the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) as the authoritative source for C++ best practices. If a conflict arises between this guide and the C++ Core Guidelines, this document takes precedence. For new features or scenarios, discuss with the team to establish a consistent approach.
