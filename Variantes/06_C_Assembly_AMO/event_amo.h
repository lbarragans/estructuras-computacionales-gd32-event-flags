#ifndef EVENT_AMO_H
#define EVENT_AMO_H

#include <stdint.h>

void event_publish_amo(volatile uint32_t *flags, uint32_t mask);
uint32_t event_claim_amo(volatile uint32_t *flags);

#endif
