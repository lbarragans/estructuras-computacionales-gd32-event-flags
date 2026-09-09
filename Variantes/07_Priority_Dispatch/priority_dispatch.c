#include <stdint.h>

enum {
    EVENT_LED_TICK    = (1UL << 0),
    EVENT_ONE_SECOND  = (1UL << 1),
    EVENT_MODE_CHANGE = (1UL << 2)
};

volatile uint32_t g_dispatch_trace[3];
volatile uint32_t g_dispatch_trace_count = 0U;

static void trace(uint32_t event)
{
    if (g_dispatch_trace_count < 3U) {
        g_dispatch_trace[g_dispatch_trace_count++] = event;
    }
}

void dispatch_priority(uint32_t snapshot)
{
    g_dispatch_trace_count = 0U;

    /* Prioridad de software: mayor importancia primero. */
    if ((snapshot & EVENT_MODE_CHANGE) != 0U) {
        trace(EVENT_MODE_CHANGE);
    }

    if ((snapshot & EVENT_ONE_SECOND) != 0U) {
        trace(EVENT_ONE_SECOND);
    }

    if ((snapshot & EVENT_LED_TICK) != 0U) {
        trace(EVENT_LED_TICK);
    }
}
