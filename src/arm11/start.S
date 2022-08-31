
#include "linux.h"
#include "scu.h"

	.arm
	.section .text.start
	.cpu mpcore

	.global _start
_start:
	@ Disable FIQs, IRQs, imprecise aborts
	@ and enter SVC mode
	cpsid aif, #0x13

	@ Invalidate Entire Instruction Cache,
	@ also flushes the branch target cache
	mov r0, #0
	mcr p15, 0, r0, c7, c5, 0

	@ Clear and Invalidate Entire Data Cache
	mov r0, #0
	mcr p15, 0, r0, c7, c14, 0

	@ Data Synchronization Barrier
	mov r0, #0
	mcr p15, 0, r0, c7, c10, 4

	@ Disable the MMU and data cache
	@ (the MMU is already disabled)
	mrc p15, 0, r1, c1, c0, 0
	bic r1, r1, #0b101
	mcr p15, 0, r1, c1, c0, 0

	@ Clear exclusive records
	clrex

	ldr r0, =SYNC_ADDR
	mov r1, #0
	str r1, [r0]
	str r1, [r0, #4]
	str r1, [r0, #8]
	str r1, [r0, #12]

	@ Disable SCU
	ldr r0, =SCU_BASE_ADDR
	ldr r1, [r0, #SCU_CTRL_REG]
	bic r1, r1, #SCU_CTRL_ENABLE
	str r1, [r0, #SCU_CTRL_REG]

	@ Invalidate SCU ways
	ldr r1, =0xFF
	str r1, [r0, #SCU_INV_ALL_REG]

	@ CRT initialization
	ldr r0, =__bss_start
	mov r1, #0
	ldr r2, =__bss_end
	add sp, r2, #1024
	sub r2, r2, r0
	bl memset

	bl enable_cores23

	@ Trigger CPU1
	ldr r3, =0x1FFFFFDC
	adr r4, smp_start
	str r4, [r3]

	ldr r3, =(SCU_BASE_ADDR + 0x1F00)
	ldr r4, =((2 << 16) | 1)
	str r4, [r3]

	@@@@@ Jump to the kernel @@@@@

	@ Wait until ARM9 tells Linux address...
	ldr r0, =SYNC_ADDR
wait_arm9:
	ldr r1, [r0]
	cmp r1, #0
	beq wait_arm9

	@@@@@ Map Framebuffers @@@@@
	blx init_screens
	@ Setup the registers before
	@ jumping to the kernel entry
	mov r0, #0
	ldr r1, =MACHINE_NUMBER
	ldr r2, =DTB_ADDR
	ldr lr, =ZIMAGE_ADDR
	@ Jump to the kernel!
	bx lr

smp_start:
	@ Get the CPUID
	mrc p15, 0, r0, c0, c0, 5
	ands r0, r0, #3
	@ Fall through into smp_boot

.global smp_boot
.align 4
smp_boot:
	@ r0 is the current CPU ID
	ldr r1, =SYNC_ADDR
	add r1, r1, r0, lsl #2

.Lwait_smpboot:
	ldr r12, [r1]
	cmp r12, #0
	wfeeq
	beq .Lwait_smpboot

	bx r12
.type smp_boot, STT_FUNC
.size smp_boot, . - smp_boot

	.ltorg
