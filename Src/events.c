#include <stdint.h>

#include "events.h"
#include "gd32vw55x.h"

volatile uint32_t g_event_flags = 0U;
volatile uint32_t g_isr_count = 0U;
volatile uint32_t g_events_published = 0U;

static volatile uint32_t system_milliseconds = 0U;
static uint16_t led_tick_accumulator = 0U;
static uint16_t second_accumulator = 0U;
static uint16_t mode_accumulator = 0U;

static void publish_event(uint32_t event_mask)
{
    /* La ISR agrega eventos sin sobrescribir los que main no ha procesado. */
    __atomic_fetch_or(&g_event_flags, event_mask, __ATOMIC_RELEASE);
    g_events_published++;
}

void event_timer_init_1ms(void)
{
    SysTimer_SetControlValue(SysTimer_MTIMECTL_CMPCLREN_Msk);
    SysTimer_SetCompareValue(SystemCoreClock / 4000U);

    __ECLIC_SetTrigIRQ(CLIC_INT_TMR, ECLIC_POSTIVE_EDGE_TRIGGER);
    ECLIC_ClearPendingIRQ(CLIC_INT_TMR);
    eclic_irq_enable(CLIC_INT_TMR, 0U, 0U);
}

uint32_t event_timer_millis(void)
{
    return system_milliseconds;
}

/* La interrupcion publica eventos y nunca controla directamente el LED. */
void eclic_mtip_handler(void)
{
    ECLIC_ClearPendingIRQ(CLIC_INT_TMR);

    system_milliseconds++;
    g_isr_count++;
    led_tick_accumulator++;
    second_accumulator++;
    mode_accumulator++;

    if (led_tick_accumulator >= 250U) {
        led_tick_accumulator = 0U;
        publish_event(EVENT_LED_TICK);
    }

    if (second_accumulator >= 1000U) {
        second_accumulator = 0U;
        publish_event(EVENT_ONE_SECOND);
    }

    if (mode_accumulator >= 5000U) {
        mode_accumulator = 0U;
        publish_event(EVENT_MODE_CHANGE);
    }
}
