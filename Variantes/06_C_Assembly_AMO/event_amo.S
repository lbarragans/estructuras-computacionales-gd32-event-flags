.section .text
    .align 2

    /*
     * void event_publish_amo(volatile uint32_t *flags, uint32_t mask)
     *
     * a0 = address
     * a1 = mask
     */
    .global event_publish_amo
    .type event_publish_amo, @function

event_publish_amo:
    amoor.w.aqrl zero, a1, (a0)
    ret

    .size event_publish_amo, .-event_publish_amo

    /*
     * uint32_t event_claim_amo(volatile uint32_t *flags)
     *
     * a0 = address, then previous value
     */
    .global event_claim_amo
    .type event_claim_amo, @function

event_claim_amo:
    mv          t0, a0
    li          t1, 0
    amoswap.w.aqrl a0, t1, (t0)
    ret

    .size event_claim_amo, .-event_claim_amo
