#include <stdint.h>

#include "FreeRTOS.h"
#include "event_groups.h"
#include "task.h"

#define EVT_LED_TICK    (1UL << 0)
#define EVT_ONE_SECOND  (1UL << 1)
#define EVT_MODE_CHANGE (1UL << 2)

static EventGroupHandle_t event_group;

static volatile uint32_t handled_led_ticks = 0U;
static volatile uint32_t handled_seconds = 0U;
static volatile uint32_t handled_mode_changes = 0U;

/*
 * Productor pedagogico por tarea.
 * En una integracion con ISR se debe usar la API ISR-safe apropiada para
 * Event Groups de la version/port utilizado.
 */
static void producer_task(void *argument)
{
    (void)argument;

    uint32_t ticks_250 = 0U;

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(250U));
        ticks_250++;

        EventBits_t bits = EVT_LED_TICK;

        if ((ticks_250 % 4U) == 0U) {
            bits |= EVT_ONE_SECOND;
        }

        if ((ticks_250 % 20U) == 0U) {
            bits |= EVT_MODE_CHANGE;
        }

        (void)xEventGroupSetBits(event_group, bits);
    }
}

static void consumer_task(void *argument)
{
    (void)argument;

    const EventBits_t all_bits =
        EVT_LED_TICK | EVT_ONE_SECOND | EVT_MODE_CHANGE;

    for (;;) {
        const EventBits_t bits = xEventGroupWaitBits(
            event_group,
            all_bits,
            pdTRUE,     /* limpiar bits devueltos */
            pdFALSE,    /* esperar cualquiera */
            portMAX_DELAY
        );

        if ((bits & EVT_MODE_CHANGE) != 0U) {
            handled_mode_changes++;
        }

        if ((bits & EVT_ONE_SECOND) != 0U) {
            handled_seconds++;
        }

        if ((bits & EVT_LED_TICK) != 0U) {
            handled_led_ticks++;
        }
    }
}

int main(void)
{
    event_group = xEventGroupCreate();

    if (event_group == NULL) {
        for (;;) {
        }
    }

    (void)xTaskCreate(
        producer_task,
        "Producer",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1U,
        NULL
    );

    (void)xTaskCreate(
        consumer_task,
        "Consumer",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2U,
        NULL
    );

    vTaskStartScheduler();

    for (;;) {
    }
}
