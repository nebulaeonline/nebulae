# nebulae

nebulae is an experimental (minimal) operating system / kernel; it's goals, while lofty, are to be (one day):

  - modern&mdash; implement recent concepts in operating system research
  - modular&mdash; be easy to tinker with
  - ~~Magic~~ (it will not be magic)

## What's the goal here?

The goal of this project is provide a learning / practice environment for systems-level programming on modern 64-bit architectures (first, x64, then AARCH64).

The environment aims to be beginner friendly&mdash; well, OS developer beginner friendly at least! The system is small, and easily hackable.

## Why all the Visual Studio stuff?

Well, the project files make it easy for someone to grab VS Community, clone the Github repo, and get started!  The project files can be used with MSVC or Clang with MSVC link.  The project itself is self-contained, including debugger (qemu).

Additionally, I have to give major kudos to Alex Ionescu for his excellent VisualUefi repo. 
 
### Todos

 - Start going fast -->
 
 Since this has gotten a bit of attention here and there, this readme was written with a bit more ambition towards this particular setup and kernel in mind.  Let me be clear: this is not a kernel;  it *is* however, a minimal starting ground to get going and hacking on your own.  In a way, that's kind of nice, and something I wish I had when I started many moons ago.  
 
 I hope you enjoy.  And don't worry, there will be MUCH more to come on the topic of systems programming!

License
----

2-Clause BSD
