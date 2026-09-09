#include <stdint.h>

#define EVENT_TEST (1UL << 0)

static volatile uint32_t flags = 0U;
volatile uint32_t g_publish_calls = 0U;
volatile uint32_t g_claim_batches = 0U;
volatile uint32_t g_flag_observations = 0U;

void lab_publish(void)
{
    __atomic_fetch_or(&flags, EVENT_TEST, __ATOMIC_RELEASE);
    g_publish_calls++;
}

void lab_claim(void)
{
    const uint32_t snapshot =
        __atomic_exchange_n(&flags, 0U, __ATOMIC_ACQ_REL);

    g_claim_batches++;

    if ((snapshot & EVENT_TEST) != 0U) {
        g_flag_observations++;
    }
}

/*
 * Experimento sugerido:
 *
 * lab_publish();
 * lab_publish();
 * lab_publish();
 * lab_claim();
 *
 * publish_calls      = 3
 * flag_observations  = 1
 *
 * Esto no es una perdida accidental del bitmask:
 * es su semantica "pendiente / no pendiente".
 */
