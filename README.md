# nebulae

2023 Update:

This was my starting point for trying various operating system ideas over the years, kind of a snapshot of a hump
I never truly got over.

This code doesn't actually do much of anything that an actual OS would do, aside from laying some groundwork.

That being said, some goodies are there.  This actually does exit UEFI boot services.  

It wires up the circuitry to a decent level for those just getting started.  Exceptions & interrupt handling are present
and should work (if memory serves), but it has no real SMP support, does not do much in the way of ACPI table parsing,
and has no real memory manager (kernel or otherwise) to speak of.

But it does map a set of page tables, and the headers are all there (many are original), and it switches CR3 at least once.

I wanted to release a much better version with proper ACPI table parsing and some SMP ready code, but the publicly
available ACPI libraries were much more restrictively licensed 6-10 years ago, and it's hard to wrap one's head
aroud the mountain of documentation.

I stopped most of my OS dev when life got in the way, which tends to happen to the best of us.

Then there's the reality that doing yet another C-based OS isn't where it's at it 2023.  Maybe Rust, maybe Zig, maybe any
of the other languages out there.

Hacking around with the hardware was something I did variously since the early 90s, and it was always fun, usually
slightly frustrating, but incredibly educational.  Seeing how the sausage gets made helped me immensely, starting with
that first 8051 Intel breadboard with the hex keypad.  Our assembler was a pencil, a worksheet, and a table of opcodes.  
You can see where it goes from there :)

Maybe I'll get around to spending some time updating this in the near future, because it's around the time for that 
5-year itch again.  It comes like clockwork.  Each time I come back, I'm a much better developer, but that's not saying much.

If this helps you scratch your OS itch, I'm glad I could help.
 
Happy hacking!

N

----
License
----

2-Clause BSD
