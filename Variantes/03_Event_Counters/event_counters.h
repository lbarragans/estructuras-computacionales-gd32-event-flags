#ifndef EVENT_COUNTERS_H
#define EVENT_COUNTERS_H

#include <stdint.h>

typedef struct {
    uint32_t led_tick;
    uint32_t one_second;
    uint32_t mode_change;
} event_counts_t;

void event_count_publish_led_tick(void);
void event_count_publish_one_second(void);
void event_count_publish_mode_change(void);
event_counts_t event_count_claim_all(void);

#endif
