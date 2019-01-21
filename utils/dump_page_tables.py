#!/usr/bin/python
# Takes a VirtualBox memory dump (as an .elf file), and given the offset
# of the memory dump, the cr3 value and the location of the new page
# tables, will dump all of them from the memory dump.
#
# These items can be found like so:
# Start VirtualBox to be able to create a memory dump:
#   VirtualBox --dbg --startvm nebulae
# Create the memory dump at the appropriate time:
#   VBoxManage debugvm "nebulae" dumpvmcore --filename nebdump.elf
# Find the offset of the elf64 section that contains the memory dump: 
#   objdump -h nebdump.elf | egrep -w "(Idx|load1)"
# CR3 and the new page table location is provided by nebulae and/or
# Vbox.log, depending on what you're doing.  You could also just dump
# it to screen before panicing

import sys
import array as arr
from pathlib import Path

if len(sys.argv) != 4:
    print(f"Usage: dump_page_tables.py <filename.elf> <memory_start_offset> <cr3>")
    sys.exit()

elf_file = Path(sys.argv[1])

if not elf_file.is_file():
    print(f"{sys.argv[1]} does not exist!")
    sys.exit()

memory_offset = int(sys.argv[2])

if memory_offset == 0:
    print(f"Please enter a valid memory start offset!")
    sys.exit()

cr3 = int(sys.argv[3])

if cr3 == 0:
    print(f"Please enter a valid value for the CR3 register!")
    sys.exit()

pml4_table = arr.array('L')

x64_4KB_ALIGNMENT = 0x000FFFFFFFFFF000

# Validate input file
try:
    with elf_file.open("rb") as elf:
        # at least verify this is an elf file
        elf_sig = elf.read(4)
        if elf_sig != b"\x7fELF":
            print(f"ELF signature not found!")
            sys.exit()

        # dump the pml4 table
        for pml4_i in range(512):
            # seek to the proper pml4 entry
            elf.seek(memory_offset + (cr3 + (pml4_i * 8)))
            pml4_table.append(int.from_bytes(elf.read(8), byteorder='little'))
            print(f"PML4[{pml4_i}] == " + hex(pml4_table[pml4_i]))

            # if this pml4 table is empty, we're done
            if pml4_table[pml4_i] == 0:
                continue

            # each pdpt has 512 entries
            pdpt = arr.array('L')
            for pdpt_i in range(512):
                # seek to the pdpte this pml4 entry points to
                elf.seek(memory_offset + (pml4_table[pml4_i] & x64_4KB_ALIGNMENT) + (pdpt_i * 8))
                
                pdpt.append(int.from_bytes(elf.read(8), byteorder='little'))
                print(f"PDPT[{pdpt_i}] == " + hex(pdpt[pdpt_i]))

                # see if this points to a 1GB page
                if (pdpt[pdpt_i] & 0x80) == 0x80:
                    print(f"1GB Page Table Found @ {pdpt_i} == " + hex(pdpt[pdpt_i]))
                    continue
                
                # if this pdpt is empty, we're done
                if pdpt[pdpt_i] == 0:
                    continue

                # each pd has 512 entries
                pd = arr.array("L")
                for pd_i in range(512):
                    # seek to the page directory this pdpt entry points to
                    elf.seek(memory_offset + (pdpt[pdpt_i] & x64_4KB_ALIGNMENT) + (pd_i * 8))    
                    
                    pd.append(int.from_bytes(elf.read(8), byteorder='little'))
                    print(f"PD[{pd_i}] == " + hex(pd[pd_i]))

                    # see if this pde points to a page table
                    if (pd[pd_i] & 0x80) == 0x80:
                        print(f"2MB Page Table Found @ {pd_i} == " + hex(pd[pd_i]))
                        continue

                    # jump to the page table this page directory entry points to
                    elf.seek(memory_offset + (pd[pd_i] & x64_4KB_ALIGNMENT))

                    # print each of the page table entries
                    pt = arr.array("L")
                    for pt_i in range(512):
                        pt.append(int.from_bytes(elf.read(8), byteorder='little'))
                        print(f"4KB Page Table Found @ {pt_i} == " + hex(pt[pt_i]))

except Exception as inst:
    print(type(inst))       # the exception instance
    print(inst.args)        # arguments stored in .args
    print(inst)             # __str__ allows args to be printed directly,
                            # but may be overridden in exception subclasses
    sys.exit()

sys.exit()
