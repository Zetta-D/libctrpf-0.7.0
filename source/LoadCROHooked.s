BEGIN_ASM_FUNC LoadCROHooked, weak
    str     lr, [sp,#-4]!
    bl      OnLoadCro
    ldr     lr, [sp],#4
    ldmfd   sp, {R0-r12}
    bx      lr
END_ASM_FUNC

BEGIN_ASM_FUNC AR__ExecuteRoutine, weak
    stmfd sp!, {r0-r12,lr}
    fstmfdd sp!, {d0-d15}
    ldr r2, =stackBak
    str sp, [r2]
    ldmia r0, {r4-r10,sp}
    blx r1
    ldr r0, =stackBak
    ldr sp, [r0]
    fldmead sp!, [d0-d15]
    ldmfd sp!, {r0-r12,pc}
END_ASM_FUNC

        .section .bss.stackBak.42, "aw", %nobits
        .align 2
stackBak:
        .space 8