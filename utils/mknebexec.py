#!/usr/bin/python

import sys
from pathlib import Path

if len(sys.argv) == 1:
    print("Usage: mknebexec.py <filename.pexe>")
    sys.exit()

pexe_file = Path(sys.argv[1])

if not pexe_file.is_file():
    print(f"{sys.argv[1]} does not exist!")
    sys.exit()

# Validate input file
try:
    with pexe_file.open("r+b") as pexe:
        # Verify a few points with the input PE file
        # namely the PE signature, the machine type (arch),
        # that it is a PE32+ file, and that it was linked
        # as an EFI_APPLICATION
        pexe.seek(0x3c)
        pe_header_offset = int.from_bytes(pexe.read(4), byteorder='little')
        pexe.seek(pe_header_offset)
        pe_sig = pexe.read(4)
        if pe_sig != b"PE\x00\x00":
            print("PE signature not found!")
            sys.exit()
        pexe.seek(pe_header_offset + 0x04)
        pe_machine = int.from_bytes(pexe.read(2), byteorder='little')
        if pe_machine != 0x8664:
            print("nebulae only supports the x86-64 architecture!")
            sys.exit()
        pexe.seek(pe_header_offset + 0x18)
        pe_magic = int.from_bytes(pexe.read(2), byteorder='little')
        if pe_magic != 0x20b:
            print("nebulae only supports PE32+ (x86-64)!")
            sys.exit()
        pexe.seek(pe_header_offset + 0x5c)
        pe_subsystem = int.from_bytes(pexe.read(2), byteorder='little')
        if pe_subsystem != 0x0A:
            print("nebulae only supports SUBSYSTEM_EFI_APPLICATION executables!")
            sys.exit()
        
        # Go ahead and modify a few pieces of the PE file so
        # that Windows will not attempt to load it, and so
        # nebulae's loader can recognize this PE file as one
        # of its own
        pexe.seek(pe_header_offset + 0x40)
        pexe.write(b"\x34\x00\x19\x00")
        pexe.seek(pe_header_offset + 0x4c)
        pexe.write(b"\x19")
        pexe.seek(pe_header_offset + 0x58)
        pexe.write(b"\x69\x14")
        pexe.seek(pe_header_offset + 0x5c)
        pexe.write(b"\x69\x14")

except Exception as inst:
    print(type(inst))       # the exception instance
    print(inst.args)        # arguments stored in .args
    print(inst)             # __str__ allows args to be printed directly,
                            # but may be overridden in exception subclasses
    sys.exit()

print(f"Executable '{sys.argv[1]}' altered for execution on nebulae x86-64'")
sys.exit()
