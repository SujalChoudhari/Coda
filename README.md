# Coda
## C++ Objects Dynamic Architecture
Coda is a C++-based interpreted language designed to allow for rapid development and execution of code. 
Its built-in functions and libraries simplify common programming tasks, improving productivity. 
Coda also offers high performance and versatility thanks to its implementation in C++. 


## Features

- **Interpreted Language:** Coda is an interpreted language, 
allowing for rapid development and execution of code. 
Its interpreter is implemented in C++, ensuring high performance and versatility.

- **Utility-Focused:** Coda aims to be a versatile utility language, 	
providing a wide range of functionalities and tools to simplify common programming tasks. 
It focuses on improving productivity by offering convenient built-in functions and libraries.

## Getting Started

To start using Coda, follow these steps:

1. Install the Coda interpreter.
2. Write your Coda code in a text file with the `.coda` extension.
3. Run the Coda interpreter with the path to your Coda file as an argument. 
   For example, if your Coda file is named `hello.coda`, you would run the interpreter as follows:
```bash
Coda hello.coda

OR

Coda.exe hello.coda
```
## Syntax Overview
Coda has a simple syntax that is easy to learn and use. Most of the syntax is inspired by Python and JavaScript.

### Variables
Variables in Coda are dynamically typed, meaning that they can hold values of any type.
To declare a variable, use the let keyword followed by the variable name and the value to assign to it.

```js
let x = 5
let y = "Hello, World!"
let z = true
```

Assignment is not required when declaring a variable.
```js
let x
x = 5
```

Variables can be reassigned to values of different types.

```js
let x = 5
x = "Hello, World!"
```

### Assignment without let
Using let is also not required when assigning a value to a variable.
But it is recommended to use let for better readability.

```js
x = 5
x = "Hello, World!"
```

## Constants
Constants in Coda are similar to variables, except that they cannot be reassigned.
To declare a constant, use the const keyword followed by the constant name and the value to assign to it.

```js
const x = 5
const y = "Hello, World!"
const z = true
```
## Arithmetic Operators
Coda supports the following arithmetic operators:

* Addition +
* Subtraction -
* Multiplication *
* Division /
* Modulo %

Only numbers can be used with arithmetic operators.

```js
let x = 5 + 5
let y = 5 - 5
let z = 5 * 5
```
Strings can be concatenated using the + operator.

```js
let x = "Hello, " + "World!"
```
## Object
Objects in Coda are similar to objects in JavaScript.
Objects are created like normal variables, but they are assigned a value using curly braces.

```js
let x = {
	name: "John",
	age: 20
}
```
## Chaining Objects and Dot Operator
Objects can be chained together using the dot operator.
Dot operator is used to access the properties of an object.

```js
const x = {
	a: {
		b: "Hello",
	},
	b: "World",
	c: "!"
}

print(x.a.b + " " + x.b + x.c);
```
The following code will print Hi World!

## Functions
Functions in Coda are similar to functions in Python.
Functions are created like normal variables, but they are assigned a value using the def keyword.
A function always returns the value of the last expression in its body.

```py
def add(x, y) {
	x + y
}
```
Functions can be called using the () operator.
`add(5, 5)` 
Functions are just like variables. They can be assigned to other variables.

```js
let x = add
x(5, 5)
```
### Functions in Objects
Functions can be defined in objects. It is similar to defining a function in a variable.
But in this case, the function is defined in an object. The name of the function is the key of the object.

```js
let hello = {
	something: def(name){
		println("Hello, " + name + "!");
	},
	earth: def(){
		println("Hello, Earth!");
	}
}
let inp = input("What is your name? ");
hello.something("World");
hello.earth();
hello.something(inp);
```
## Built-in Functions
Coda has a number of built-in functions that can be used to perform common tasks.

```js
print("Hello, World!")
println("Hello, World!")
sleep(20)
parseInt("20")
input("Enter a number")
```

# Contributing
Contributions to the Coda language are welcome!
If you encounter any issues or have suggestions for improvements,
please open an issue or submit a pull request on the official Coda GitHub repository.

# Changelog
See the CHANGELOG file for details.
Go through the Changelog to see the progress of the language.

# License
Coda is open-source software released under the MIT License.




