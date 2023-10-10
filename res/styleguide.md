# DropSoft C++ Style Guide
__**Author:** Erarnitox (David L. Partsch) - david@erarnitox.de__

Table of Contents

- Introduction
- Coding Style
- Special Comments
- Modern C++ Features
- Design Practices
- Unit Testing
- Documentation
- Version Control
- Debugging

## Introduction
This style guide provides guidelines for writing high-quality C++ code. 
It is based on the C++ Core Guidelines and other best practices for writing C++ code.

## Coding Style
It is important to use a consistent coding style throughout our codebase to make it
easier to read and understand.
This includes things like indentation, naming conventions, and formatting.

Here are some specific guidelines to follow:

- use real tabs
- Use snake_case for naming variables and functions
- use trailing braces everywhere (if, else, functions, ...)
```cpp
if( x ) {
}
```
- Use comments to explain complex or non-obvious code
- almost always use auto
- use initializer lists where possible
- Use precision specification for floating point values
- initialize variables using the standard initialization and make use of literals to avoid implicit casts
```cpp
auto f{ 0.5f };

//instead of:
float = 0.5;
```
- every execution path of a function should have an explicit `return` even in void functions
- use lambda style return types:
```cpp
auto main() -> int {
	//...
	return 0;
}
```
- use capitalized CamelCase for naming classes
- use camelCase for naming structs
- use const as much as possible
- use [[nodiscard]] as much as possible

## Special Comments
### File header comment
Put a file header comment with the folling information into every file you create:
- Copyright
- Original author(s) <mail@domain.example>
- License
- One-line purpose statement or description
- link to documentation and usage examples

Here is an example:
```cpp
/**
 *  (c) Copyright dropsoft.org - All rights reserved
 *  Author: Erarnitox <david@erarnitox.de>
 *  
 *  License: Proprietary and confidential
 *  Unauthorized copying of this file, via any medium is strictly prohibited
 *
 *  Description: This class manages Database access 
 *  and provides a high level API for internal use
 *
 *  Documentation: https://droplet.dropsoft.org/doxygen/database
**/
```

### Function implementation header
Every function and method implementation should have an introductory comment like this:
```cpp
//////////////////////////////////////////////////////////////////////////////
// HANDLE ALL GLOBAL SLASH COMMANDS
//////////////////////////////////////////////////////////////////////////////
static inline
auto handle_button_clicks(const dpp::button_click_t& event, dpp::cluster& bot) -> void {
	for(auto commandFp : Commands::CommandFunctionPointers::handle_button_clicks()){
		Commands::call_handle_button_clicks(commandFp, event, bot);
	}
}
```

### Doxygen Comments

### Note Comments
- use `#TODO:` to annotate things that need to be done in the future
- use `#FIXME:` to annotate broken or partially working code

## Modern C++ Features
C++ has evolved significantly over the years, and there are many modern features
that can make our code more concise, expressive, and efficient.
These include things like range-based for loops, auto type deduction, move semantics,
and smart pointers.

Make sure to familiarize yourself with these features and use them where appropriate.

## Design Practices
Good design practices are essential for creating maintainable and scalable code.
This includes things like separating concerns, encapsulating implementation details,
and following SOLID principles.

Make sure to design your code with these principles in mind.

## Unit Testing
Unit tests are a critical part of any software development process,
and they are especially important for C++ code.
Unit tests help to ensure that your code is working as intended and can catch regressions
early on.

Make sure to write thorough unit tests for your code, and run them regularly to catch any issues.

## Documentation
Proper documentation is essential for helping others understand your code and make changes to it.
Make sure to include detailed comments for any complex or non-obvious code.

We are using Doxygen to generate more comprehensive documentation.

## Version Control
we are using git as version control solution.
For now the only policy is to commit often!
A specific branching strategy is specific to the specific projects.

## Not listed
If something is not mentioned here, follow the C++ Core Guidelines:
https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
