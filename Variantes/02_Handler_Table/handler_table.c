#include <stdint.h>

enum {
    EVENT_LED_TICK    = (1UL << 0),
    EVENT_ONE_SECOND  = (1UL << 1),
    EVENT_MODE_CHANGE = (1UL << 2)
};

typedef void (*event_handler_t)(void);

typedef struct {
    uint32_t mask;
    event_handler_t handler;
} event_route_t;

volatile uint32_t g_led_ticks_handled = 0U;
volatile uint32_t g_seconds_handled = 0U;
volatile uint32_t g_mode_changes_handled = 0U;

static void on_led_tick(void)
{
    g_led_ticks_handled++;
}

static void on_one_second(void)
{
    g_seconds_handled++;
}

static void on_mode_change(void)
{
    g_mode_changes_handled++;
}

/* El orden de la tabla define el orden de despacho. */
static const event_route_t routes[] = {
    {EVENT_MODE_CHANGE, on_mode_change},
    {EVENT_ONE_SECOND,  on_one_second},
    {EVENT_LED_TICK,    on_led_tick}
};

void dispatch_event_snapshot(uint32_t events)
{
    const uint32_t count = sizeof(routes) / sizeof(routes[0]);

    for (uint32_t i = 0U; i < count; ++i) {
        if ((events & routes[i].mask) != 0U) {
            routes[i].handler();
        }
    }
}
