#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

enum {
    EVENT_LED_TICK    = (1UL << 0),
    EVENT_ONE_SECOND  = (1UL << 1),
    EVENT_MODE_CHANGE = (1UL << 2)
};

extern volatile uint32_t g_event_flags;

static inline void event_publish(uint32_t mask)
{
    __atomic_fetch_or(&g_event_flags, mask, __ATOMIC_RELEASE);
}

static inline uint32_t event_claim_all(void)
{
    return __atomic_exchange_n(&g_event_flags, 0U, __ATOMIC_ACQ_REL);
}

#endif
