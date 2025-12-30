# VSCode C++ Development Environment

A VSCode C++ development environment for Linux, richly populated with
quality-oriented tools.


## Motivation

VSCode users know that the application is highly expandable and flexible.
They also know that discovering all the tools and techniques to configure
VSCode is not easy. This repository presents a highly enriched VSCode
environment for C++ development on Linux.

The repository has a C++ statistics accumulator class and tests. The
statistics code is not really important. It serves to showcase the many
additions required to turn VSCode in to a quality-oriented C++ development
tool. Those many additions are the point of this repository. It might be
used as a template for new projects.


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

These instructions assume you are working on Linux with VSCode, git, and
docker installed. They also assume you have a GitHub account with registered
SSH keys so you can clone this repository and open it in VSCode.

```
git clone git@github.com:ian-fisher-developer/development-tools.git

cd development-tools

code .
```

VSCode recognizes the .devcontainer directory and offers to re-open the project
in the container. Or, click on `Open a Remote Window` (at the very left of the
status bar) and choose `Reopen in Container`. Note that, for the first time
only, it may take a while for VSCode to create the docker images.

### Unit Tests

- The CMake Tools extension has a build-kit selector in the status bar.
  Choose GCC 11.4.0. The extension runs the CMake configure step to the
  build/build-vscode-cmake-tools directory.
- Choose the `all` build target (in the status bar selector) to build the
  library, the tests, and the example programs. Open the test explorer side
  pane to view and run the unit tests. Set `Start Continuous Run` (at the top
  of the test explorer) to automatically run the tests when they are rebuilt.
  Note how the test explorer enables easy re-running of a single test. Note
  also that in the screen-shot above, the test explorer is moved to VSCode's
  secondary side bar, on the right.
- Open src/lib/StatisticsAccumulator.cpp from the file explorer. Note that the
  code editor has inline git blame and linter suggestions from clang-tidy.
  Ctrl-S auto-formats C++ and CMake files according to the project standards.
- Choose the `statistics_coverage_report` target and build it. Turn on the
  Coverage Gutters extension's `Watch` function (in the status bar) to add
  coverage annotation to the C++ code editor in the form of colored bars by
  the line numbers. Select `Coverage Gutters: Preview Coverage Report` from
  the editor's context menu to browse the HTML report.
- The environment provides custom tasks running mutation testing, and
  generating reports from valgrind, and the compiler's address sanitizer and
  undefined-behavior sanitizer.
  Select `Run Build Task` from VSCode's `Terminal` menu to execute the tasks.
