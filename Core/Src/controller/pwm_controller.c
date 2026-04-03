#include "controller/pwm_controller.h"
#include "schedule_controller.h"

#define GRAVITY_FALLS_LENGTH                                                   \
  (sizeof(song_gravity_falls) / sizeof(song_gravity_falls[0]))

static TIM_HandleTypeDef *s_pwm_htim = NULL;
static const NoteEvent_t *s_song = NULL;
static size_t s_song_length = 0;
static size_t s_song_index = 0;

static void play_note_impl(float frequency, uint16_t duration) {
  (void)duration;
  if (s_pwm_htim == NULL) {
    return;
  }

  if (frequency <= 0.0f) {
    /* Rest/silence */
    HAL_TIM_PWM_Stop(s_pwm_htim, TIM_CHANNEL_1);
  } else {
    /* Calculate timer parameters for desired frequency
     * System clock: 80 MHz
     * Formula: freq = timer_clock / (prescaler * (period + 1))
     * We use period = 1000 for good resolution
     */
    float timer_clock = 80000000.0f; /* 80 MHz */
    uint32_t period = 1000;
    uint32_t prescaler =
        (uint32_t)((timer_clock / (frequency * (float)period)) - 1.0f);

    /* Limit prescaler to 16-bit */
    if (prescaler > 65535)
      prescaler = 65535;
    if (prescaler < 1)
      prescaler = 1;

    __HAL_TIM_SET_PRESCALER(s_pwm_htim, prescaler);
    __HAL_TIM_SET_AUTORELOAD(s_pwm_htim, period);

    __HAL_TIM_SET_COMPARE(s_pwm_htim, TIM_CHANNEL_1, /* (period / 100)*1 */ 5);
    // 1% duty cycle for square wave

    HAL_TIM_PWM_Start(s_pwm_htim, TIM_CHANNEL_1);
  }
}

static void stop_note_callback(void) {
  if (s_pwm_htim != NULL) {
    HAL_TIM_PWM_Stop(s_pwm_htim, TIM_CHANNEL_1);
  }
}

static void play_note(float frequency, uint16_t duration) {
  play_note_impl(frequency, duration);
  ScheduleController_Schedule(duration, stop_note_callback);
}

static void song_step_cb(void) {
  if (s_song == NULL || s_song_index >= s_song_length) {
    return;
  }

  const NoteEvent_t note = s_song[s_song_index++];
  play_note(note.frequency, note.duration);

  if (s_song_index < s_song_length) {
    ScheduleController_Schedule(note.duration, song_step_cb);
  }
}

static void play_song_internal(const NoteEvent_t *song, size_t num_notes) {
  s_song = song;
  s_song_length = num_notes;
  s_song_index = 0;

  if (s_song == NULL || s_song_length == 0) {
    stop_note_callback();
    return;
  }

  song_step_cb();
}

void PWM_Controller_Init(TIM_HandleTypeDef *htim) { s_pwm_htim = htim; }

void PWM_Controller_Toggle(void) {
  // Toggle the PWM output
  // This is a placeholder; actual toggle code will depend on your specific
  // requirements
}

void PWM_Controller_Next_Song(void) {
  play_song_internal(song_gravity_falls, GRAVITY_FALLS_LENGTH);
}
