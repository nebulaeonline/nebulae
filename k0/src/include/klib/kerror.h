// Copyright (c) 2003-2018 RCH3 (nebulae@nebulae.online)
// Copyright (c) 2019 Nebulae Foundation, LLC. All rights reserved.
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

#ifndef __K0_KERROR_H
#define __K0_KERROR_H

// System-wide Status Code type
typedef INT64 nstatus;
#define NEB_ERROR(X)                    ((X < 0) ? TRUE : FALSE)
#define NEB_SUCCESS(X)                  ((X >= 0) ? TRUE : FALSE)
#define NEB_OK                          0LL
#define NEB_UNDEFINED                   -1LL

// Error Codes
#define NEBERROR_NULL_PTR_UNEXPECTED    -2LL
#define NEBERROR_MEM_ALLOCATION_ERR     -3LL
#define NEBERROR_MEM_ZERO_ERR           -4LL

#define NEBERROR_BAD_STACK_DIRECTION    -1000LL
#define NEBERROR_BAD_STACK_SIZE         -1001LL
#define NEBERROR_STACK_MEM_ALLOC_ERR    -1002LL
#define NEBERROR_STACK_ELEMENT_NOT_FOUND -1003LL
#define NEBERROR_UNABLE_TO_PUSH_VALUE   -1004LL

#define NEBERROR_INVALID_PAGE_SIZE      -5000LL

#define NEBERROR_ACPI_RSDP_NOT_FOUND    -10000LL
#define NEBERROR_ACPI_XSDT_NOT_FOUND    -10001LL
#define NEBERROR_ACPI_INVALID_XSDT      -10002LL

#define NEBSTATUS_REMOVED_4KB_PAGE      0x1000LL
#define NEBSTATUS_REMOVED_2MB_PAGE      0x200000LL

#define NEBSTATUS_UNDETERMINED          -555555555556LL

#endif // __K0_KERROR_H
