#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

// Note: Not all instructions are necessarily implemented, but are still noted here for
// completeness. This may be either because it is problematic to run on a guest, or
// because it is simply not an instruction that is supported on a guest domain.
// Many of the descriptions are obtained directly from the Intel manual.

#include "main.h"

#define SUSPEND_INTERRUPTS(FLAGS, CODE) \
    local_irq_save(FLAGS);              \
    local_irq_disable();                \
    CODE                                \
    local_irq_restore(FLAGS);           \
    local_irq_enable();


extern uintptr_t *vmem_aligned;

// CPU feature check for virtual machine extensions
void cpuid_test_support_f(void);

// XOR
void xor_f(void);

// ** Begin Instructions That Cause VM Exits Unconditionally **

// CPUID
// Returns processor identification and feature information in the EAX, EBX, ECX, and EDX registers.
void cpuid_f(void);

// GETSEC
// The GETSEC instruction provides a capability leaf function for system software to discover
// the available GETSEC leaf functions that are supported in a processor. Table 6-2 lists the
// currently available GETSEC leaf functions.
void getsec_f(void);

// INVD
// Invalidates (flushes) the processor's internal caches and issues a special-function bus
// cycle that directs external caches to also flush themselves. Data held in internal caches
// is not written back to main memory.
void invd_f(void);

// XSETBV
// Writes the contents of registers EDX:EAX into the 64-bit extended control register (XCR)
// specified in the ECX register. (On processors that support the Intel 64 architecture,
// the high-order 32 bits of RCX are ignored.)
void xsetbv_f(void);

/*
// INVEPT
// Invalidate Cached EPT Mappings
void  invept_f(void) {
    int operand = 0x0001;
    int type = 1;
    asm volatile(
        "invept %0, %1\n"
        :
        : "m"(operand), "r"(type)
    );
}
*/
// INVVPID
// Invalidate Cached VPID Mappings
// <Not implemented>

// VMCALL
// Call to VM Monitor
// <Not implemented>

// VMCLEAR
// Clear Virtual-Machine Control Structure
// <Not implemented>

// VMLAUNCH
// Launch Virtual Machine
// <Not implemented>

// VMPTRLD
// Load Pointer to Virtual-Machine Control Structure
// <Not implemented>

// VMPTRST
// Store Pointer to Virtual-Machine Control Structure
// <Not implemented>

// VMRESUME
// Resume Virtual Machine
// <Not implemented>

// VMXOFF
// Leave VMX Operation
// <Not implemented>

// VMXON
// Enter VMX Operation
// <Not implemented>

// ** End Instructions That Cause VM Exits Unconditionally **

// ** Begin Instructions That Cause VM Exits Conditionally  **

// CLTS
// The CLTS instruction causes a VM exit if the bits in position 3 (corresponding to CR0.TS)
// are set in both the CR0 guest/host mask and the CR0 read shadow.
// Description: Clears the task-switched (TS) flag in the CR0 register.
void clts_f(void);

// ENCLS
// The ENCLS instruction causes a VM exit if the "enable ENCLS exiting" VM-execution control is 1 and one of the following is true:
//    - The value of EAX is less than 63 and the corresponding bit in the ENCLS-exiting bitmap is 1 (see Section 24.6.16).
//    - The value of EAX is greater than or equal to 63 and bit 63 in the ENCLS-exiting bitmap is 1.
// <Not implemented>

// HLT
// The HLT instruction causes a VM exit if the "HLT exiting" VM-execution control is 1.
// <Not implemented>

// IN, INS/INSB/INSW/INSD
// The behavior of each of these instructions is determined by the settings of the "unconditional I/O exiting" and "use I/O bitmaps" VM-execution controls:
//    - If both controls are 0, the instruction executes normally.
//    - If the "unconditional I/O exiting" VM-execution control is 1 and the "use I/O bitmaps" VM-execution control the instruction causes a VM exit.
//    - If the "use I/O bitmaps" VM-execution control is 1, the instruction causes a VM exit if it attempts to access an I/O port corresponding to a bit set to 1 in the appropriate I/O bitmap (see Section 24.6.4). If an I/O operation "wraps around" the 16-bit I/O-port space (accesses ports FFFFH and 0000H), the I/O instruction causes a VM exit (the "unconditional I/O exiting" VM-execution control is ignored if the "use I/O bitmaps" VM-execution control is 1).
//See Section 25.1.1 for information regarding the priority of VM exits relative to faults that may be caused by the INS and OUTS instructions.
void inb_f(void);

// OUT, OUTS/OUTSB/OUTSW/OUTSD
// (See the exiting description in the "IN, INS/INSB/INSW/INSD" section above)
void outb_f(void);

// INVLPG
// The INVLPG instruction causes a VM exit if the "INVLPG exiting" VM-execution control is 1.
// Description: Invalidates any translation lookaside buffer (TLB) entries specified with the
// source operand. The source operand is a memory address. The processor determines the page
// that contains that address and flushes all TLB entries for that page.
void invlpg_f(void);

// INVPCID
// The INVPCID instruction causes a VM exit if the "INVLPG exiting" and "enable INVPCID"
// VM-execution controls are both 1.
void invpcid_f(void);

// LGDT, LIDT, LLDT, LTR, SGDT, SIDT, SLDT, STR
// These instructions cause VM exits if the "descriptor-table exiting" VM-execution control is 1.
// <Not implemented>

// LMSW
// In general, the LMSW instruction causes a VM exit if it would write, for any bit set in the low 4 bits of the CR0 guest/host mask, a value different than the corresponding bit in the CR0 read shadow. LMSW never clears bit 0 of CR0 (CR0.PE); thus, LMSW causes a VM exit if either of the following are true:
//    - The bits in position 0 (corresponding to CR0.PE) are set in both the CR0 guest/mask and the source operand, and the bit in position 0 is clear in the CR0 read shadow.
//    - For any bit position in the range 3:1, the bit in that position is set in the CR0 guest/mask and the values of the corresponding bits in the source operand and the CR0 read shadow differ.
// <Not implemented>

// MONITOR
// The MONITOR instruction causes a VM exit if the “MONITOR exiting” VM-execution control is 1.
// <Not implemented>

// MOV from CR3
// The MOV from CR3 instruction causes a VM exit if the “CR3-store exiting” VM-execution control is 1. The first processors to support the virtual-machine extensions supported only the 1-setting of this control.
// Note: At the moment, this is effectively implemented in mov_from_cr3_f(), as it does a MOV from RAX, then a MOV to RAX [of the same value].

// MOV from CR8
// The MOV from CR8 instruction causes a VM exit if the “CR8-store exiting” VM-execution control is 1.
void mov_from_cr8_f(void);

// MOV to CR0
// The MOV to CR0 instruction causes a VM exit unless the value of its source operand matches, for the position of each bit set in the CR0 guest/host mask, the corresponding bit in the CR0 read shadow. (If every bit is clear in the CR0 guest/host mask, MOV to CR0 cannot cause a VM exit.)
void mov_to_cr0_f(void);

// MOV to CR3
// The MOV to CR3 instruction causes a VM exit unless the “CR3-load exiting” VM-execution control is 0 or the value of its source operand is equal to one of the CR3-target values specified in the VMCS. Only the first n CR3-target values are considered, where n is the CR3-target count. If the “CR3-load exiting” VMexecution control is 1 and the CR3-target count is 0, MOV to CR3 always causes a VM exit.
// The first processors to support the virtual-machine extensions supported only the 1-setting of the “CR3-load exiting” VM-execution control. These processors always consult the CR3-target controls to determine whether an execution of MOV to CR3 causes a VM exit.
void mov_to_cr3_f(void);

// MOV to CR4
// The MOV to CR4 instruction causes a VM exit unless the value of its source operand matches, for the position of each bit set in the CR4 guest/host mask, the corresponding bit in the CR4 read shadow.
void mov_to_cr4_f(void);

// MOV to CR8
// The MOV to CR8 instruction causes a VM exit if the “CR8-load exiting” VM-execution control is 1.
void mov_to_cr8_f(void);

// MOV DR
// The MOV DR instruction causes a VM exit if the “MOV-DR exiting” VM-execution control is 1. Such VM exits represent an exception to the principles identified in Section 25.1.1 in that they take priority over the following: general-protection exceptions based on privilege level; and invalid-opcode exceptions that occur because CR4.DE=1 and the instruction specified access to DR4 or DR5.
void mov_dr0_f(void);
void mov_dr1_f(void);
void mov_dr2_f(void);
void mov_dr3_f(void);
void mov_dr4_f(void);
void mov_dr5_f(void);
void mov_dr6_f(void);
void mov_dr7_f(void);

// MWAIT
// The MWAIT instruction causes a VM exit if the “MWAIT exiting” VM-execution control is 1.
// If this control is 0, the behavior of the MWAIT instruction may be modified (see Section 25.3).
// <Not implemented>

// PAUSE
// The behavior of each of this instruction depends on CPL and the settings of the “PAUSE exiting”
// and “PAUSE-loop exiting” VM-execution controls:
//    - CPL = 0.
//         - If the “PAUSE exiting” and “PAUSE-loop exiting” VM-execution controls are both 0, the
//           instruction executes normally.
//         - If the “PAUSE exiting” VM-execution control is 1, the PAUSE instruction causes a VM
//           exit (the “PAUSEloop exiting” VM-execution control is ignored if CPL = 0 and the
//           “PAUSE exiting” VM-execution control is 1).
//         - If the “PAUSE exiting” VM-execution control is 0 and the “PAUSE-loop exiting”
//           VM-execution contro1, the following treatment applies.
//           The processor determines the amount of time between this execution of PAUSE and the
//           previous execution of PAUSE at CPL 0. If this amount of time exceeds the value of the
//           VM-execution control field PLE_Gap, the processor considers this execution to be the
//           first execution of PAUSE in a loop. (It also does so for the first execution of PAUSE
//           at CPL 0 after VM entry.)
//           Otherwise, the processor determines the amount of time since the most recent execution
//           of PAUSE that was considered to be the first in a loop. If this amount of time exceeds
//           the value of the VM-execution control field PLE_Window, a VM exit occurs.
//           For purposes of these computations, time is measured based on a counter that runs at
//           the same rate as the timestamp counter (TSC).
//    - CPL > 0.
//         - If the “PAUSE exiting” VM-execution control is 0, the PAUSE instruction executes normally.
//         - If the “PAUSE exiting” VM-execution control is 1, the PAUSE instruction causes a VM exit.
//    The “PAUSE-loop exiting” VM-execution control is ignored if CPL > 0.

// RDMSR
// The RDMSR instruction causes a VM exit if any of the following are true:
//    - The “use MSR bitmaps” VM-execution control is 0.
//    - The value of ECX is not in the ranges 00000000H – 00001FFFH and C0000000H – C0001FFFH.
//    - The value of ECX is in the range 00000000H – 00001FFFH and bit n in read bitmap for low MSRs
//      is 1, where n is the value of ECX.
//    - The value of ECX is in the range C0000000H – C0001FFFH and bit n in read bitmap for high MSRs
//      is 1, where n is the value of ECX & 00001FFFH.
// See Section 24.6.9 for details regarding how these bitmaps are identified.
void rdmsr_f(void);

// RDPMC
// The RDPMC instruction causes a VM exit if the “RDPMC exiting” VM-execution control is 1.
void rdpmc_f(void);

// RDRAND
// The RDRAND instruction causes a VM exit if the “RDRAND exiting” VM-execution control is 1.
void rdrand_f(void);

// RDSEED
// The RDSEED instruction causes a VM exit if the “RDSEED exiting” VM-execution control is 1.
void rdseed_f(void);

// RDTSC
// The RDTSC instruction causes a VM exit if the “RDTSC exiting” VM-execution control is 1.
void rdtsc_f(void);

// RDTSCP
// The RDTSCP instruction causes a VM exit if the “RDTSC exiting” and “enable RDTSCP”
// VM-execution controls are both 1.
void rdtscp_f(void);

// RSM
// The RSM instruction causes a VM exit if executed in system-management mode (SMM).
// <Not implemented>

// VMREAD
// Read Field from Virtual-Machine Control Structure
// The VMREAD instruction causes a VM exit if any of the following are true:
//    - The “VMCS shadowing” VM-execution control is 0.
//    - Bits 63:15 (bits 31:15 outside 64-bit mode) of the register source operand are not all 0.
//    - Bit n in VMREAD bitmap is 1, where n is the value of bits 14:0 of the register source
//      operand. See Section24.6.15 for details regarding how the VMREAD bitmap is identified.
// If the VMREAD instruction does not cause a VM exit, it reads from the VMCS referenced by the VMCS
// link pointer. See Chapter 30, “VMREAD—Read Field from Virtual-Machine Control Structure” for
// details of the operation of the VMREAD instruction.
// <Not implemented>


// VMWRITE
// Write Field to Virtual-Machine Control Structure
// The VMWRITE instruction causes a VM exit if any of the following are true:
//    - The “VMCS shadowing” VM-execution control is 0.
//    - Bits 63:15 (bits 31:15 outside 64-bit mode) of the register source operand are not all 0.
//    - Bit n in VMWRITE bitmap is 1, where n is the value of bits 14:0 of the register source
//      operand. See Section 24.6.15 for details regarding how the VMWRITE bitmap is identified.
// If the VMWRITE instruction does not cause a VM exit, it writes to the VMCS referenced by the
// VMCS link pointer. See Chapter 30, “VMWRITE—Write Field to Virtual-Machine Control Structure”
// for details of the operation of the VMWRITE instruction.
// <Not implemented>

// The WBINVD instruction causes a VM exit if the “WBINVD exiting” VM-execution control is 1.
void wbinvd_f(void);

// WRMSR
// The WRMSR instruction causes a VM exit if any of the following are true:
//    - The “use MSR bitmaps” VM-execution control is 0.
//    - The value of ECX is not in the ranges 00000000H – 00001FFFH and C0000000H – C0001FFFH.
//    - The value of ECX is in the range 00000000H – 00001FFFH and bit n in write bitmap for low MSRs is 1, where n is the value of ECX.
//    - The value of ECX is in the range C0000000H – C0001FFFH and bit n in write bitmap for high MSRs is 1, where n is the value of ECX & 00001FFFH.
// See Section 24.6.9 for details regarding how these bitmaps are identified.
// Description: Writes the contents of registers EDX:EAX into the 64-bit model specific register (MSR) specified in the ECX register.
void wrmsr_f(void);

// XRSTORS
// The XRSTORS instruction causes a VM exit if the “enable XSAVES/XRSTORS” VM-execution control is 1 and any bit is set in the logical-AND of the following three values: EDX:EAX, the IA32_XSS MSR, and the XSS-exiting bitmap (see Section 24.6.19).
// <Not implemented>

// XSAVES
// The XSAVES instruction causes a VM exit if the “enable XSAVES/XRSTORS” VM-execution control is 1 and any bit is set in the logical-AND of the following three values: EDX:EAX, the IA32_XSS MSR, and the XSSexiting bitmap (see Section 24.6.19).
// <Not implemented>

// ** End Instructions That Cause VM Exits Conditionally **

// ** Begin Instructions That Are Non-Temporal **

// MOVNTDQA
// Load Double Quadword Non-Temporal Aligned Hint
void movntdqa_f(void);

// MOVNTDQ
// Store Packed Integers Using Non-Temporal Hint
void movntdq_f(void);

// ** End Instructions That Are Non-Temporal **

// ** Begin Instructions That Cause a Write-Combining Buffer Flush **

// CLFLUSH
// Flush Cache Line
void clflush_f(void);

// SFENCE
// Store Fence
void sfence_f(void);

// MFENCE
// Memory Fence
void mfence_f(void);

// ** End Instructions That Cause a Write-Combining Buffer Flush **

#endif
