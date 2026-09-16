.section .text
    .align 2

    /*
     * Ejercicio 05
     * Banderas de eventos atomicas en RISC-V puro.
     *
     * La tabla de vectores/startup del SDK sigue siendo infraestructura.
     * La logica de aplicacion no contiene codigo C.
     */

    /* RCU y GPIOC */
    .equ RCU_AHB1EN,          0x40023830

    .equ GPIOC_CTL,           0x40020800
    .equ GPIOC_OMODE,         0x40020804
    .equ GPIOC_OSPD,          0x40020808
    .equ GPIOC_PUD,           0x4002080C
    .equ GPIOC_BOP,           0x40020818
    .equ GPIOC_BC,            0x40020828
    .equ GPIOC_TG,            0x4002082C

    .equ GPIOC_CLOCK_BIT,     0x00000004
    .equ PC13_BIT,            0x00002000
    .equ PC13_FIELD_MASK,     0x0C000000
    .equ PC13_OUTPUT_MODE,    0x04000000
    .equ PC13_SPEED_10MHZ,    0x04000000

    /* SysTimer */
    .equ SYSTIMER_BASE,       0xD1000000
    .equ SYSTIMER_CMP_LO,     0xD1000008
    .equ SYSTIMER_CMP_HI,     0xD100000C
    .equ SYSTIMER_MTIMECTL,   0xD1000FF8
    .equ MTIMECTL_CMPCLREN,   0x00000002

    /* ECLIC, IRQ 7 */
    .equ ECLIC_INTIP7,        0xD200101C
    .equ ECLIC_INTIE7,        0xD200101D
    .equ ECLIC_INTATTR7,      0xD200101E
    .equ ECLIC_INTCTRL7,      0xD200101F

    .equ ECLIC_POS_EDGE,      0x02

    /* La biblioteca de firmware usada por este proyecto configura 160 MHz. */
    .equ SYSTEM_CORE_CLOCK_HZ, 160000000
    .equ SYSTIMER_COMPARE_1MS, 40000
    .equ EVENT_LED_TICK,       1
    .equ EVENT_ONE_SECOND,     2
    .equ EVENT_MODE_CHANGE,    4

    .global main
    .type main, @function

main:
    /* --------------------------------------------------------- */
    /* 1. Habilitar reloj GPIOC.                                 */
    /* --------------------------------------------------------- */
    li      t0, RCU_AHB1EN
    lw      t1, 0(t0)
    ori     t1, t1, GPIOC_CLOCK_BIT
    sw      t1, 0(t0)
    fence   iorw, iorw

    /* --------------------------------------------------------- */
    /* 2. Configurar PC13 como salida push-pull, 10 MHz, no pull.*/
    /* --------------------------------------------------------- */
    li      t0, GPIOC_CTL
    lw      t1, 0(t0)
    li      t2, PC13_FIELD_MASK
    not     t2, t2
    and     t1, t1, t2
    li      t2, PC13_OUTPUT_MODE
    or      t1, t1, t2
    sw      t1, 0(t0)

    li      t0, GPIOC_OMODE
    lw      t1, 0(t0)
    li      t2, PC13_BIT
    not     t2, t2
    and     t1, t1, t2
    sw      t1, 0(t0)

    li      t0, GPIOC_OSPD
    lw      t1, 0(t0)
    li      t2, PC13_FIELD_MASK
    not     t2, t2
    and     t1, t1, t2
    li      t2, PC13_SPEED_10MHZ
    or      t1, t1, t2
    sw      t1, 0(t0)

    li      t0, GPIOC_PUD
    lw      t1, 0(t0)
    li      t2, PC13_FIELD_MASK
    not     t2, t2
    and     t1, t1, t2
    sw      t1, 0(t0)

    /* LED activo en bajo: iniciar apagado. */
    li      t0, GPIOC_BOP
    li      t1, PC13_BIT
    sw      t1, 0(t0)

    /* --------------------------------------------------------- */
    /* 3. Configurar SysTimer para periodo de 1 ms.              */
    /*    160 MHz / 4000 = 40000                                */
    /* --------------------------------------------------------- */

    li      t0, SYSTIMER_MTIMECTL
    li      t1, MTIMECTL_CMPCLREN
    sw      t1, 0(t0)

    /* Escritura segura del compare de 64 bits en RV32. */
    li      t0, SYSTIMER_CMP_LO
    li      t2, -1
    sw      t2, 0(t0)

    li      t0, SYSTIMER_CMP_HI
    sw      zero, 0(t0)

    li      t0, SYSTIMER_CMP_LO
    li      t1, SYSTIMER_COMPARE_1MS
    sw      t1, 0(t0)

    /* --------------------------------------------------------- */
    /* 4. Configurar ECLIC IRQ 7.                               */
    /* --------------------------------------------------------- */

    /* Clear pending. */
    li      t0, ECLIC_INTIP7
    sb      zero, 0(t0)

    /* Rising edge, non-vector attribute. */
    li      t0, ECLIC_INTATTR7
    li      t1, ECLIC_POS_EDGE
    sb      t1, 0(t0)

    /* Level/prioridad = 0, como la referencia. */
    li      t0, ECLIC_INTCTRL7
    sb      zero, 0(t0)

    /* Enable IRQ 7. */
    li      t0, ECLIC_INTIE7
    li      t1, 1
    sb      t1, 0(t0)

    /* Habilitar interrupciones globales M-mode: mstatus.MIE = 1. */
    li      t0, 0x8
    csrs    mstatus, t0

    /* --------------------------------------------------------- */
    /* 5. Consumidor: reclamar snapshot con amoswap.w.           */
    /* --------------------------------------------------------- */
    la      s0, g_event_flags
    li      s1, 0                  /* 0=lento, 1=rapido */
    li      s2, 0                  /* divisor del modo lento */
.Lmain_loop:
    amoswap.w.aqrl t0, zero, (s0)
    beqz    t0, .Lbackground
    la      t1, g_last_event_snapshot
    sw      t0, 0(t1)
    la      t1, g_event_batches
    lw      t2, 0(t1)
    addi    t2, t2, 1
    sw      t2, 0(t1)

    andi    t1, t0, EVENT_MODE_CHANGE
    beqz    t1, .Lcheck_second
    xori    s1, s1, 1
    li      s2, 0
    la      t1, g_mode_changes
    lw      t2, 0(t1)
    addi    t2, t2, 1
    sw      t2, 0(t1)
.Lcheck_second:
    andi    t1, t0, EVENT_ONE_SECOND
    beqz    t1, .Lcheck_led
    la      t1, g_seconds_handled
    lw      t2, 0(t1)
    addi    t2, t2, 1
    sw      t2, 0(t1)
.Lcheck_led:
    andi    t1, t0, EVENT_LED_TICK
    beqz    t1, .Lbackground
    bnez    s1, .Ltoggle_led
    addi    s2, s2, 1
    li      t1, 2
    bltu    s2, t1, .Lbackground
    li      s2, 0
.Ltoggle_led:
    li      t1, GPIOC_TG
    li      t2, PC13_BIT
    sw      t2, 0(t1)
    la      t1, g_led_toggle_count
    lw      t2, 0(t1)
    addi    t2, t2, 1
    sw      t2, 0(t1)
.Lbackground:
    la      t1, g_background_iterations
    lw      t2, 0(t1)
    addi    t2, t2, 1
    sw      t2, 0(t1)
    j       .Lmain_loop

    .size main, .-main


    /*
     * Handler llamado desde la infraestructura de interrupciones del startup.
     * El wrapper del startup administra el contexto de interrupcion.
     */
    .align 2
    .global eclic_mtip_handler
    .type eclic_mtip_handler, @function

eclic_mtip_handler:
    /* Limpiar pending IRQ 7. */
    li      t0, ECLIC_INTIP7
    sb      zero, 0(t0)

    /* Contadores de 1 ms y publicacion atomica de eventos. */
    la      t0, system_milliseconds
    lw      t1, 0(t0)
    addi    t1, t1, 1
    sw      t1, 0(t0)

    la      t0, led_tick_accumulator
    lw      t1, 0(t0)
    addi    t1, t1, 1
    li      t2, 250
    bltu    t1, t2, .Lstore_led_acc
    li      t1, 0
    la      t3, g_event_flags
    li      t4, EVENT_LED_TICK
    amoor.w.aqrl zero, t4, (t3)
.Lstore_led_acc:
    sw      t1, 0(t0)

    la      t0, second_accumulator
    lw      t1, 0(t0)
    addi    t1, t1, 1
    li      t2, 1000
    bltu    t1, t2, .Lstore_second_acc
    li      t1, 0
    la      t3, g_event_flags
    li      t4, EVENT_ONE_SECOND
    amoor.w.aqrl zero, t4, (t3)
.Lstore_second_acc:
    sw      t1, 0(t0)

    la      t0, mode_accumulator
    lw      t1, 0(t0)
    addi    t1, t1, 1
    li      t2, 5000
    bltu    t1, t2, .Lstore_mode_acc
    li      t1, 0
    la      t3, g_event_flags
    li      t4, EVENT_MODE_CHANGE
    amoor.w.aqrl zero, t4, (t3)
.Lstore_mode_acc:
    sw      t1, 0(t0)

    ret

    .size eclic_mtip_handler, .-eclic_mtip_handler


    .section .bss
    .align 2

    .global system_milliseconds
    .type system_milliseconds, @object

system_milliseconds:
    .word 0

    .size system_milliseconds, 4

    .global g_event_flags
    .global g_last_event_snapshot
    .global g_event_batches
    .global g_seconds_handled
    .global g_mode_changes
    .global g_led_toggle_count
    .global g_background_iterations
g_event_flags:             .word 0
g_last_event_snapshot:     .word 0
g_event_batches:           .word 0
g_seconds_handled:         .word 0
g_mode_changes:            .word 0
g_led_toggle_count:       .word 0
g_background_iterations:  .word 0
led_tick_accumulator:      .word 0
second_accumulator:        .word 0
mode_accumulator:          .word 0
