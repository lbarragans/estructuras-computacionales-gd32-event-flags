#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

enum {
    EVENT_LED_TICK    = (1UL << 0),
    EVENT_ONE_SECOND  = (1UL << 1),
    EVENT_MODE_CHANGE = (1UL << 2)
};

extern volatile uint32_t g_event_flags;
extern volatile uint32_t g_isr_count;
extern volatile uint32_t g_events_published;

void event_timer_init_1ms(void);
uint32_t event_timer_millis(void);

#endif /* EVENTS_H */
