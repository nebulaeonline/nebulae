# nebulae

nebulae is an experimental (minimal) operating system / kernel; it's goals, while lofty, are to be (one day):

  - modern&mdash; implement recent concepts in operating system research
  - modular&mdash; be easy to tinker with
  - ~~Magic~~ (it will not be magic)

## So what does it actually do so far?

A moderate bit of basic setup.  The memory subsystem is a mess.  The ideas I had back then were not well thought out.  However it does have the following features:

  - nasm assembly routines for most x64 system programming instructions you'd need all configured to call from C (or other lang supporting C FFI).
  - sets up all interrupt and exception vectors with decently commented code, and no expansion macros (with the exception of nasm, forgive me!)
  - there is basic skeleton code for MMIO and ACPI
  - it can map pages, for the most basic definition of "map pages"
  
## So what can't it do?

  - most things, lol :P
  - as mentioned above, the memory subsystem is a mess, but there are some good starting blocks
  - it does not switch to usermode
  - it does not unmap or clean up anything.  At all.
  - the code is messy in places, but I tried to comment as best I could

## What's the goal here?

The goal of this project is provide a learning / practice environment for systems-level programming on modern 64-bit architectures (first, x64, then AARCH64).

The environment aims to be beginner friendly&mdash; well, OS developer beginner friendly at least! The system is small, and easily hackable.

## Why all the Visual Studio stuff?

Well, the project files make it easy for someone to grab VS Community, clone the Github repo, and get started!  The project files can be used with MSVC or Clang with MSVC link.  The project itself is self-contained, including debugger (qemu).

Additionally, I have to give major kudos to Alex Ionescu for his excellent VisualUefi repo. 
 
### Todos

 - Start going fast -->
 
 Since this has gotten a bit of attention here and there, this readme was written with a bit more ambition towards this particular setup and kernel in mind.  Let me be clear: this is not a kernel;  it *is* however, a minimal starting ground to get going and hacking on your own.  In a way, that's kind of nice, and something I wish I had when I started many moons ago. Believe me, when you get started and you are tying to write bare metal code, the setup and the tooling is 90% of the problem.
 
 I really hope you enjoy.  And don't worry, there will be MUCH more to come on the topic of systems programming!
 
 Happy hacking!

License
----

2-Clause BSD
