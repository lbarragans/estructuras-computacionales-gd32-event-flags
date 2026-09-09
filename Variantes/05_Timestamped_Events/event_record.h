#ifndef EVENT_RECORD_H
#define EVENT_RECORD_H

#include <stdint.h>

typedef enum {
    EVENT_LED_TICK = 0,
    EVENT_ONE_SECOND,
    EVENT_MODE_CHANGE,
    EVENT_SENSOR_SAMPLE
} event_type_t;

typedef struct {
    event_type_t type;
    uint32_t timestamp_ms;
    uint32_t value;
} event_record_t;

#endif
