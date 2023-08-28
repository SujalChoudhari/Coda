# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.0.11] - 28-8-2023

### Added
- copy operator `:=`


## [0.0.10] - 14-8-2023

### Added
- `scope` keyword
- Scopes, ability to create a new scope and access the parent scope. `scope <name> { <block of code> }`
- functions no longer return the last evaluated value, use `return` instead.


## [0.0.9] - 13-8-2023

### Added
- `import` keyword
- import copies the code from the file and pastes it in the current file (like `#include` in C)
- `return`, `break` and `continue` keywords added as type `<jump>`, hence are assignable and returnable.

### Changed
- function parameters are constants (can't be changed)
- functions create a seperate scope for themselves, hence variables declared inside a function are not accessible outside the function. Function body `<block>` is a seperate scope.
- Each imported file is imported only once, hence if a file is imported twice, it will be imported only once.


## [0.0.8] - 12-8-2023

### Added
- `for` loop
- `while` loop
- `do-while` loop
- `list` data type along with `list` keywords and`[`,`]` (member access operators)

### Changed
- loop created a separate environment.
- `+` is added as unary operator (no effect to values)
- identifiers can be alphanumeric and can contain `	_` (underscore) in between. (first letter must be alphabet)

## [0.0.7] - 11-8-2023

### Added
- `if`, `elif` and `else` statements.
- Unary operators `++`, `--` and `!`.
- `+=`, `-=`, `*=` and `/=` operators.

## [0.0.6] - 10-8-2023

### Added
- Unary Operator (`+`,`-`)
- Safety for unknown errors (caused in the interpreter)
- `exit()` and `quit()` functions
- logical and relational operators added (`<`,`>`,`<=`,`>=`,`==`,`!=`) , (`&&`,`||`)


## [0.0.5] - 05-06-2023

### Changed
- Functions in Objects are supported
- Bugs fixed

### Added
- Runtime error for different size of arguments in function call


## [0.0.4] - 30-05-2023
Command Line Arguments

### Added
- Command Line Arguments
- `-w` flag for waiting after execution

### Changed
- Declaration of variables

## [0.0.3] - 29-05-2023
Member Expressions

### Added
- Object Member Access Expressions
- Object Member Call Expressions		
- Object Reassignment


## [0.0.2] - 28-05-2023
User Defined Functions

### Added
- User Defined Functions
- Closures
- Command line Execution of Interpreter

### Removed
- `return` keyword


## [0.0.1] - 25-05-2023
First Release (Coda Release-alpha)

### Added
- Lexer/Tokenizer
- Parser
- Interpreter
- Arithmetic operation handler
- Boolean Types
- Strings
- Char
- Variables (Assignment - dynamic)
- Variables (Assignment - constants)
- Variables (Look Up)
- Objects (Js-like objects)
- Native Functions  (printing,inputs,sleep)
