#ifndef SCHEDULE_CONTROLLER_H
#define SCHEDULE_CONTROLLER_H

#include "main.h"
#include <stddef.h>
#include <stdint.h>

typedef void (*schedule_callback_t)(void);

void ScheduleController_Init(void);
void ScheduleController_Process(void);
int ScheduleController_Schedule(uint32_t ms, schedule_callback_t cb);
void ScheduleController_Cancel(schedule_callback_t cb);
int ScheduleController_ScheduleFromISR(uint32_t ms, schedule_callback_t cb);

#endif /* SCHEDULE_CONTROLLER_H */
