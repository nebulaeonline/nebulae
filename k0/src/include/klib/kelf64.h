// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __K0_KELF64_H
#define __K0_KELF64_H

#include "../k0.h"

// Elf Data types
typedef UINT64 Elf64_Addr;
typedef UINT64 Elf64_Off;
typedef UINT16 Elf64_Half;
typedef UINT32 Elf64_Word;
typedef INT32  Elf64_Sword;
typedef UINT64 Elf64_Xword;
typedef INT64  Elf64_Sxword;

typedef struct s_Elf64_Ehdr {
    UINT8      e_ident[16];         // ELF identification
    Elf64_Half e_type;              // Object file type
    Elf64_Half e_machine;           // Machine type
    Elf64_Word e_version;           // Object file version
    Elf64_Addr e_entry;             // Entry point address
    Elf64_Off  e_phoff;             // Program header offset
    Elf64_Off  e_shoff;             // Section header offset
    Elf64_Word e_flags;             // Processor-specific flags
    Elf64_Half e_ehsize;            // ELF header size
    Elf64_Half e_phentsize;         // Size of program header entry
    Elf64_Half e_phnum;             // Number of program header entries
    Elf64_Half e_shentsize;         // Size of section header entry
    Elf64_Half e_shnum;             // Number of section header entries
    Elf64_Half e_shstrndx;          // Section name string table index
} Elf64_Ehdr;

// ELF Identification, e_ident
#define EI_MAG0         0           // File identification
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4           // File class
#define EI_DATA         5           // Data encoding
#define EI_VERSION      6           // File version
#define EI_OSABI        7           // OS/ABI identification
#define EI_ABIVERSION   8           // ABI version
#define EI_PAD          9           // Start of padding bytes
#define EI_NIDENT       16          // Size of e_ident[]

// Object File Classes, e_ident[EI_CLASS]
#define ELFCLASS32      1           // 32-bit objects
#define ELFCLASS64      2           // 64-bit objects

// Data Encodings, e_ident[EI_DATA]
#define ELFDATA2LSB     1           // Object file data structures are little endian
#define ELFDATA2MSB     2           // Object file data structures are big endian

// Operating System and ABI Identifiers, e_ident[EI_OSABI]
// From FreeBSD Master 2019-01-07
// https://github.com/freebsd/freebsd/blob/master/sys/sys/elf_common.h
#define    ELFOSABI_NONE        0        /* UNIX System V ABI */
#define    ELFOSABI_HPUX        1        /* HP-UX operating system */
#define    ELFOSABI_NETBSD        2        /* NetBSD */
#define    ELFOSABI_LINUX        3        /* GNU/Linux */
#define    ELFOSABI_HURD        4        /* GNU/Hurd */
#define    ELFOSABI_86OPEN        5        /* 86Open common IA32 ABI */
#define    ELFOSABI_SOLARIS    6        /* Solaris */
#define    ELFOSABI_AIX        7        /* AIX */
#define    ELFOSABI_IRIX        8        /* IRIX */
#define    ELFOSABI_FREEBSD    9        /* FreeBSD */
#define    ELFOSABI_TRU64        10        /* TRU64 UNIX */
#define    ELFOSABI_MODESTO    11        /* Novell Modesto */
#define    ELFOSABI_OPENBSD    12        /* OpenBSD */
#define    ELFOSABI_OPENVMS    13        /* Open VMS */
#define    ELFOSABI_NSK        14        /* HP Non-Stop Kernel */
#define    ELFOSABI_AROS        15        /* Amiga Research OS */
#define    ELFOSABI_FENIXOS    16        /* FenixOS */
#define    ELFOSABI_CLOUDABI    17        /* Nuxi CloudABI */
#define    ELFOSABI_ARM_AEABI    64        /* ARM EABI */
#define    ELFOSABI_ARM        97        /* ARM */
#define ELFOSABI_NEBULAE    254     /* nebulae */
#define    ELFOSABI_STANDALONE    255        /* Standalone (embedded) application */

#define    ELFOSABI_SYSV        ELFOSABI_NONE    /* symbol used in old spec */
#define    ELFOSABI_MONTEREY    ELFOSABI_AIX    /* Monterey */
#define    ELFOSABI_GNU        ELFOSABI_LINUX

// Object File Types, e_type
#define ET_NONE         0           // No file type
#define ET_REL          1           // Relocatable object file
#define ET_EXEC         2           // Executable file
#define ET_DYN          3           // Shared object file
#define ET_CORE         4           // Core file
#define ET_LOOS         0xFE00      // Environment-specific use
#define ET_HIOS         0xFEFF
#define ET_LOPROC       0xFF00      // Processor-specific use
#define ET_HIPROC       0xFFFF

// Special Section Indices
#define SHN_UNDEF       0           // Undefined or meaningless section reference
#define SHN_LOPROC      0xFF00      // Processor-specific use
#define SHN_HIPROC      0xFF1F      
#define SHN_LOOS        0xFF20      // Environment-specific use
#define SHN_HIOS        0xFF3F
#define SHN_ABS         0xFFF1      // The corresponding reference is an absolute value
#define SHN_COMMON      0xFFF2      // Has a symbol that has been declared as a common block

// Section Header
typedef struct s_Elf64_Shdr {
    Elf64_Word  sh_name;            // Section name
    Elf64_Word  sh_type;            // Section type
    Elf64_Xword sh_flags;           // Section attributes
    Elf64_Addr  sh_addr;            // Virtual address in memory
    Elf64_Off   sh_offset;          // Offset in file
    Elf64_Xword sh_size;            // Size of section
    Elf64_Word  sh_link;            // Link to other section
    Elf64_Word  sh_info;            // Misc Info
    Elf64_Xword sh_addralign;       // Address alignment boundary
    Elf64_Xword sh_entsize;         // Size of entries, if section has table
} Elf64_Shdr;

// Section Types, sh_type
#define SHT_NULL        0           // Marks an unused section header 
#define SHT_PROGBITS    1           // Contains information defined by the program
#define SHT_SYMTAB      2           // Contains a linker symbol table
#define SHT_STRTAB      3           // Contains a string table
#define SHT_RELA        4           // Contains "Rela" type relocation entries
#define SHT_HASH        5           // Contains a symbol hash table
#define SHT_DYNAMIC     6           // Contains dynamic linking tables
#define SHT_NOTE        7           // Contains note information
#define SHT_NOBITS      8           // Contains uninitialized space; does not occupy any space in the file
#define SHT_REL         9           // Contains "Rel" type relocation entries
#define SHT_SHLIB       10          // Reserved
#define SHT_DYNSYM      11          // Contains a dynamic loader symbol table
#define SHT_LOOS        0x60000000  // Environment-specific use
#define SHT_HIOS        0x6FFFFFFF
#define SHT_LOPROC      0x70000000  // Processor-specific use
#define SHT_HIPROC      0x7FFFFFFF

// Section Attributes, sh_flags
#define SHF_WRITE       0x01        // Section contains writable data
#define SHF_ALLOC       0x02        // Section is allocated in memory image of program
#define SHF_EXECINSTR   0x04        // Section contains executable instructions
#define SHF_MASKOS      0x0F000000  // Environment-specific use
#define SHF_MASKPROC    0xF0000000  // Processor-specific use

// Symbol Table Entry
typedef struct s_Elf64_Sym {
    Elf64_Word  st_name;            // Symbol name
    CHAR8       st_info;            // Type and Binding attributes
    CHAR8       st_other;           // Reserved
    Elf64_Half  st_shndx;           // Section table index
    Elf64_Addr  st_value;           // Symbol value
    Elf64_Xword st_size;            // Size of object (e.g., common)
} Elf64_Sym;

// Symbol Bindings
#define STB_LOCAL       0           // Not visible outside the object file
#define STB_GLOBAL      1           // Global symbol, visible to all object files
#define STB_WEAK        2           // Global scope, but with lower precedence than global symbols
#define STB_LOOS        10          // Environment-specific use
#define STB_HIOS        12
#define STB_LOPROC      13          // Processor-specific use
#define STB_HIPROC      15

// Symbol Types
#define STT_NOTYPE      0           // No type specified (e.g. an absolute symbol)
#define STT_OBJECT      1           // Data object
#define STT_FUNC        2           // Function entrypoint
#define STT_SECTION     3           // Symbol is associated with a section
#define STT_FILE        4           // Source file associated with the object file
#define STT_LOOS        10          // Environment-specific use
#define STT_HIOS        12
#define STT_LOPROC      13          // Processor-specific use
#define STT_HIPROC      15

// Relocations
typedef struct s_Elf64_Rel {
    Elf64_Addr  r_offset;           // Address of reference
    Elf64_Xword r_info;             // Symbol index and type of relocation
} Elf64_Rel;

typedef struct s_Elf64_Rela {
    Elf64_Addr   r_offset;          // Address of reference
    Elf64_Xword  r_info;            // Symbol index and type of relocation
    Elf64_Sxword r_addend;          // Constant part of expression
} Elf64_Rela;

// Relocation Macros
#define ELF64_R_SYM(i)      ((i) >> 32)
#define ELF64_R_TYPE(i)     ((i) & 0xFFFFFFFFL)
#define ELF64_R_INFO(s, t)  (((s) << 32) + ((t) & 0xFFFFFFFFL))

// Program Header Table
typedef struct s_Elf64_Phdr {
    Elf64_Word  p_type;             // Type of segment
    Elf64_Word  p_flags;            // Segment attributes
    Elf64_Off   p_offset;           // Offset in file
    Elf64_Addr  p_vaddr;            // Virtual address in memory
    Elf64_Addr  p_paddr;            // Reserved
    Elf64_Xword p_filesz;           // Size of segment in file
    Elf64_Xword p_memsz;            // Size of segment in memory
    Elf64_Xword p_align;            // Alignment of segment
} Elf64_Phdr;

// Segment Types, p_type
#define PT_NULL         0           // Unused entry
#define PT_LOAD         1           // Loadable segment
#define PT_DYNAMIC      2           // Dynamic linking tables
#define PT_INTERP       3           // Program interpreter path name
#define PT_NOTE         4           // Note sections
#define PT_SHLIB        5           // Reserved
#define PT_PHDR         6           // Program header table
#define PT_LOOS         0x60000000  // Environment-specific use
#define PT_HIOS         0x6FFFFFFF
#define PT_LOPROC       0x70000000  // Processor-specific use
#define PT_HIPROC       0x7FFFFFFF

// Segment Attributes, p_flags
#define PF_X            0x1         // Execute permission
#define PF_W            0x2         // Write permission
#define PF_R            0x4         // Read permission
#define PF_MASKOS       0x00FF0000  // Reserved for environment-specific use
#define PF_MASKPROC     0xFF000000  // Reserved for processor-specific use

// Dynamic Table
typedef struct s_Elf64_Dyn {
    Elf64_Sxword d_tag;
    union {
        Elf64_Xword d_val;
        Elf64_Addr  d_ptr;
    } d_un;
} Elf64_Dyn;

extern Elf64_Dyn _DYNAMIC[];

// Dynamic Table Entries
#define DT_NULL         0           // Marks the end of the dynamic array
#define DT_NEEDED       1           // The string table offset of the name of a needed library
#define DT_PLTRELSZ     2           // Total size, in bytes, of the relocation entries associated with the procedure linkage table
#define DT_PLTGOT       3           // Contains an address associated with the linkage table. The specific meaning of this field is processor-dependent
#define DT_HASH         4           // Address of the symbol hash table
#define DT_STRTAB       5           // Address of the dynamic string table
#define DT_SYMTAB       6           // Address of the dynamic symbol table
#define DT_RELA         7           // Address of a relocation table with Elf64_Rela entries
#define DT_RELASZ       8           // Total size, in bytes, of the DT_RELA relocation table
#define DT_RELAENT      9           // Size, in bytes, of each DT_RELA relocation entry
#define DT_STRSZ        10          // Total size, in bytes, of the string table
#define DT_SYMENT       11          // Size, in bytes, of each symbol table entry
#define DT_INIT         12          // Address of the initialization function
#define DT_FINI         13          // Address of the termination function
#define DT_SONAME       14          // The string table offset of the name of this shared object
#define DT_RPATH        15          // The string table offset of a shared library search path string
// The presence of this dynamic table entry modifies the symbol resolution slgorithm for references 
// within the library.  Symbols defined within the library are used to resolve references before 
// the dynamic linker searches the usual search path:
#define DT_SYMBOLIC     16
#define DT_REL          17          // Address of a relocation table with Elf64_Rel entries
#define DT_RELSZ        18          // Total size, in bytes, of the DT_REL relocation table
#define DT_RELENT       19          // Size, in bytes, of each DT_REL relocation entry
#define DT_PLTREL       20          // Type of relocation entry used for the procedure linkage table.  The d_val member contains either DT_REL or DT_RELA.
#define DT_DEBUG        21          // Reserved for debugger use
#define DT_TEXTREL      22          // Signals that the relocation table contains relocations for a non-writable segment
#define DT_JMPREL       23          // Address of the relocations associated with the procedure linkage table
#define DT_BIND_NOW     24          // Signals that the dynamic loader should process all relocations for this object before transferring control to the program
#define DT_INIT_ARRAY   25          // Pointer to an array of pointers to initialization functions
#define DT_FINI_ARRAY   26          // Pointer to an array of pointers to termination functions
#define DT_INIT_ARRAYSZ 27          // Size, in bytes, of the array of initialization functions
#define DT_FINI_ARRAYSZ 28          // Size, in bytes, of the array of termination functions
#define DT_LOOS         0x60000000  // Environment-specific use
#define DT_HIOS         0x6FFFFFFF
#define DT_LOPROC       0x70000000  // Processor-specific use
#define DT_HIPROC       0x7FFFFFFF

UINT64 elf64_hash(CONST CHAR8* name);

#endif // __K0_KELF64_H
