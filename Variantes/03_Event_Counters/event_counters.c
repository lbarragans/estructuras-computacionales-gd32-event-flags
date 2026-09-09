#include "event_counters.h"

static volatile uint32_t led_tick_count = 0U;
static volatile uint32_t one_second_count = 0U;
static volatile uint32_t mode_change_count = 0U;

void event_count_publish_led_tick(void)
{
    __atomic_fetch_add(&led_tick_count, 1U, __ATOMIC_RELEASE);
}

void event_count_publish_one_second(void)
{
    __atomic_fetch_add(&one_second_count, 1U, __ATOMIC_RELEASE);
}

void event_count_publish_mode_change(void)
{
    __atomic_fetch_add(&mode_change_count, 1U, __ATOMIC_RELEASE);
}

event_counts_t event_count_claim_all(void)
{
    event_counts_t result;

    result.led_tick =
        __atomic_exchange_n(&led_tick_count, 0U, __ATOMIC_ACQ_REL);
    result.one_second =
        __atomic_exchange_n(&one_second_count, 0U, __ATOMIC_ACQ_REL);
    result.mode_change =
        __atomic_exchange_n(&mode_change_count, 0U, __ATOMIC_ACQ_REL);

    return result;
}
