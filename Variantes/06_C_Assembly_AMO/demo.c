#include <stdint.h>
#include "event_amo.h"

enum {
    EVENT_A = (1UL << 0),
    EVENT_B = (1UL << 1),
    EVENT_C = (1UL << 2)
};

static volatile uint32_t flags = 0U;

int main(void)
{
    event_publish_amo(&flags, EVENT_A);
    event_publish_amo(&flags, EVENT_C);

    volatile uint32_t snapshot = event_claim_amo(&flags);

    /*
     * Esperado:
     * snapshot = 0x05
     * flags    = 0x00
     */
    (void)snapshot;

    for (;;) {
    }
}
