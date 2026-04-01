#include "utils/utils.h"


#define DELAY_SLOTS 8

typedef void (*delay_cb_t)(void);

typedef struct {
  volatile uint8_t active;    /* 0 = free, 1 = armed */
  uint32_t expire_tick;       /* absolute tick when callback is due */
  delay_cb_t cb;              /* callback to call when expired */
} delay_slot_t;

static delay_slot_t delay_slots[DELAY_SLOTS] = {0};
