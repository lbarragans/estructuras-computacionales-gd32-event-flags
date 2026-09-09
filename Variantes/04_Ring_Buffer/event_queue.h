#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdint.h>

typedef enum {
    EVENT_TYPE_LED_TICK = 0,
    EVENT_TYPE_ONE_SECOND,
    EVENT_TYPE_MODE_CHANGE
} event_type_t;

uint32_t event_queue_push_from_isr(event_type_t event);
uint32_t event_queue_pop(event_type_t *event);
uint32_t event_queue_dropped(void);

#endif
