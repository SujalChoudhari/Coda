# Contributing to Coda - C++ Objects Dynamic Architecture

Thank you for your interest in contributing to Coda! 
We appreciate your help in making our project better. 
Before you get started, please read and follow these guidelines 
to ensure a smooth and collaborative contribution process.

## Getting Started

- Make sure you have a GitHub account.
- Fork the repository on GitHub: [Coda GitHub Repository](https://github.com/SujalChoudhari/Coda)
- Clone your fork locally: `git clone https://github.com/your-username/Coda.git`
- Create a new branch for your contributions: `git checkout -b feature/your-initials-your-feature`.
  For example: `git checkout -b feature/sc-sound`

## Contributing Guidelines

### Code of Conduct

Please note that by participating in this project, 
you are expected to follow our [Code of Conduct](./CODE_OF_CONDUCT.md). 
Be respectful, kind, and considerate in all interactions.

### Bug Reports and Feature Requests

If you encounter a bug or have a feature request, 
please create an issue on our GitHub repository. 
Be sure to provide as much detail as possible, 
including steps to reproduce the issue if it's a bug.

### Making Changes

1. Make your changes in your local branch (your feature branch).
2. Ensure your code adheres to our coding standards and practices (naming conventions and structure of code).
3. Write meaningful commit messages (no emoji, no special characters).
4. Push your changes to your fork on GitHub.

### Pull Requests

When you are ready to submit your changes, follow these steps:

1. Ensure your fork is up to date with the latest changes from the main repository.
   ```
   git remote add upstream https://github.com/SujalChoudhari/Coda.git
   git fetch upstream
   git checkout main
   git merge upstream/main
   git push origin main
   ```

2. Create a pull request from your branch to the main repository.
   - Include a clear and concise title.
   - Provide a detailed description of your changes, including any relevant context.
   - Reference any related issues.

3. Be prepared to address feedback and make additional changes if necessary.

4. Once your pull request is approved, it will be merged with the dev branch by the project maintainers. 
   You do not need to merge it yourself.

### Testing

Please ensure that your code changes include appropriate tests. 
If you are not sure how to write tests for your contribution, feel free to ask for guidance.

### Documentation

If your changes involve modifications to documentation, 
update the relevant documentation files and provide clear explanations.

## Hacktoberfest Participation

We are excited to participate in Hacktoberfest! 
To be eligible for a Hacktoberfest reward, 
make sure your pull requests adhere to the Hacktoberfest guidelines.
If We feel that the Pull Requests are *spammy*, 
we will mark it as `spam` or `invalid` and will discard the pull request instantly.

## What You Can Contribute during Hacktoberfest23

### What Coda Needs From You

Coda is a programming language that has made significant progress, including the implementation of the FFI (Foreign Function Interface). However, to make Coda more powerful and versatile, it needs a wide range of libraries to expand its capabilities. This is where you can contribute during Hacktoberfest!

### How to Create Libraries

Creating libraries for Coda is a valuable way to contribute. Here's a step-by-step guide on how to create libraries that can be integrated into Coda:

1. **Set Up a Visual Studio Project**:
   - Navigate to the `./dll` folder in the Coda repository.
   - Create a new Visual Studio project, similar to existing ones like `OS` and `Maths` that are already located in the `./dll` folder.

2. **Build the Library**:
   - If you are creating a C++-based package, configure your Visual Studio project to build the package as a DLL (Dynamic Link Library). 
     If you are creating a package in the Coda language only, you may not need to build it as a DLL.

3. **(Only for DLL-Based Libraries) Access the FFI**:
   - Within your package project, you can access `FFI.h` from the Coda source code, along with other standard C++ libraries. 
     However, you should avoid accessing other parts of the Coda source code.

4. **Structure Your Code**:
   - Seek inspiration from existing packages like `Maths` and `OS` for guidance on organizing your library code.
   - Typically, your library should include C++ files (.cpp) and a `<name>.coda` file that contains the definitions of DLL functions.
   - If you don't have C++ code, simply create a `.coda` file with the contents in it.
     > Note: The name from the folder path `./dll/<name>` is used when importing modules using the `import` statement.

5. **Build and Test**:
   - Build your library using the Visual Studio project you created (applies to DLL-based libraries only).
   - **IMPORTANT:** Ensure that you copy the `<name>.coda` files to `$(SolutionDir)bin\$(Configuration)\$(Platform)\`.
   - Test your library to ensure it functions correctly and is compatible with Coda. (Please note that Coda may not be cross-platform at this time, but we are planning to make it so in the future.)

By creating libraries for Coda, you can help expand its functionality and make it more useful for developers. If you have any questions or need assistance during the library creation process, don't hesitate to reach out.

For more information on creating and managing pull requests, please refer to the [Official GitHub Documentation](https://docs.github.com/en/github/collaborating-with-pull-requests).

Happy coding, and thank you for contributing to Coda!


## Getting Help

If you need assistance or have any questions, feel free to reach out to Sujal Choudhari at sjlchoudhari@gmail.com.

Thank you for contributing to Coda and helping us make it better for everyone! 
