#include "schedule_controller.h"
#include <stdlib.h>
#include <string.h>

#define SC_INITIAL_CAP 16
#define SC_MAX_CALLBACKS_PER_PROCESS 8

typedef struct {
    uint32_t expire_tick;
    schedule_callback_t cb;
} sc_entry_t;

static sc_entry_t* entries = NULL;
static size_t count = 0;
static size_t cap = 0;

static int grow_to_fit(size_t needed);

void ScheduleController_Init(void) {
    count = 0;
    if (!entries) {
        entries = malloc(SC_INITIAL_CAP * sizeof(sc_entry_t));
        if (entries)
            cap = SC_INITIAL_CAP;
    }
}
/**
 * Schedules a callback to be executed after a specified delay.
 * @param ms The delay in milliseconds.
 * @param cb The callback function to schedule.
 * @brief Schedules a callback to be executed after a specified delay. IRQ manipulation is handled
 internally to ensure thread safety.
 * @return 0 on success, -1 on failure.

 */
int ScheduleController_Schedule(uint32_t ms, schedule_callback_t cb) {
    if (!cb)
        return -1;
    uint32_t now = HAL_GetTick();
    uint32_t expire = now + ms;

    __disable_irq();
    if (cap < (count + 1)) {
        if (grow_to_fit(count + 1) != 0) {
            __enable_irq();
            return -1;
        }
    }

    size_t insert = count;
    while (insert > 0 && (int32_t)(expire - entries[insert - 1].expire_tick) < 0) {
        entries[insert] = entries[insert - 1];
        insert--;
    }

    entries[insert].expire_tick = expire;
    entries[insert].cb = cb;
    count++;
    __enable_irq();
    return 0;
}

int ScheduleController_ScheduleFromISR(uint32_t ms, schedule_callback_t cb) {
    if (!cb)
        return -1;
    uint32_t now = HAL_GetTick();
    uint32_t expire = now + ms;

    __disable_irq();
    if (count >= cap) {
        __enable_irq();
        return -1;
    }

    size_t insert = count;
    while (insert > 0 && (int32_t)(expire - entries[insert - 1].expire_tick) < 0) {
        entries[insert] = entries[insert - 1];
        insert--;
    }

    entries[insert].expire_tick = expire;
    entries[insert].cb = cb;
    count++;
    __enable_irq();
    return 0;
}

void ScheduleController_Cancel(schedule_callback_t cb) {
    if (!cb)
        return;

    __disable_irq();

    size_t write_index = 0;
    for (size_t read_index = 0; read_index < count; ++read_index) {
        if (entries[read_index].cb != cb) {
            entries[write_index++] = entries[read_index];
        }
    }

    count = write_index;
    __enable_irq();
}

void ScheduleController_Process(void) {
    size_t processed = 0;

    while (processed < SC_MAX_CALLBACKS_PER_PROCESS) {
        schedule_callback_t due_cb = NULL;

        __disable_irq();

        if (count == 0) {
            __enable_irq();
            return;
        }

        uint32_t now = HAL_GetTick();

        if ((int32_t)(now - entries[0].expire_tick) >= 0) {
            due_cb = entries[0].cb;
            for (size_t i = 1; i < count; ++i) {
                entries[i - 1] = entries[i];
            }
            count--;
        }

        __enable_irq();

        if (!due_cb) {
            return;
        }

        due_cb();
        processed++;
    }
}

static int grow_to_fit(size_t needed) {
    if (cap >= needed)
        return 0;
    size_t newcap = cap ? cap * 2 : SC_INITIAL_CAP;
    while (newcap < needed)
        newcap *= 2;
    sc_entry_t* p = realloc(entries, newcap * sizeof(sc_entry_t));
    if (!p)
        return -1;
    entries = p;
    cap = newcap;
    return 0;
}