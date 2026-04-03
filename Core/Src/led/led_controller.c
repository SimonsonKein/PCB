#include "led/led_controller.h"
#include "schedule_controller.h"

static LED_Animation_t current_animation = LED_OFF;
static uint32_t last_tick = 0;
static uint32_t blink_end_tick = 0;

void LED_Controller_Toggle(void) {
    GPIOA->ODR ^= GPIO_ODR_OD5;
}

static void turn_off_led(void) {
    GPIOA->BRR = (1U << 5);
}

static void turn_on_led(void) {
    GPIOA->BSRR = (1U << 5);
}

static void blink_toggle_cb(void);
static void blink_restart_cb(void);
static void short_pulse_on_cb(void);
static void short_pulse_off_cb(void);
static void long_pulse_on_cb(void);
static void long_pulse_off_cb(void);

void LED_Controller_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    GPIOA->MODER &= ~GPIO_MODER_MODE5;
    GPIOA->MODER |= GPIO_MODER_MODE5_0;
    turn_off_led();
}

void LED_Controller_SetAnimation(LED_Animation_t animation) {
    current_animation = animation;
    last_tick = HAL_GetTick();

    switch (current_animation) {
    case LED_OFF:
        turn_off_led();
        break;
    case LED_ON:
        turn_on_led();
        break;
    case LED_BLINK:
        blink_end_tick = HAL_GetTick() + 600;
        turn_on_led();
        ScheduleController_Schedule(50, blink_toggle_cb);
        break;
    case LED_SHORT_PULSE:
        short_pulse_on_cb();
        break;
    case LED_LONG_PULSE:
        long_pulse_on_cb();
        break;
    }
}

/* convenience helpers */
void LED_Controller_NextAnimation(void) {
    LED_Animation_t next = (LED_Animation_t)((current_animation + 1) % 5);
    LED_Controller_SetAnimation(next);
}

void LED_Controller_PrevAnimation(void) {
    LED_Animation_t prev = (LED_Animation_t)((current_animation + 5 - 1) % 5);
    LED_Controller_SetAnimation(prev);
}

/* --- Scheduled callback implementations --- */

/* Blink: toggle and reschedule only if still in BLINK animation */
static void blink_toggle_cb(void) {
    if (current_animation != LED_BLINK)
        return;

    if ((int32_t)(HAL_GetTick() - blink_end_tick) >= 0) {
        turn_off_led();
        ScheduleController_Schedule(300, blink_restart_cb);
        return;
    }

    LED_Controller_Toggle();
    ScheduleController_Schedule(50, blink_toggle_cb);
}

static void blink_restart_cb(void) {
    if (current_animation != LED_BLINK)
        return;

    blink_end_tick = HAL_GetTick() + 600;
    turn_on_led();
    ScheduleController_Schedule(50, blink_toggle_cb);
}

/* Short pulse: on 300ms, off 700ms repeating */
static void short_pulse_on_cb(void) {
    if (current_animation != LED_SHORT_PULSE)
        return;
    turn_on_led();
    /* schedule off after 300ms */
    ScheduleController_Schedule(300, short_pulse_off_cb);
}
static void short_pulse_off_cb(void) {
    if (current_animation != LED_SHORT_PULSE)
        return;
    turn_off_led();
    /* schedule on after 700ms */
    ScheduleController_Schedule(700, short_pulse_on_cb);
}

/* Long pulse: on 500ms, off 500ms repeating */
static void long_pulse_on_cb(void) {
    if (current_animation != LED_LONG_PULSE)
        return;
    turn_on_led();
    ScheduleController_Schedule(500, long_pulse_off_cb);
}
static void long_pulse_off_cb(void) {
    if (current_animation != LED_LONG_PULSE)
        return;
    turn_off_led();
    ScheduleController_Schedule(500, long_pulse_on_cb);
}
