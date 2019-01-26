// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
// Portions Copyright (c) 2012 Patrick Doane and others.  See AUTHORS.ioapic 
//   file for list.
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

#ifndef __K0_IOAPIC_H
#define __K0_IOAPIC_H

#include "x64.h"

#define X64_APIC_BASE_MSR                   0x1B
#define X64_APIC_BASE_ADDR_MASK             X64_4KB_ALIGN_MASK
#define X64_APIC_MAX_LVT_ENTRY_MASK         0x00FF0000
#define X64_APIC_VERSION_MASK               0x000000FF

#define X64_APIC_IOREGSEL                   0x00
#define X64_APIC_IOWIN                      0x10
#define X64_APIC_IOAPICID                   0x00
#define X64_APIC_IOAPICVER                  0x01
#define X64_APIC_IOAPICARB                  0x02
#define X64_APIC_IOREDTBL                   0x10

#define X64_APIC_GLOBAL_ENABLE              BIT11
#define X64_APIC_BSP                        BIT8
#define X64_APIC_EOI_BROADCAST_SUPPRESSION  BIT24
#define X64_APIC_ID_REG_OFFSET              0x20        // read/write (read-only since Nehalem)
#define X64_APIC_VERSION_REG_OFFSET         0x30        // read only
#define X64_APIC_TPR_OFFSET                 0x80        // read/write
#define X64_APIC_ARB_PRIORITY_REG1_OFFSET   0x90        // read only
#define X64_APIC_PPR_OFFSET                 0xA0        // read only
#define X64_APIC_EOI_REG_OFFSET             0xB0        // write only
#define X64_APIC_REMOTE_READ_REG1_OFFSET    0xC0        // read only
#define X64_APIC_LOGICAL_DEST_REG_OFFSET    0xD0        // read/write
#define X64_APIC_DEST_FORMAT_REG_OFFSET     0xE0        // read/write
#define X64_APIC_SPRIOUS_INT_VECTOR_OFFSET  0xF0        // read/write
#define X64_APIC_ISR_BITS_31_00_OFFSET      0x100       // read only
#define X64_APIC_ISR_BITS_63_32_OFFSET      0x110       // read only
#define X64_APIC_ISR_BITS_95_64_OFFSET      0x120       // read only
#define X64_APIC_ISR_BITS_127_96_OFFSET     0x130       // read only
#define X64_APIC_ISR_BITS_159_128_OFFSET    0x140       // read only
#define X64_APIC_ISR_BITS_191_160_OFFSET    0x150       // read only
#define X64_APIC_ISR_BITS_223_192_OFFSET    0x160       // read only
#define X64_APIC_ISR_BITS_255_224_OFFSET    0x170       // read only
#define X64_APIC_TMR_BITS_31_00_OFFSET      0x180       // read only
#define X64_APIC_TMR_BITS_63_32_OFFSET      0x190       // read only
#define X64_APIC_TMR_BITS_95_64_OFFSET      0x1A0       // read only
#define X64_APIC_TMR_BITS_127_96_OFFSET     0x1B0       // read only
#define X64_APIC_TMR_BITS_159_128_OFFSET    0x1C0       // read only
#define X64_APIC_TMR_BITS_191_160_OFFSET    0x1D0       // read only
#define X64_APIC_TMR_BITS_223_192_OFFSET    0x1E0       // read only
#define X64_APIC_TMR_BITS_255_224_OFFSET    0x1F0       // read only
#define X64_APIC_IRR_BITS_31_00_OFFSET      0x200       // read only
#define X64_APIC_IRR_BITS_63_32_OFFSET      0x210       // read only
#define X64_APIC_IRR_BITS_95_64_OFFSET      0x220       // read only
#define X64_APIC_IRR_BITS_127_96_OFFSET     0x230       // read only
#define X64_APIC_IRR_BITS_159_128_OFFSET    0x240       // read only
#define X64_APIC_IRR_BITS_191_160_OFFSET    0x250       // read only
#define X64_APIC_IRR_BITS_223_192_OFFSET    0x260       // read only
#define X64_APIC_IRR_BITS_255_224_OFFSET    0x270       // read only
#define X64_APIC_ERROR_STATS_REG_OFFSET     0x280       // read only
#define X64_APIC_LVT_CMCI_REG_OFFSET        0x2F0       // read/write
#define X64_APIC_ICR_BITS_31_00_OFFSET      0x300       // read/write
#define X64_APIC_ICR_BITS_63_32_OFFSET      0x310       // read/write
#define X64_APIC_LVT_TIMER_REG_OFFSET       0x320       // read/write
#define X64_APIC_LVT_THERMAL_REG2_OFFSET    0x330       // read/write
#define X64_APIC_LVT_PERF_MON_REG3_OFFSET   0x340       // read/write
#define X64_APIC_LVT_LINT0_REG_OFFSET       0x350       // read/write
#define X64_APIC_LVT_LINT1_REG_OFFSET       0x360       // read/write
#define X64_APIC_LVT_ERROR_REG_OFFSET       0x370       // read/write
#define X64_APIC_INITIAL_COUNT_REG_OFFSET   0x380       // read/write
#define X64_APIC_CURRENT_COUNT_REG_OFFSET   0x390       // read only
#define X64_APIC_DIVIDE_CONFIG_REG_OFFSET   0x3E0       // read/write

#define X64_APIC_INT_TIMER_MODE_ONESHOT     0x00
#define X64_APIC_INT_TIMER_MODE_PERIODIC    (~BIT18 & BIT17)
#define X64_APIC_INT_TIMER_MODE_TSC_DEADLINE (~BIT17 & BIT18)

#define X64_APIC_INT_DELIVERY_STATUS_IDLE   0x00
#define X64_APIC_INT_DELIVERY_STATUS_PENDING BIT12

#define X64_APIC_INT_DELIVERY_MODE_FIXED    0x00
#define X64_APIC_INT_DELIVERY_MODE_SMI      (0x02 << 8)
#define X64_APIC_INT_DELIVERY_MODE_NMI      (0x04 << 8)
#define X64_APIC_INT_DELIVERY_MODE_EXT_INT  (0x07 << 8)
#define X64_APIC_INT_DELIVERY_MODE_INIT     (0x05 << 8)

#define X64_APIC_INT_TRIGGER_MODE_EDGE      0x00
#define X64_APIC_INT_TRIGGER_MODE_LEVEL     BIT15

#define X64_APIC_INT_NOT_MASKED             0x00
#define X64_APIC_INT_MASKED                 BIT16

#define X64_APIC_INT_PIN_POLARITY_HIGH      0x00
#define X64_APIC_INT_PIN_POLARITY_LOW       BIT13

#define X64_APIC_INT_REMOTE_IRR_FLAG        BIT14

#define X64_APIC_INT_VECTOR_MASK            0x000000FF

#define X64_APIC_ESR_ILLEGAL_REG_ADDR       BIT7
#define X64_APIC_ESR_RECV_ILLEGAL_VECTOR    BIT6
#define X64_APIC_ESR_SEND_ILLEGAL_VECTOR    BIT5
#define X64_APIC_ESR_REDIRECTABLE_IPI       BIT4
#define X64_APIC_ESR_RECV_ACCEPT_ERROR      BIT3
#define X64_APIC_ESR_RECV_CHECKSUM_ERROR    BIT2
#define X64_APIC_ESR_SEND_CHECKSUM_ERROR    BIT1

#define X64_APIC_DIVIDE_CONFIG_BY_2         b0000
#define X64_APIC_DIVIDE_CONFIG_BY_4         b0001
#define X64_APIC_DIVIDE_CONFIG_BY_8         b0010
#define X64_APIC_DIVIDE_CONFIG_BY_16        b0011
#define X64_APIC_DIVIDE_CONFIG_BY_32        b1000
#define X64_APIC_DIVIDE_CONFIG_BY_64        b1001
#define X64_APIC_DIVIDE_CONFIG_BY_128       b1010
#define X64_APIC_DIVIDE_CONFIG_BY_1         b1011

#define X64_APIC_END_OF_INTERRUPT           0x20

#define X64_PIC1_CMD                        0x0020
#define X64_PIC1_DATA                       0x0021
#define X64_PIC2_CMD                        0x00a0
#define X64_PIC2_DATA                       0x00a1

#define X64_PIC_ICW1_ICW4                   0x01        // ICW4 command word: 0 = not needed, 1 = needed
#define X64_PIC_ICW1_SINGLE                 0x02        // Single mode: 0 = cascade, 1 = single
#define X64_PIC_ICW1_ADI                    0x04        // Call address interval: 0 = interval of 8, 1 = interval of 4
#define X64_PIC_ICW1_LTIM                   0x08        // Interrupt trigger mode: 0 = edge, 1 = level
#define X64_PIC_ICW1_INIT                   0x10        // Initialization

#define X64_PIC_ICW4_8086                   0x01        // Microprocessor mode: 0=MCS-80/85, 1=8086/8088
#define X64_PIC_ICW4_AUTO                   0x02        // Auto EOI: 0 = disabled, 1 = enabled
#define X64_PIC_ICW4_BUF_SLAVE              0x04        // Buffered mode/slave
#define X64_PIC_ICW4_BUF_MASTER             0x0C        // Buffered mode/master
#define X64_PIC_ICW4_SFNM                   0x10        // Special fully nested is programmed

VOID   InitLocalAPIC();
UINT32 ReadIOApic(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT8 reg);
VOID   WriteIOApic(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT8 reg, UINT32 value);

#endif // __K0_LOCAL_APIC_H
