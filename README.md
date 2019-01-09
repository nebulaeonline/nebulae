# nebulae

nebulae is an experimental operating system:

  - modern&mdash; implements recent concepts in operating system research
  - modular&mdash; it's easy to tinker with
  - ~~Magic~~

## What's the goal here?

The goal of this project is provide a learning / practice environment for systems-level programming on modern 64-bit architectures (first, x64, then AARCH64).

The environment aims to be beginner friendly&mdash; well, OS developer beginner friendly at least!; The system is small, and easily hackable.

## Why all the Visual Studio stuff?

Well, the project files make it easy for someone to grab VS Community, clone the Github repo, and get started!  The project files can be used with MSVC, Clang ~~with lld-link~~ (lld-link does not like the object files, even ones it created), or Clang with MSVC link.  The project itself is self-contained, including debugger (qemu).

Additionally, I have to give major kudos to Alex Ionescu for his excellent VisualUefi repo. 
 
### Todos

 - Write (WAY) MORE Code

License
----

3-Clause BSD
