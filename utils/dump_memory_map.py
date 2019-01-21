#!/usr/bin/python
# Takes a VirtualBox memory dump (as an .elf file), and given the offset
# of the memory dump and the address of the uefi memory map, will dump
# the map.
#

import sys
import array as arr
from pathlib import Path

uefi_mem_descr_size = 0x30

if len(sys.argv) != 5:
    print(f"Usage: dump_memory_map.py <filename.elf> <memory_start_offset> <memory_map_address> <memory_map_size>")
    sys.exit()

elf_file = Path(sys.argv[1])

if not elf_file.is_file():
    print(f"{sys.argv[1]} does not exist!")
    sys.exit()

memory_offset = int(sys.argv[2])

if memory_offset == 0:
    print(f"Please enter a valid memory start offset!")
    sys.exit()

memory_map_address = int(sys.argv[3])

if memory_map_address == 0:
    print(f"Please enter a valid memory map address!")
    sys.exit()

memory_map_size = int(sys.argv[4])

if memory_map_size == 0:
    print(f"Pleae enter a valid memory map size!")
    sys.exit()

# Validate input file
try:
    with elf_file.open("rb") as elf:
        # at least verify this is an elf file
        elf_sig = elf.read(4)
        if elf_sig != b"\x7fELF":
            print(f"ELF signature not found!")
            sys.exit()

        for memmap_i in range(memory_map_size // uefi_mem_descr_size):
            # seek to the memory map
            elf.seek(memory_offset + memory_map_address + (memmap_i * uefi_mem_descr_size))

            memory_block_type = int.from_bytes(elf.read(8), byteorder='little')
            memory_block_physical_start = int.from_bytes(elf.read(8), byteorder='little')
            memory_block_virtual_start = int.from_bytes(elf.read(8), byteorder='little')
            memory_block_number_of_pages = int.from_bytes(elf.read(8), byteorder='little')
            memory_block_attribute = int.from_bytes(elf.read(8), byteorder='little')

            print(f"mem_region type: " + hex(memory_block_type) + " pstart: " + hex(memory_block_physical_start) + " vstart: " + hex(memory_block_virtual_start) + " pages: " + hex(memory_block_number_of_pages) + " attr: " + hex(memory_block_attribute))

except Exception as inst:
    print(type(inst))       # the exception instance
    print(inst.args)        # arguments stored in .args
    print(inst)             # __str__ allows args to be printed directly,
                            # but may be overridden in exception subclasses
    sys.exit()

sys.exit()
