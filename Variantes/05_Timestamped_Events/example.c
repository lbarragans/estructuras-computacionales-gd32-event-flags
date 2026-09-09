#include <stdint.h>
#include "event_record.h"

volatile event_record_t g_last_event;

void handle_event_record(const event_record_t *event)
{
    g_last_event = *event;

    switch (event->type) {
    case EVENT_LED_TICK:
        break;

    case EVENT_ONE_SECOND:
        break;

    case EVENT_MODE_CHANGE:
        break;

    case EVENT_SENSOR_SAMPLE:
        /*
         * event->timestamp_ms indica cuando ocurrio.
         * event->value transporta la muestra.
         */
        break;

    default:
        break;
    }
}
