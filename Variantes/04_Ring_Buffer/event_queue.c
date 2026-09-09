#include "event_queue.h"

#define EVENT_QUEUE_CAPACITY 16U

static event_type_t queue[EVENT_QUEUE_CAPACITY];
static volatile uint32_t head = 0U;
static volatile uint32_t tail = 0U;
static volatile uint32_t dropped = 0U;

/*
 * Modelo SPSC:
 *   un productor: ISR
 *   un consumidor: main
 *
 * Head y tail son publicados/cargados atomicamente.
 */
uint32_t event_queue_push_from_isr(event_type_t event)
{
    const uint32_t local_head =
        __atomic_load_n(&head, __ATOMIC_RELAXED);
    const uint32_t next =
        (local_head + 1U) % EVENT_QUEUE_CAPACITY;
    const uint32_t local_tail =
        __atomic_load_n(&tail, __ATOMIC_ACQUIRE);

    if (next == local_tail) {
        __atomic_fetch_add(&dropped, 1U, __ATOMIC_RELAXED);
        return 0U;
    }

    queue[local_head] = event;
    __atomic_store_n(&head, next, __ATOMIC_RELEASE);
    return 1U;
}

uint32_t event_queue_pop(event_type_t *event)
{
    const uint32_t local_tail =
        __atomic_load_n(&tail, __ATOMIC_RELAXED);
    const uint32_t local_head =
        __atomic_load_n(&head, __ATOMIC_ACQUIRE);

    if (local_tail == local_head) {
        return 0U;
    }

    *event = queue[local_tail];

    __atomic_store_n(
        &tail,
        (local_tail + 1U) % EVENT_QUEUE_CAPACITY,
        __ATOMIC_RELEASE
    );

    return 1U;
}

uint32_t event_queue_dropped(void)
{
    return __atomic_load_n(&dropped, __ATOMIC_RELAXED);
}
