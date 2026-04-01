#include "schedule_controller.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint32_t expire_tick;
  schedule_callback_t cb;
} sc_entry_t;

static sc_entry_t *g_entries = NULL;
static size_t g_count = 0;
static size_t g_cap = 0;

#define SC_INITIAL_CAP 16

static int grow_to_fit(size_t needed)
{
  if (g_cap >= needed) return 0;
  size_t newcap = g_cap ? g_cap * 2 : SC_INITIAL_CAP;
  while (newcap < needed) newcap *= 2;
  sc_entry_t *p = realloc(g_entries, newcap * sizeof(sc_entry_t));
  if (!p) return -1;
  g_entries = p;
  g_cap = newcap;
  return 0;
}

void ScheduleController_Init(void)
{
  g_count = 0;
  if (!g_entries) {
    g_entries = malloc(SC_INITIAL_CAP * sizeof(sc_entry_t));
    if (g_entries) g_cap = SC_INITIAL_CAP;
  }
}

int ScheduleController_Schedule(uint32_t ms, schedule_callback_t cb)
{
  if (!cb) return -1;
  uint32_t now = HAL_GetTick();
  uint32_t expire = now + ms;

  __disable_irq();
  if (g_cap < (g_count + 1)) {
    if (grow_to_fit(g_count + 1) != 0) {
      __enable_irq();
      return -1;
    }
  }

  g_entries[g_count].expire_tick = expire;
  g_entries[g_count].cb = cb;
  g_count++;
  __enable_irq();
  return 0;
}

int ScheduleController_ScheduleFromISR(uint32_t ms, schedule_callback_t cb)
{
  if (!cb) return -1;
  uint32_t now = HAL_GetTick();
  uint32_t expire = now + ms;

  __disable_irq();
  if (g_count >= g_cap) {
    __enable_irq();
    return -1;
  }
  g_entries[g_count].expire_tick = expire;
  g_entries[g_count].cb = cb;
  g_count++;
  __enable_irq();
  return 0;
}

void ScheduleController_Process(void)
{
  __disable_irq();
  size_t n = g_count;
  if (n == 0) { __enable_irq(); return; }

  sc_entry_t *snapshot = malloc(n * sizeof(sc_entry_t));
  if (!snapshot) { __enable_irq(); return; }

  memcpy(snapshot, g_entries, n * sizeof(sc_entry_t));
  g_count = 0;
  __enable_irq();

  uint32_t now = HAL_GetTick();

  for (size_t i = 0; i < n; ++i) {
    sc_entry_t *e = &snapshot[i];
    if ((int32_t)(now - e->expire_tick) >= 0) {
      if (e->cb) e->cb();
    } else {
      uint32_t remaining = e->expire_tick - now;
      (void)ScheduleController_Schedule(remaining, e->cb);
    }
  }

  free(snapshot);
}
