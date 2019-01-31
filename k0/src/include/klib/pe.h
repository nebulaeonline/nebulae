// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
//
//   https://docs.microsoft.com/en-us/windows/desktop/debug/pe-format
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

#ifndef __K0_PE_H
#define __K0_PE_H

#include "../k0.h"

#include <Library/ShellLib.h>

#define PE_SIGNATURE                                    0x00004550

// Machine types
#define PE_FILE_MACHINE_UNKNOWN                      0x0000  // Unknown machine type
#define PE_FILE_MACHINE_AM33                         0x01D3  // Matsushita AM33
#define PE_FILE_MACHINE_AMD64                        0x8664  // x64
#define PE_FILE_MACHINE_ARM                          0x01C0  // ARM le
#define PE_FILE_MACHINE_ARM64                        0xAA64  // ARM64 le
#define PE_FILE_MACHINE_ARMNT                        0x01C4  // ARM Thumb-2 le
#define PE_FILE_MACHINE_EBC                          0x0EBC  // EFI byte code
#define PE_FILE_MACHINE_I386                         0x014C  // Intel 386+
#define PE_FILE_MACHINE_IA64                         0x0200  // Intel Itanium processor family
#define PE_FILE_MACHINE_M32R                         0x9041  // Mitsubishi M32R le
#define PE_FILE_MACHINE_MIPS16                       0x0266  // MIPS16
#define PE_FILE_MACHINE_MIPSFPU                      0x0366  // MIPS with FPU
#define PE_FILE_MACHINE_MIPSFPU16                    0x0466  // MIPS16 with FPU
#define PE_FILE_MACHINE_POWERPC                      0x01F0  // Power PC le
#define PE_FILE_MACHINE_POWERPCFP                    0x01F1  // Power PC with floating point support
#define PE_FILE_MACHINE_R4000                        0x0166  // MIPS le
#define PE_FILE_MACHINE_RISCV32                      0x5032  // RISC-V 32-bit address space
#define PE_FILE_MACHINE_RISCV64                      0x5064  // RISC-V 64-bit address space
#define PE_FILE_MACHINE_RISCV128                     0x5128  // RISC-V 128-bit address space
#define PE_FILE_MACHINE_SH3                          0x01A2  // Hitachi SH3
#define PE_FILE_MACHINE_SH3DSP                       0x01A3  // Hitachi SH3 DSP
#define PE_FILE_MACHINE_SH4                          0x01A6  // Hitachi SH4
#define PE_FILE_MACHINE_SH5                          0x01A8  // Hitachi SH5
#define PE_FILE_MACHINE_THUMB                        0x01C2  // Thumb
#define PE_FILE_MACHINE_WCEMIPSV2                    0x0169  // MIPS le WCE v2

// Characteristics

// Image only, Windows CE, and Microsoft Windows NT and later. 
// This indicates that the file does not contain base relocations 
// and must therefore be loaded at its preferred base address. 
// If the base address is not available, the loader reports an 
// error. The default behavior of the linker is to strip base 
// relocations from executable(EXE) files.
#define PE_FILE_RELOCS_STRIPPED                      0x0001
// Image only. This indicates that the image file is valid and 
// can be run. If this flag is not set, it indicates a linker error.
#define PE_FILE_EXECUTABLE_IMAGE                     0x0002
// COFF line numbers have been removed. This flag is deprecated and should be zero.
#define PE_FILE_LINE_NUMS_STRIPPED                   0x0004
// COFF symbol table entries for local symbols have been removed. 
// This flag is deprecated and should be zero.
#define PE_FILE_LOCAL_SYMS_STRIPPED                  0x0008
// Obsolete. Aggressively trim working set. This flag is deprecated 
// for Windows 2000 and later and must be zero.
#define PE_FILE_AGGRESSIVE_WS_TRIM                   0x0010
// Application can handle > 2-GB addresses.
#define PE_FILE_LARGE_ADDRESS_AWARE                  0x0020
// This flag is reserved for future use.
#define PE_FILE_RESERVED_FUTURE                      0x0040
// Little endian : the least significant bit(LSB) precedes the most 
// significant bit (MSB) in memory. 
// This flag is deprecated and should be zero.
#define PE_FILE_BYTES_REVERSED_LO                    0x0080
// Machine is based on a 32-bit-word architecture.
#define PE_FILE_32BIT_MACHINE                        0x0100
// Debugging information is removed from the image file.
#define PE_FILE_DEBUG_STRIPPED                       0x0200
// If the image is on removable media, fully load it and copy it to the swap file.
#define PE_FILE_REMOVABLE_RUN_FROM_SWAP              0x0400
// If the image is on network media, fully load it and copy it to the swap file.
#define PE_FILE_NET_RUN_FROM_SWAP                    0x0800
// The image file is a system file, not a user program.
#define PE_FILE_SYSTEM                               0x1000
// The image file is a dynamic-link library (DLL). Such files are 
// considered executable files for almost all purposes, although 
// they cannot be directly run.
#define PE_FILE_DLL                                  0x2000
// The file should be run only on a uniprocessor machine.
#define PE_FILE_UP_SYSTEM_ONLY                       0x4000
// Big endian: the MSB precedes the LSB in memory. 
// This flag is deprecated and should be zero.
#define PE_FILE_BYTES_REVERSED_HI                    0x8000

// PE magic
#define PE_MAGIC_32                                     0x10B
#define PE_MAGIC_64                                     0x20B

// Image subsystems
#define PE_SUBSYSTEM_UNKNOWN                         0       // An unknown subsystem
#define PE_SUBSYSTEM_NATIVE                          1       // Device drivers and native Windows processes
#define PE_SUBSYSTEM_WINDOWS_GUI                     2       // The Windows graphical user interface(GUI) subsystem
#define PE_SUBSYSTEM_WINDOWS_CUI                     3       // The Windows character subsystem
#define PE_SUBSYSTEM_OS2_CUI                         5       // The OS / 2 character subsystem
#define PE_SUBSYSTEM_POSIX_CUI                       7       // The Posix character subsystem
#define PE_SUBSYSTEM_NATIVE_WINDOWS                  8       // Native Win9x driver
#define PE_SUBSYSTEM_WINDOWS_CE_GUI                  9       // Windows CE
#define PE_SUBSYSTEM_EFI_APPLICATION                 10      // An Extensible Firmware Interface(EFI) application
#define PE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER         11      // An EFI driver with boot services
#define PE_SUBSYSTEM_EFI_RUNTIME_DRIVER              12      // An EFI driver with run - time services
#define PE_SUBSYSTEM_EFI_ROM                         13      // An EFI ROM image
#define PE_SUBSYSTEM_XBOX                            14      // XBOX
#define PE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION        16      // Windows boot application.

// DLL characteristics

// Image can handle a high entropy 64-bit virtual address space.
#define PE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA        0x0020
// DLL can be relocated at load time.
#define PE_DLLCHARACTERISTICS_DYNAMIC_BASE           0x0040
// Code Integrity checks are enforced.
#define PE_DLLCHARACTERISTICS_FORCE_INTEGRITY        0x0080
// Image is NX compatible.
#define PE_DLLCHARACTERISTICS_NX_COMPAT              0x0100
// Isolation aware, but do not isolate the image.
#define PE_DLLCHARACTERISTICS_NO_ISOLATION           0x0200
// Does not use structured exception(SE) handling. No SE handler 
// may be called in this image.
#define PE_DLLCHARACTERISTICS_NO_SEH                 0x0400
// Do not bind the image.
#define PE_DLLCHARACTERISTICS_NO_BIND                0x0800
// Image must execute in an AppContainer.
#define PE_DLLCHARACTERISTICS_APPCONTAINER           0x1000
// A WDM driver.
#define PE_DLLCHARACTERISTICS_WDM_DRIVER             0x2000
// Image supports Control Flow Guard.
#define PE_DLLCHARACTERISTICS_GUARD_CF               0x4000
// Terminal Server aware.
#define PE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE  0x8000

// COFF header struct
typedef PACKED_MS struct s_pe_coff_file_header {
    UINT16  machine;
    UINT16  number_of_sections;
    UINT32  time_date_stamp;
    UINT32  symbol_table_ptr;
    UINT32  number_of_symbols;
    UINT16  optional_header_size;
    UINT16  characteristics;
} PACKED_GNU pe_coff_file_header;

// Image header 
#define PE32_STANDARD_FIELDS_SIZE                       28
#define PE64_STANDARD_FIELDS_SIZE                       24

#define PE32_WINDOWS_FIELDS_SIZE                        68
#define PE64_WINDOWS_FIELDS_SIZE                        88

typedef PACKED_MS struct s_pe32_coff_PE_header {
    UINT16  magic;
    UINT8   major_linker_version;
    UINT8   minor_linker_version;
    UINT32  size_of_code;
    UINT32  size_of_initialized_data;
    UINT32  entrypoint_addr;
    UINT32  base_of_code;
    UINT32  base_of_data;
} PACKED_GNU pe32_coff_PE_header;

typedef PACKED_MS struct s_pe64_coff_PE_header {
    UINT16  magic;
    UINT8   major_linker_version;
    UINT8   minor_linker_version;
    UINT32  size_of_code;
    UINT32  size_of_initialized_data;
    UINT32  entrypoint_addr;
    UINT32  base_of_code;
} PACKED_GNU pe64_coff_PE_header;

typedef PACKED_MS struct s_pe32_coff_windows_fields {
    UINT32  PE_base;
    UINT32  section_alignment;
    UINT32  file_alignment;
    UINT16  major_os_version;
    UINT16  minor_os_version;
    UINT16  major_PE_version;
    UINT16  minor_PE_version;
    UINT16  major_subsystem_version;
    UINT16  minor_subsystem_version;
    UINT32  win32_version;
    UINT32  size_of_image;
    UINT32  size_of_headers;
    UINT32  checksum;
    UINT16  subsystem;
    UINT16  dll_characteristics;
    UINT32  size_of_stack_reserve;
    UINT32  size_of_stack_commit;
    UINT32  size_of_heap_reserve;
    UINT32  size_of_heap_commit;
    UINT32  loader_flags;
    UINT32  number_of_rva_and_sizes;
} PACKED_GNU pe32_coff_windows_fields;

typedef PACKED_MS struct s_pe64_coff_windows_fields {
    UINT64  PE_base;
    UINT32  section_alignment;
    UINT32  file_alignment;
    UINT16  major_os_version;
    UINT16  minor_os_version;
    UINT16  major_PE_version;
    UINT16  minor_PE_version;
    UINT16  major_subsystem_version;
    UINT16  minor_subsystem_version;
    UINT32  win32_version;
    UINT32  size_of_image;
    UINT32  size_of_headers;
    UINT32  checksum;
    UINT16  subsystem;
    UINT16  dll_characteristics;
    UINT64  size_of_stack_reserve;
    UINT64  size_of_stack_commit;
    UINT64  size_of_heap_reserve;
    UINT64  size_of_heap_commit;
    UINT32  loader_flags;
    UINT32  number_of_rva_and_sizes;
} PACKED_GNU pe64_coff_windows_fields;

typedef PACKED_MS struct s_pe_coff_PE_data_dir_ptr {
    UINT32  virtual_address;
    UINT32  size;
} PACKED_GNU pe_coff_PE_data_dir_ptr;

typedef PACKED_MS struct s_pe_coff_PE_data_dir {
    pe_coff_PE_data_dir_ptr  export_table;
    pe_coff_PE_data_dir_ptr  import_table;
    pe_coff_PE_data_dir_ptr  resource_table;
    pe_coff_PE_data_dir_ptr  exception_table;
    pe_coff_PE_data_dir_ptr  certificate_table;
    pe_coff_PE_data_dir_ptr  base_relocation_table;
    pe_coff_PE_data_dir_ptr  debug_data;
    pe_coff_PE_data_dir_ptr  architecture_reserved;  // must be zero
    pe_coff_PE_data_dir_ptr  global_ptr;             // must be zero
    pe_coff_PE_data_dir_ptr  thread_local_storage_table;
    pe_coff_PE_data_dir_ptr  load_config_table;
    pe_coff_PE_data_dir_ptr  bound_import;
    pe_coff_PE_data_dir_ptr  import_address_table;
    pe_coff_PE_data_dir_ptr  delay_import_descriptor;
    pe_coff_PE_data_dir_ptr  clr_runtime_header;
    pe_coff_PE_data_dir_ptr  reserved;               // must be zero
} PACKED_GNU pe_coff_PE_data_dir;

// Image sections

// Section flags
// The section should not be padded to the next boundary. This flag 
// is obsolete and is replaced by PE_SCN_ALIGN_1BYTES. This is valid 
// only for object files.
#define PE_SCN_TYPE_NO_PAD                   0x00000008
// The section contains executable code.
#define PE_SCN_CNT_CODE                      0x00000020
// The section contains initialized data.
#define PE_SCN_CNT_INITIALIZED_DATA          0x00000040
// The section contains uninitialized data.
#define PE_SCN_CNT_UNINITIALIZED_ DATA       0x00000080
// The section contains uninitialized data.
#define PE_SCN_LNK_OTHER                     0x00000100
// The section contains comments or other information. The directve section 
// has this type.
// This is valid for object files only.
#define PE_SCN_LNK_INFO                      0x00000200
// The section will not become part of the image.This is valid only for object files.
#define PE_SCN_LNK_REMOVE                    0x00000800
// The section contains COMDAT data.For more information, see COMDAT Sections (Object Only).
// This is valid only for object files.
#define PE_SCN_LNK_COMDAT                    0x00001000
// The section contains data referenced through the global pointer(GP).
#define PE_SCN_GPREL                         0x00008000
// Reserved for future use.
#define PE_SCN_MEM_PURGEABLE                 0x00020000
// Reserved for future use.
#define PE_SCN_MEM_16BIT                     0x00020000
// Reserved for future use.
#define PE_SCN_MEM_LOCKED                    0x00040000
// Reserved for future use.
#define PE_SCN_MEM_PRELOAD                   0x00080000
// Align data on a 1-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_1BYTES                  0x00100000
// Align data on a 2-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_2BYTES                  0x00200000
// Align data on a 4-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_4BYTES                  0x00300000
// Align data on an 8-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_8BYTES                  0x00400000
// Align data on a 16-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_16BYTES                 0x00500000
// Align data on a 32 - byte boundary.Valid only for object files.
#define PE_SCN_ALIGN_32BYTES                 0x00600000
// Align data on a 64-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_64BYTES                 0x00700000
// Align data on a 128-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_128BYTES                0x00800000
// Align data on a 256 - byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_256BYTES                0x00900000
// Align data on a 512-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_512BYTES                0x00A00000
// Align data on a 1024-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_1024BYTES               0x00B00000
// Align data on a 2048-byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_2048BYTES               0x00C00000
// Align data on a 4096 - byte boundary.Valid only for object files.
#define PE_SCN_ALIGN_4096BYTES               0x00D00000
// Align data on an 8192 - byte boundary. Valid only for object files.
#define PE_SCN_ALIGN_8192BYTES               0x00E00000
// The section contains extended relocations.
#define PE_SCN_LNK_NRELOC_OVFL               0x01000000
// The section can be discarded as needed.
#define PE_SCN_MEM_DISCARDABLE               0x02000000
// The section cannot be cached.
#define PE_SCN_MEM_NOT_CACHED                0x04000000
// The section is not pageable.
#define PE_SCN_MEM_NOT_PAGED                 0x08000000
// The section can be shared in memory.
#define PE_SCN_MEM_SHARED                    0x10000000
// The section can be executed as code.
#define PE_SCN_MEM_EXECUTE                   0x20000000
// The section can be read.
#define PE_SCN_MEM_READ                      0x40000000
// The section can be written to.
#define PE_SCN_MEM_WRITE                     0x80000000 

// Section header
typedef PACKED_MS struct s_pe_coff_section_header {
    CHAR8   name[8];
    UINT32  virtual_size;
    UINT32  virtual_addr;
    UINT32  size_of_raw_data;
    UINT32  pointer_to_raw_data;
    UINT32  pointer_to_relocations;
    UINT32  pointer_to_line_numbers;
    UINT16  number_of_relocations;
    UINT16  number_of_line_numbers;
    UINT32  characteristics;
} PACKED_GNU pe_coff_section_header;

// Relocations

// Relocation Types

// x64
// The relocation is ignored.
#define PE_REL_AMD64_ABSOLUTE                        0x0000
// The 64-bit VA of the relocation target.
#define PE_REL_AMD64_ADDR64                          0x0001
// The 32-bit VA of the relocation target.
#define PE_REL_AMD64_ADDR32                          0x0002
// The 32-bit address without an image base(RVA).
#define PE_REL_AMD64_ADDR32NB                        0x0003
// The 32-bit relative address from the byte following the relocation.
#define PE_REL_AMD64_REL32                           0x0004
// The 32-bit address relative to byte distance 1 from the relocation.
#define PE_REL_AMD64_REL32_1                         0x0005
// The 32-bit address relative to byte distance 2 from the relocation.
#define PE_REL_AMD64_REL32_2                         0x0006
// The 32-bit address relative to byte distance 3 from the relocation.
#define PE_REL_AMD64_REL32_3                         0x0007
// The 32-bit address relative to byte distance 4 from the relocation.
#define PE_REL_AMD64_REL32_4                         0x0008
// The 32-bit address relative to byte distance 5 from the relocation.
#define PE_REL_AMD64_REL32_5                         0x0009
// The 16-bit section index of the section that contains the target. This is 
// used to support debugging information.
#define PE_REL_AMD64_SECTION                         0x000A
// The 32-bit offset of the target from the beginning of its section. This is 
// used to support debugging information and static thread local storage.
#define PE_REL_AMD64_SECREL                          0x000B
// A 7-bit unsigned offset from the base of the section that contains the target.
#define PE_REL_AMD64_SECREL7                         0x000C
// CLR tokens.
#define PE_REL_AMD64_TOKEN                           0x000D
// A 32-bit signed span - dependent value emitted into the object.
#define PE_REL_AMD64_SREL32                          0x000E
// A pair that must immediately follow every span-dependent value.
#define PE_REL_AMD64_PAIR                            0x000F
// A 32-bit signed span - dependent value that is applied at link time.
#define PE_REL_AMD64_SSPAN32                         0x0010

// ARM64
// The relocation is ignored.
#define PE_REL_ARM64_ABSOLUTE                        0x0000
// The 32-bit VA of the target.
#define PE_REL_ARM64_ADDR32                          0x0001
// The 32-bit RVA of the target.
#define PE_REL_ARM64_ADDR32NB                        0x0002
// The 26-bit relative displacement to the target, for B and BL instructions.
#define PE_REL_ARM64_BRANCH26                        0x0003
// The page base of the target, for ADRP instruction.
#define PE_REL_ARM64_PAGEBASE_REL21                  0x0004
// The 12-bit relative displacement to the target, for instruction ADR
#define PE_REL_ARM64_REL21                           0x0005
// The 12-bit page offset of the target, for instructions ADD / ADDS 
// (immediate) with zero shift.
#define PE_REL_ARM64_PAGEOFFSET_12A                  0x0006
// The 12-bit page offset of the target, for instruction LDR (indexed, unsigned immediate).
#define PE_REL_ARM64_PAGEOFFSET_12L                  0x0007
// The 32-bit offset of the target from the beginning of its section. This is used to support 
// debugging information and static thread local storage.
#define PE_REL_ARM64_SECREL                          0x0008
// Bit 0:11 of section offset of the target, for instructions ADD / ADDS 
// (immediate) with zero shift.
#define PE_REL_ARM64_SECREL_LOW12A                   0x0009
// Bit 12 : 23 of section offset of the target, for instructions ADD / ADDS 
// (immediate) with zero shift.
#define PE_REL_ARM64_SECREL_HIGH12A                  0x000A
// Bit 0 : 11 of section offset of the target, for instruction LDR 
// (indexed, unsigned immediate).
#define PE_REL_ARM64_SECREL_LOW12L                   0x000B
// CLR token.
#define PE_REL_ARM64_TOKEN                           0x000C
// The 16-bit section index of the section that contains the target. 
// This is used to support debugging information.
#define PE_REL_ARM64_SECTION                         0x000D
// The 64-bit VA of the relocation target.
#define PE_REL_ARM64_ADDR64                          0x000E
// The 19-bit offset to the relocation target, for conditional B instruction.
#define PE_REL_ARM64_BRANCH19                        0x000F
// The 14-bit offset to the relocation target, for instructions TBZ and TBNZ.
#define PE_REL_ARM64_BRANCH14                        0x0010
// The 32-bit relative address from the byte following the relocation.
#define PE_REL_ARM64_REL32                           0x0011

// i386
// The relocation is ignored.
#define PE_REL_I386_ABSOLUTE                         0x0000
// Not supported.
#define PE_REL_I386_DIR16                            0x0001
// Not supported.
#define PE_REL_I386_REL16                            0x0002
// The target's 32-bit VA. 
#define PE_REL_I386_DIR32                            0x0006
// The target's 32-bit RVA. 
#define PE_REL_I386_DIR32NB                          0x0007
// Not supported.
#define PE_REL_I386_SEG12                            0x0009
// The 16-bit section index of the section that contains the target. 
// This is used to support debugging information.
#define PE_REL_I386_SECTION                          0x000A
// The 32-bit offset of the target from the beginning of its section.
// This is used to support debugging information and static thread local storage.
#define PE_REL_I386_SECREL                           0x000B
// The CLR token.
#define PE_REL_I386_TOKEN                            0x000C
// A 7-bit offset from the base of the section that contains the target.
#define PE_REL_I386_SECREL7                          0x000D
// The 32-bit relative displacement to the target. This supports the x86 
// relative branch and call instructions.
#define PE_REL_I386_REL32                            0x0014

// Section number values
// The symbol record is not yet assigned a section.A value of zero indicates that 
// a reference to an external symbol is defined elsewhere. A value of non-zero is 
// a common symbol with a size that is specified by the value.
#define PE_SYM_UNDEFINED                             0
// The symbol has an absolute(non-relocatable) value and is not an address.
#define PE_SYM_ABSOLUTE                              -1
// The symbol provides general type or debugging information but does not correspond 
// to a section. Microsoft tools use this setting along with .file records (storage 
// class FILE). 
#define PE_SYM_DEBUG                                 -2

// Masks
#define PE_BASE_TYPE_MASK                               0x0F
#define PE_COMPLEX_TYPE_MASK                            0xF0

// Base symbol types
// No type information or unknown base type.
// Microsoft tools use this setting
#define PE_SYM_TYPE_NULL                             0
// No valid type; used with void pointers and functions
#define PE_SYM_TYPE_VOID                             1
// A character(signed byte)
#define PE_SYM_TYPE_CHAR                             2
// A 2-byte signed integer
#define PE_SYM_TYPE_SHORT                            3
// A natural integer type(normally 4 bytes in Windows)
#define PE_SYM_TYPE_INT                              4
// A 4-byte signed integer
#define PE_SYM_TYPE_LONG                             5
// A 4-byte floating-point number
#define PE_SYM_TYPE_FLOAT                            6
// An 8-byte floating-point number
#define PE_SYM_TYPE_DOUBLE                           7
// A structure
#define PE_SYM_TYPE_STRUCT                           8
// A union
#define PE_SYM_TYPE_UNION                            9
// An enumerated type
#define PE_SYM_TYPE_ENUM                             10
// A member of enumeration(a specific value)
#define PE_SYM_TYPE_MOE                              11
// A byte; unsigned 1-byte integer                      
#define PE_SYM_TYPE_BYTE                             12
// A word; unsigned 2-byte integer
#define PE_SYM_TYPE_WORD                             13
// An unsigned integer of natural size(normally, 4 bytes)
#define PE_SYM_TYPE_UINT                             14
// An unsigned 4-byte integer
#define PE_SYM_TYPE_DWORD                            15

// Complex symbol types
// No derived type; the symbol is a simple scalar variable.
#define PE_SYM_DTYPE_NULL                            0
// The symbol is a pointer to base type.                
#define PE_SYM_DTYPE_POINTER                         1
// The symbol is a function that returns a base type.
#define PE_SYM_DTYPE_FUNCTION                        2
// The symbol is an array of base type.
#define PE_SYM_DTYPE_ARRAY                           3

// Symbol storage classes
// A special symbol that represents the end of function, for debugging purposes.
#define PE_SYM_CLASS_END_OF_FUNCTION                 -1
// No assigned storage class.
#define PE_SYM_CLASS_NULL                            0
// The automatic(stack) variable.The Value field specifies the stack frame offset.
#define PE_SYM_CLASS_AUTOMATIC                       1
// A value that Microsoft tools use for external symbols. The Value field indicates 
// the size if the section number is PE_SYM_UNDEFINED (0). If the section number 
// is not zero, then the Value field specifies the offset within the section.
#define PE_SYM_CLASS_EXTERNAL                        2
// The offset of the symbol within the section.If the Value field is zero, then 
// the symbol represents a section name.
#define PE_SYM_CLASS_STATIC                          3
// A register variable.The Value field specifies the register number.
#define PE_SYM_CLASS_REGISTER                        4
// A symbol that is defined externally.
#define PE_SYM_CLASS_EXTERNAL_DEF                    5
// A code label that is defined within the module.The Value field specifies 
// the offset of the symbol within the section.
#define PE_SYM_CLASS_LABEL                           6
// A reference to a code label that is not defined.
#define PE_SYM_CLASS_UNDEFINED_LABEL                 7
// The structure member. The Value field specifies the n th member.
#define PE_SYM_CLASS_MEMBER_OF_STRUCT                8
// A formal argument (parameter) of a function. The Value field specifies the n-th argument.
#define PE_SYM_CLASS_ARGUMENT                        9
// The structure tag- name entry.
#define PE_SYM_CLASS_STRUCT_TAG                      10
// A union member. The Value field specifies the n-th member.
#define PE_SYM_CLASS_MEMBER_OF_UNION                 11
// The Union tag- name entry.
#define PE_SYM_CLASS_UNION_TAG                       12
// A Typedef entry.
#define PE_SYM_CLASS_TYPE_DEFINITION                 13
// A static data declaration.
#define PE_SYM_CLASS_UNDEFINED_STATIC                14
// An enumerated type tagname entry.
#define PE_SYM_CLASS_ENUM_TAG                        15
// A member of an enumeration. The Value field specifies the n-th member.
#define PE_SYM_CLASS_MEMBER_OF_ENUM                  16
// A register parameter.
#define PE_SYM_CLASS_REGISTER_PARAM                  17
// A bitfield reference. The Value field specifies the n-th bit in the bitfield.
#define PE_SYM_CLASS_BIT_FIELD                       18
// A .bb (beginning of block) or .eb (end of block) record. The Value field is 
// the relocatable address of the code location.
#define PE_SYM_CLASS_BLOCK                           100
// A value that Microsoft tools use for symbol records that define the extent of 
// a function: begin function (.bf), end function (.ef), and lines in function (.lf).
// For .lf records, the Value field gives the number of source lines in the function.
// For .ef records, the Value field gives the size of the function code.
#define PE_SYM_CLASS_FUNCTION                        101
// An end-of-structure entry.
#define PE_SYM_CLASS_END_OF_STRUCT                   102
// A value that Microsoft tools, as well as traditional COFF format, use for the 
// source-file symbol record. The symbol is followed by auxiliary records that name the file.
#define PE_SYM_CLASS_FILE                            103
// A definition of a section (Microsoft tools use STATIC storage class instead).
#define PE_SYM_CLASS_SECTION                         104
// A weak external. For more information, see Auxiliary Format 3: Weak Externals.
#define PE_SYM_CLASS_WEAK_EXTERNAL                   105
// A CLR token symbol.The name is an ASCII string that consists of the hexadecimal value 
// of the token. For more information, see CLR Token Definition (Object Only).
#define PE_SYM_CLASS_CLR_TOKEN                       107

// Relocation struct
typedef PACKED_MS struct s_pe_coff_relocation {
    UINT32  virtual_addr;
    UINT32  symbol_table_index;
    UINT16  relocation_type;
} PACKED_GNU pe_coff_relocation;

// Symbol name union
typedef PACKED_MS union u_pe_coff_name {
    CHAR8   name[8];
    UINT64  string_table_offset;
} PACKED_GNU pe_coff_name;

// Symbol table record
typedef PACKED_MS struct s_pe_coff_symbol_table_record {
    pe_coff_name name;
    UINT32  value;
    UINT16  section_number;
    UINT16  type;
    UINT8   storage_class;
    UINT8   number_of_aux_symbols;
} PACKED_GNU pe_coff_symbol_table_record;

// Auxiliary format #1
typedef PACKED_MS struct s_pe_coff_aux_format_1 {
    UINT32  tag_index;
    UINT32  total_size;
    UINT32  pointer_to_line_number;
    UINT32  pointer_to_next_function;
    UINT16  unused;
} PACKED_GNU pe_coff_aux_format_1;

// Auxiliary format #2
typedef PACKED_MS struct s_pe_coff_aux_format_2 {
    UINT32  unused1;
    UINT16  line_number;
    CHAR8   unused_bytes[6];
    UINT32  pointer_to_next_function;
    UINT16  unused2;
} PACKED_GNU pe_coff_aux_format_2;

// Auxiliary format #3
typedef PACKED_MS struct s_pe_coff_aux_format_3 {
    UINT32  tag_index;
    UINT32  characteristics;
    CHAR8   unused_bytes[10];
} PACKED_GNU pe_coff_aux_format_3;

// Auxiliary format #4
typedef PACKED_MS struct s_pe_coff_aux_format_4 {
    CHAR8   file_name[18];
} PACKED_GNU pe_coff_aux_format_4;

// Auxiliary format #5
typedef PACKED_MS struct s_pe_coff_aux_format_5 {
    UINT32  length;
    UINT16  number_of_relocations;
    UINT16  number_of_line_numbers;
    UINT32  checksum;
    UINT16  number;
    UINT8   selection;
    CHAR8   unused[3];
} PACKED_GNU pe_coff_aux_format_5;

// COMDAT sections
// If this symbol is already defined, the linker issues a "multiply defined symbol" error.
#define PE_COMDAT_SELECT_NODUPLICATES                1
// Any section that defines the same COMDAT symbol can be linked; the rest are removed.
#define PE_COMDAT_SELECT_ANY                         2
// The linker chooses an arbitrary section among the definitions for this symbol. If all 
// definitions are not the same size, a "multiply defined symbol" error is issued.
#define PE_COMDAT_SELECT_SAME_SIZE                   3
// The linker chooses an arbitrary section among the definitions for this symbol. If all 
// definitions do not match exactly, a "multiply defined symbol" error is issued.
#define PE_COMDAT_SELECT_EXACT_MATCH                 4
// The section is linked if a certain other COMDAT section is linked. This other section 
// is indicated by the Number field of the auxiliary symbol record for the section definition.
// This setting is useful for definitions that have components in multiple sections(for 
// example, code in one and data in another), but where all must be linked or discarded as a 
// set. The other section with which this section is associated must be a COMDAT section; 
// it cannot be another associative COMDAT section (that is, the other section cannot have 
// PE_COMDAT_SELECT_ASSOCIATIVE set).
#define PE_COMDAT_SELECT_ASSOCIATIVE                 5
// The linker chooses the largest definition from among all of the definitions for this 
// symbol. If multiple definitions have this size, the choice between them is arbitrary.
#define PE_COMDAT_SELECT_LARGEST                     6

// CLR token definition
typedef PACKED_MS struct s_pe_coff_clr_token {
    UINT8   aux_type;
    UINT8   reserved;
    UINT32  symbol_table_index;
    CHAR8   reserved_bytes[12];
} PACKED_GNU pe_coff_clr_token;

// Attribute certificate table
typedef PACKED_MS struct s_pe_coff_attribute_certificate_table {
    UINT32  length;
    UINT16  revision;
    UINT16  certificate_type;
    UINT32  certificate;
} PACKED_GNU pe_coff_attribute_certificate_table;

// WIN_CERTIFICATE
// Version 1, legacy version of the Win_Certificate structure. It is supported only 
// for purposes of verifying legacy Authenticode signatures
#define WIN_CERT_REVISION_1_0                           0x0100
// Version 2 is the current version of the Win_Certificate structure.
#define WIN_CERT_REVISION_2_0                           0x0200

// WIN_CERTIFICATE types
// certificate contains an X.509 Certificate
// Not Supported 
#define WIN_CERT_TYPE_X509                              0x0001
// certificate contains a PKCS#7 SignedData structure
#define WIN_CERT_TYPE_PKCS_SIGNED_DATA                  0x0002
// Reserved
#define WIN_CERT_TYPE_RESERVED_1                        0x0003
// Terminal Server Protocol Stack Certificate signing
// Not Supported 
#define WIN_CERT_TYPE_TS_STACK_SIGNED                   0x0004

// Delay-load directory table
typedef PACKED_MS struct s_pe_coff_delay_load_directory_table {
    UINT32  attributes;                     // must be zero
    UINT32  name_rva;
    UINT32  module_handle;
    UINT32  delay_import_address_table;
    UINT32  delay_import_name_table;
    UINT32  bound_delay_import_table;
    UINT32  unload_delay_import_table;
    UINT32  time_stamp;
} PACKED_GNU pe_coff_delay_load_directory_table;

// Debug types
// An unknown value that is ignored by all tools.
#define PE_DEBUG_TYPE_UNKNOWN                        0
// The COFF debug information(line numbers, symbol table, and string table).
// This type of debug information is also pointed to by fields in the file headers.
#define PE_DEBUG_TYPE_COFF                           1
// The Visual C++ debug information.
#define PE_DEBUG_TYPE_CODEVIEW                       2
// The frame pointer omission(FPO) information. This information tells the debugger 
// how to interpret nonstandard stack frames, which use the EBP register for a 
// purpose other than as a frame pointer.
#define PE_DEBUG_TYPE_FPO                            3
// The location of DBG file.
#define PE_DEBUG_TYPE_MISC                           4
// A copy of.pdata section.
#define PE_DEBUG_TYPE_EXCEPTION                      5
// Reserved.
#define PE_DEBUG_TYPE_FIXUP                          6
// The mapping from an RVA in image to an RVA in source image.
#define PE_DEBUG_TYPE_OMAP_TO_SRC                    7
// The mapping from an RVA in source image to an RVA in image.
#define PE_DEBUG_TYPE_OMAP_FROM_SRC                  8
// Reserved for Borland.
#define PE_DEBUG_TYPE_BORLAND                        9
// Reserved.
#define PE_DEBUG_TYPE_RESERVED10                     10
// Reserved.
#define PE_DEBUG_TYPE_CLSID                          11
// PE determinism or reproducibility.
#define PE_DEBUG_TYPE_REPRO                          16

// Debug directory entry
typedef PACKED_MS struct s_pe_coff_debug_directory_entry {
    UINT32  characteristics;
    UINT32  time_date_stamp;
    UINT16  major_version;
    UINT16  minor_version;
    UINT32  type;
    UINT32  size_of_data;
    UINT32  addr_of_raw_data;
    UINT32  pointer_to_raw_data;
} PACKED_GNU pe_coff_debug_directory_entry;

// FPO data
typedef PACKED_MS struct s_pe_coff_fpo_data {
    UINT32  offset_start;   // offset 1st byte of function code
    UINT32  proc_size;      // # bytes in function
    UINT32  locals_size;    // # bytes in locals/4
    UINT16  params_size;    // # bytes in params/4
    UINT16  prolog : 8;     // # bytes in prolog
    UINT16  regs : 3;       // # regs saved
    UINT16  seh : 1;        // TRUE if SEH in func
    UINT16  bp : 1;         // TRUE if EBP has been allocated
    UINT16  reserved : 1;   // reserved for future use
    UINT16  frame : 2;      // frame type
} PACKED_GNU pe_coff_fpo_data;

// Export directory table
typedef PACKED_MS struct s_pe_coff_export_directory_table {
    UINT32  export_flags;
    UINT32  time_date_stamp;
    UINT16  major_version;
    UINT16  minor_version;
    UINT32  name_rva;
    UINT32  ordinal_base;
    UINT32  address_table_entries;
    UINT32  number_of_name_pointers;
    UINT32  export_addr_table_rva;
    UINT32  name_pointer_rva;
    UINT32  ordinal_table_rva;
} PACKED_GNU pe_coff_export_directory_table;

typedef UINT32  pe_coff_export_rva;
typedef UINT32  pe_coff_forwarder_rva;

// Import directory table
typedef PACKED_MS struct s_pe_coff_import_directory_table {
    UINT32  import_lookup_table_rva;
    UINT32  time_date_stamp;
    UINT32  forwarder_chain;
    UINT32  name_rva;
    UINT32  import_addr_table_rva;
} PACKED_GNU pe_coff_import_directory_table;

#define PE32_COFF_IMPORT_BY_ORDINAL                     BIT31
#define PE32_COFF_IMPORT_BY_ORDINAL_MASK                0x0000FFFF

#define PE64_COFF_IMPORT_BY_ORDINAL                     BIT63
#define PE64_COFF_IMPORT_BY_ORDINAL_MASK                0x000000000000FFFFLL

#define PE_COFF_HINT_NAME_TABLE_RVA_MASK                0x3FFFFFFF

// x64 function table entry
typedef PACKED_MS struct s_pe_coff_x64_fn_table_entry {
    UINT32  begin_addr;     // rva of corresponding fn
    UINT32  end_addr;       // rva of end of the fn
    UINT32  unwind_info;    // rva of the unwind info
} PACKED_GNU pe_coff_x64_fn_table_entry;

// ARM function table entry
typedef PACKED_MS struct s_pe_coff_arm_fn_table_entry {
    UINT32  begin_addr;         // va of corresponding fn
    UINT32  prolog_length : 8;  // number of instructions in the fn prolog
    UINT32  fn_length: 22;      // number of instructions in the fn
    UINT32  flag_32bit: 1;      // is 32-bit?
    UINT32  exception_flag : 1; // exception handler exists?
} PACKED_GNU pe_coff_arm_fn_table_entry;

// Base relocation block
typedef PACKED_MS struct s_pe_coff_base_relocation_block {
    UINT32  page_rva;
    UINT32  block_size;
} PACKED_GNU pe_coff_base_relocation_block;

// Type of offset field entry
typedef PACKED_MS struct s_pe_coff_type_or_offset_field {
    UINT16  base_relocation_type : 4;
    UINT16  offset : 12;
} PACKED_GNU pe_coff_type_or_offset_field;

// Base relocation types
// The base relocation is skipped.This type can be used to pad a block.
#define PE_REL_BASED_ABSOLUTE                        0
// The base relocation adds the high 16 bits of the difference to the 16-bit 
// field at offset. The 16-bit field represents the high value of a 32-bit word.
#define PE_REL_BASED_HIGH                            1
// The base relocation adds the low 16 bits of the difference to the 16-bit field 
// at offset. The 16-bit field represents the low half of a 32-bit word.
#define PE_REL_BASED_LOW                             2
// The base relocation applies all 32 bits of the difference to the 32 - bit field at offset.
#define PE_REL_BASED_HIGHLOW                         3
// The base relocation adds the high 16 bits of the difference to the 16-bit field at offset.
// The 16-bit field represents the high value of a 32-bit word. The low 16 bits of the 32-bit value 
// are stored in the 16-bit word that follows this base relocation. This means that this base 
// relocation occupies two slots.
#define PE_REL_BASED_HIGHADJ                         4
// The relocation interpretation is dependent on the machine type.
// When the machine type is MIPS, the base relocation applies to a MIPS jump instruction.
#define PE_REL_BASED_MIPS_JMPADDR                    5
// This relocation is meaningful only when the machine type is ARM or Thumb. The base relocation 
// applies the 32-bit address of a symbol across a consecutive MOVW / MOVT instruction pair.
#define PE_REL_BASED_ARM_MOV32                       5
// This relocation is only meaningful when the machine type is RISC-V. The base relocation applies 
// to the high 20 bits of a 32-bit absolute address.
#define PE_REL_BASED_RISCV_HIGH20                    5
// This relocation is meaningful only when the machine type is Thumb. The base relocation applies 
// the 32-bit address of a symbol to a consecutive MOVW / MOVT instruction pair.
#define PE_REL_BASED_THUMB_MOV32                     7
// This relocation is only meaningful when the machine type is RISC-V. The base relocation applies 
// to the low 12 bits of a 32-bit absolute address formed in RISC-V I-type instruction format.
#define PE_REL_BASED_RISCV_LOW12I                    7
// This relocation is only meaningful when the machine type is RISC-V. The base relocation applies 
// to the low 12 bits of a 32-bit absolute address formed in RISC - V S - type instruction format.
#define PE_REL_BASED_RISCV_LOW12S                    8
// The relocation is only meaningful when the machine type is MIPS. The base relocation applies 
// to a MIPS16 jump instruction.
#define PE_REL_BASED_MIPS_JMPADDR16                  9
// The base relocation applies the difference to the 64-bit field at offset.
#define PE_REL_BASED_DIR64                           10

// Thread local storage directory
typedef PACKED_MS struct s_pe32_coff_tls_directory {
    UINT32  raw_data_start_va;
    UINT32  raw_data_end_va;
    UINT32  address_of_index;
    UINT32  address_of_callbacks;
    UINT32  size_of_zero_fill;
    UINT32  characteristics;
} PACKED_GNU pe32_coff_tls_directory;

typedef PACKED_MS struct s_pe64_coff_tls_directory {
    UINT64  raw_data_start_va;
    UINT64  raw_data_end_va;
    UINT64  address_of_index;
    UINT64  address_of_callbacks;
    UINT32  size_of_zero_fill;
    UINT32  characteristics;
} PACKED_GNU pe64_coff_tls_directory;

typedef VOID(*PPE_TLS_CALLBACK)(VOID* dll_handle, UINT32 reason, VOID* reserved);

#define PE_COFF_TLS_CALLBACK_REASON_DLL_PROCESS_ATTACH      1
#define PE_COFF_TLS_CALLBACK_REASON_DLL_THREAD_ATTACH       2
#define PE_COFF_TLS_CALLBACK_REASON_DLL_THREAD_DETACH       3
#define PE_COFF_TLS_CALLBACK_REASON_DLL_PROCESS_DETACH      0

typedef PACKED_MS struct s_pe32_coff_load_configuration_structure {
    UINT32  characteristics;
    UINT32  time_date_stamp;
    UINT16  major_version;
    UINT16  minor_version;
    UINT32  global_flags_clear;
    UINT32  global_flags_set;
    UINT32  critical_section_default_timeout;
    UINT32  decommit_free_block_threshhold;
    UINT32  decommit_total_free_threshhold;
    UINT32  lock_prefix_table;
    UINT32  max_allocation_size;
    UINT32  virtual_memory_threshhold;
    UINT32  process_affinity_mask;
    UINT32  process_heap_flags;
    UINT16  csd_version;
    UINT16  reserved;
    UINT32  edit_list;
    UINT32  security_cookie;
    UINT32  seh_table;
    UINT32  seh_handler_count;
    UINT32  guard_cfcheck_fn_ptr;
    UINT32  guard_cfdispatch_fn_ptr;
    UINT32  guard_cf_fn_table;
    UINT32  guard_cf_fn_count;
    UINT32  guard_flags;
    CHAR8   code_integrity[12];
    UINT32  guard_address_taken_iat_entry_table;
    UINT32  guard_address_taken_iat_entry_count;
    UINT32  guard_long_jump_target_table;
    UINT32  guard_long_jump_target_count;
} PACKED_GNU pe32_coff_load_configuration_structure;

typedef PACKED_MS struct s_pe64_coff_load_configuration_structure {
    UINT32  characteristics;
    UINT32  time_date_stamp;
    UINT16  major_version;
    UINT16  minor_version;
    UINT32  global_flags_clear;
    UINT32  global_flags_set;
    UINT32  critical_section_default_timeout;
    UINT64  decommit_free_block_threshhold;
    UINT64  decommit_total_free_threshhold;
    UINT64  lock_prefix_table;
    UINT64  max_allocation_size;
    UINT64  virtual_memory_threshhold;
    UINT64  process_affinity_mask;
    UINT32  process_heap_flags;
    UINT16  csd_version;
    UINT16  reserved;
    UINT64  edit_list;
    UINT64  security_cookie;
    UINT64  seh_table;
    UINT64  seh_count;
    UINT64  guard_cfcheck_fn_ptr;
    UINT64  guard_cfdispatch_fn_ptr;
    UINT64  guard_cf_fn_table;
    UINT64  guard_cf_fn_count;
    UINT32  guard_flags;
    CHAR8   code_integrity[12];
    UINT64  guard_address_taken_iat_entry_table;
    UINT64  guard_address_taken_iat_entry_count;
    UINT64  guard_long_jump_target_table;
    UINT64  guard_long_jump_target_count;
} PACKED_GNU pe64_coff_load_configuration_structure; 

// Module performs control flow integrity checks using system - supplied support.
#define PE_GUARD_CF_INSTRUMENTED                     0x00000100
// Module performs control flow and write integrity checks.
#define PE_GUARD_CFW_INSTRUMENTED                    0x00000200
// Module contains valid control flow target metadata.
#define PE_GUARD_CF_FUNCTION_TABLE_PRESENT           0x00000400
// Module does not make use of the / GS security cookie.
#define PE_GUARD_SECURITY_COOKIE_UNUSED              0x00000800
// Module supports read only delay load IAT.
#define PE_GUARD_PROTECT_DELAYLOAD_IAT               0x00001000
// Delayload import table in its own.didat section(with nothing else in it) that 
// can be freely reprotected.
#define PE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION    0x00002000
// Module contains suppressed export information.This also infers that the address 
// taken IAT table is also present in the load config.
#define PE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT  0x00004000
// Module enables suppression of exports.
#define PE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION        0x00008000
// Module contains longjmp target information.
#define PE_GUARD_CF_LONGJUMP_TABLE_PRESENT           0x00010000
// Mask for the subfield that contains the stride of Control Flow Guard function table 
// entries (that is, the additional count of bytes per table entry).
#define PE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK         0xF0000000
// Additionally, the Windows SDK winnt.h header defines this macro for the amount of 
// bits to right-shift the GuardFlags value to right-justify the Control Flow Guard 
// function table stride:
#define PE_GUARD_CF_FUNCTION_TABLE_SIZE_SHIFT 28

typedef PACKED_MS struct s_pe_coff_resource_directory_table {
    UINT32  characteristics;
    UINT32  time_date_stamp;
    UINT16  major_version;
    UINT16  minor_version;
    UINT16  number_of_name_entries;
    UINT16  number_of_id_entries;
} PACKED_GNU pe_coff_resource_directory_table;

typedef PACKED_MS struct s_pe_coff_resource_directory_entry {
    UINT32  name_offset;
    UINT32  integer_id;
    UINT32  data_entry_offset;
    UINT32  subdirectory_offset;
} PACKED_GNU pe_coff_resource_directory_entry;

typedef PACKED_MS struct s_pe_coff_resource_directory_string {
    UINT16  length;
    UINTN   start_of_string;
} PACKED_GNU pe_coff_resource_directory_string;

typedef PACKED_MS struct s_pe_coff_resource_data_entry {
    UINT32  data_rva;
    UINT32  size;
    UINT32  codepage;
    UINT32  reserved;   // must be zero
} PACKED_GNU pe_coff_resource_data_entry;

#define PE_COFF_ARCHIVE_FILE_SIGNATURE                  "!<arch>"

typedef PACKED_MS struct s_pe_coff_archive_member_header {
    CHAR8   name[16];
    CHAR8   datestamp[12];
    CHAR8   user_id[6];
    CHAR8   group_id[6];
    CHAR8   mode[8];
    CHAR8   size[10];
    CHAR8   end_of_header[2];  // { '~', '\n' }
} PACKED_GNU pe_coff_archive_member_header;

// Import header
typedef PACKED_MS struct s_pe_coff_import_header {
    UINT16  sig1;
    UINT16  sig2;
    UINT16  version;
    UINT16  machine;
    UINT32  time_date_stamp;
    UINT32  size_of_data;
    UINT16  ordinal_or_hint;
    UINT16  import_type: 2;
    UINT16  name_type : 3;
    UINT16  reserved : 11;
} PACKED_GNU pe_coff_import_header;

#define PE_COFF_IMPORT_TYPE_CODE                        0
#define PE_COFF_IMPORT_TYPE_DATA                        1
#define PE_COFF_IMPORT_TYPE_CONST                       2

// Uefi file struct
typedef struct s_uefi_file {
    SHELL_FILE_HANDLE file_handle;
    CHAR8 *buffer;
    UINT64 size;
} uefi_file;

VOID UefiLoadPEFile(CHAR16 *filename, uefi_file *fp);

#endif // __K0_PE_H
