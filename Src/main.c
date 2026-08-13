#include <stdint.h>

#include "events.h"
#include "gd32vw55x.h"

#define LED_GPIO_PORT       GPIOC
#define LED_GPIO_PIN        GPIO_PIN_13
#define LED_GPIO_CLOCK      RCU_GPIOC

typedef enum {
    BLINK_MODE_SLOW = 0,
    BLINK_MODE_FAST
} blink_mode_t;

volatile blink_mode_t g_blink_mode = BLINK_MODE_SLOW;
volatile uint32_t g_last_event_snapshot = 0U;
volatile uint32_t g_event_batches = 0U;
volatile uint32_t g_seconds_handled = 0U;
volatile uint32_t g_mode_changes = 0U;
volatile uint32_t g_led_toggle_count = 0U;
volatile uint32_t g_background_iterations = 0U;
volatile uint8_t g_led_is_on = 0U;

static uint8_t slow_tick_divider = 0U;

static void led_init(void)
{
    rcu_periph_clock_enable(LED_GPIO_CLOCK);

    gpio_mode_set(
        LED_GPIO_PORT,
        GPIO_MODE_OUTPUT,
        GPIO_PUPD_NONE,
        LED_GPIO_PIN
    );

    gpio_output_options_set(
        LED_GPIO_PORT,
        GPIO_OTYPE_PP,
        GPIO_OSPEED_10MHZ,
        LED_GPIO_PIN
    );

    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

static void led_toggle(void)
{
    if (g_led_is_on != 0U) {
        gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
        g_led_is_on = 0U;
    } else {
        gpio_bit_reset(LED_GPIO_PORT, LED_GPIO_PIN);
        g_led_is_on = 1U;
    }

    g_led_toggle_count++;
}

static uint32_t claim_pending_events(void)
{
    /* Obtiene una fotografia y limpia las banderas de forma indivisible. */
    return __atomic_exchange_n(&g_event_flags, 0U, __ATOMIC_ACQ_REL);
}

static void handle_mode_change_event(void)
{
    if (g_blink_mode == BLINK_MODE_SLOW) {
        g_blink_mode = BLINK_MODE_FAST;
    } else {
        g_blink_mode = BLINK_MODE_SLOW;
    }

    slow_tick_divider = 0U;
    g_mode_changes++;
}

static void handle_led_tick_event(void)
{
    if (g_blink_mode == BLINK_MODE_FAST) {
        led_toggle();
        return;
    }

    slow_tick_divider++;
    if (slow_tick_divider >= 2U) {
        slow_tick_divider = 0U;
        led_toggle();
    }
}

static void dispatch_events(uint32_t events)
{
    if ((events & EVENT_MODE_CHANGE) != 0U) {
        handle_mode_change_event();
    }

    if ((events & EVENT_ONE_SECOND) != 0U) {
        g_seconds_handled++;
    }

    if ((events & EVENT_LED_TICK) != 0U) {
        handle_led_tick_event();
    }
}

static void background_task_update(void)
{
    g_background_iterations++;
}

int main(void)
{
    led_init();
    event_timer_init_1ms();

    while (1) {
        uint32_t pending_events = claim_pending_events();

        if (pending_events != 0U) {
            g_last_event_snapshot = pending_events;
            g_event_batches++;
            dispatch_events(pending_events);
        }

        background_task_update();
    }
}
