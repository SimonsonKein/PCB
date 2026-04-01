#ifndef SCHEDULE_CONTROLLER_H
#define SCHEDULE_CONTROLLER_H

#include "main.h"
#include <stdint.h>
#include <stddef.h>

typedef void (*schedule_callback_t)(void);

void ScheduleController_Init(void);

int ScheduleController_Schedule(uint32_t ms, schedule_callback_t cb);

int ScheduleController_ScheduleFromISR(uint32_t ms, schedule_callback_t cb);

void ScheduleController_Process(void);

#endif /* SCHEDULE_CONTROLLER_H */
