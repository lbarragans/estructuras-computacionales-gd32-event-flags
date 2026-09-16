#include <stdint.h>

#include "app_cfg.h"
#include "gd32vw55x_platform.h"
#include "wrapper_os.h"

#include "gd32vw55x.h"

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
static volatile uint8_t fast_mode = 0U;
static uint8_t slow_tick_divider = 0U;

#define LED_GPIO_PORT  GPIOC
#define LED_GPIO_PIN   GPIO_PIN_13
#define LED_GPIO_CLOCK RCU_GPIOC

static void led_init(void)
{
    rcu_periph_clock_enable(LED_GPIO_CLOCK);
    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
    gpio_output_options_set(
        LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, LED_GPIO_PIN
    );
    gpio_bit_set(LED_GPIO_PORT, LED_GPIO_PIN);
}

static void led_toggle(void)
{
    gpio_bit_toggle(LED_GPIO_PORT, LED_GPIO_PIN);
}

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
            fast_mode ^= 1U;
            slow_tick_divider = 0U;
        }

        if ((bits & EVT_ONE_SECOND) != 0U) {
            handled_seconds++;
        }

        if ((bits & EVT_LED_TICK) != 0U) {
            handled_led_ticks++;
            if (fast_mode != 0U) {
                led_toggle();
            } else {
                slow_tick_divider++;
                if (slow_tick_divider >= 2U) {
                    slow_tick_divider = 0U;
                    led_toggle();
                }
            }
        }
    }
}

int main(void)
{
    sys_os_init();
    platform_init();
    led_init();
    event_group = xEventGroupCreate();

    if (event_group == NULL) {
        for (;;) {
        }
    }

    BaseType_t producer_ok = xTaskCreate(
        producer_task,
        "Producer",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1U,
        NULL
    );

    BaseType_t consumer_ok = xTaskCreate(
        consumer_task,
        "Consumer",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2U,
        NULL
    );

    if ((producer_ok != pdPASS) || (consumer_ok != pdPASS)) {
        for (;;) {
        }
    }

    sys_os_start();

    for (;;) {
    }
}
