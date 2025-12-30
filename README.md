# VSCode C++ Development Environment

A VSCode C++ development environment for Linux, richly populated with
quality-oriented tools.

This repository has a C++ statistics accumulator class and tests. The
statistics code is not really important. It serves to showcase the many
additions required to turn VSCode in to a rich C++ development tool.
Those many additions are the point of this repository. It might be used
as a template for new projects.


## Overview

The developmment enviroonment is provided via two directories:

- .devcontainer

  A VSCode containerized x86 development installlation based on Ubuntu.
  The developer works in the environment under their own account.

- .vscode

  VSCode settings, CMake variants, and custom tasks for C++ development.

The directories complement each other. The .vscode files provide settings
for the .devcontainer tools.

The resulting VSCode session has many integrated tools:

- CMake tools with configuration pre-sets and build-target selection
- Auto-format on save (C++ and CMake files)
- Inline linter hints and git blame
- Test explorer and code coverage annotation
- Mutation testing
- Valgrind reports and code sanitizer reports

Read on below for detailed instructions in using the environment.

![VSCode screenshot showing the C++ developer environment](VSCode.jpeg)


## Use The Environment
