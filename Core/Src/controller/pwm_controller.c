#include "controller/pwm_controller.h"
#include "schedule_controller.h"
#include <stdbool.h>

static TIM_HandleTypeDef* p_pwm_htim = NULL;
static const NoteEvent_t* p_song = NULL;
static size_t song_length = 0;
static size_t song_index = 0;
static bool is_song_playing = false;

static void song_step_cb(void);
static void stop_note_callback(void);
static void stop_song_internal(void);
static void play_song_internal(const NoteEvent_t* song, size_t num_notes);

void PWM_Controller_Init(TIM_HandleTypeDef* htim) {
    p_pwm_htim = htim;
}

void PWM_Controller_Next_Song(void) {
    if (is_song_playing) {
        stop_song_internal();
    }

    play_song_internal(song_gravity_falls, GRAVITY_FALLS_LENGTH);
}

static void play_note_impl(float frequency, uint16_t duration) {
    (void)duration;
    if (p_pwm_htim == NULL) {
        return;
    }

    if (frequency <= 0.0f) {
        HAL_TIM_PWM_Stop(p_pwm_htim, TIM_CHANNEL_1);
    } else {
        float timer_clock = 80000000.0f; /* 80 MHz */
        uint32_t period = 1000;
        uint32_t prescaler = (uint32_t)((timer_clock / (frequency * (float)period)) - 1.0f);

        if (prescaler > 65535)
            prescaler = 65535;
        if (prescaler < 1)
            prescaler = 1;

        __HAL_TIM_SET_PRESCALER(p_pwm_htim, prescaler);
        __HAL_TIM_SET_AUTORELOAD(p_pwm_htim, period);

        __HAL_TIM_SET_COMPARE(p_pwm_htim, TIM_CHANNEL_1, /* (period / 100)*1 */ 5);
        // 1% duty cycle for square wave

        HAL_TIM_PWM_Start(p_pwm_htim, TIM_CHANNEL_1);
    }
}

static void stop_note_callback(void) {
    if (p_pwm_htim != NULL) {
        HAL_TIM_PWM_Stop(p_pwm_htim, TIM_CHANNEL_1);
    }
}

static void stop_song_internal(void) {
    ScheduleController_Cancel(song_step_cb);
    ScheduleController_Cancel(stop_note_callback);

    p_song = NULL;
    song_length = 0;
    song_index = 0;
    is_song_playing = false;

    stop_note_callback();
}

static void play_note(float frequency, uint16_t duration) {
    play_note_impl(frequency, duration);
    ScheduleController_Schedule(duration, stop_note_callback);
}

static void song_step_cb(void) {
    if (!is_song_playing || p_song == NULL || song_index >= song_length) {
        is_song_playing = false;
        return;
    }

    const NoteEvent_t note = p_song[song_index++];
    play_note(note.frequency, note.duration);

    if (song_index < song_length) {
        ScheduleController_Schedule(note.duration, song_step_cb);
    } else {
        is_song_playing = false;
    }
}

static void play_song_internal(const NoteEvent_t* song, size_t num_notes) {
    p_song = song;
    song_length = num_notes;
    song_index = 0;
    is_song_playing = true;

    if (p_song == NULL || song_length == 0) {
        stop_note_callback();
        is_song_playing = false;
        return;
    }

    song_step_cb();
}
