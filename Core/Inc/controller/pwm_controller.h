#ifndef INC_LED_CONTROLLER_H_
#define INC_LED_CONTROLLER_H_

#include "main.h"

#if 0 /* USER CODE BEGIN Includes */

#define C0 16.35f
#define C0s 17.32f
#define D0b 17.32f
#define D0 18.35f
#define D0s 19.45f
#define E0b 19.45f
#define E0 20.60f
#define F0 21.83f
#define F0s 23.12f
#define G0b 23.12f
#define G0 24.50f
#define G0s 25.96f
#define A0b 25.96f
#define A0 27.50f
#define A0s 29.14f
#define B0b 29.14f
#define B0 30.87f

#define C1 32.70f
#define C1s 34.65f
#define D1b 34.65f
#define D1 36.71f
#define D1s 38.89f
#define E1b 38.89f
#define E1 41.20f
#define F1 43.65f
#define F1s 46.25f
#define G1b 46.25f
#define G1 49.00f
#define G1s 51.91f
#define A1b 51.91f
#define A1 55.00f
#define A1s 58.27f
#define B1b 58.27f
#define B1 61.74f

#define C2 65.41f
#define C2s 69.30f
#define D2b 69.30f
#define D2 73.42f
#define D2s 77.78f
#define E2b 77.78f
#define E2 82.41f
#define F2 87.31f
#define F2s 92.50f
#define G2b 92.50f
#define G2 98.00f
#define G2s 103.83f
#define A2b 103.83f
#define A2 110.00f
#define A2s 116.54f
#define B2b 116.54f
#define B2 123.47f

#define C3 130.81f
#define C3s 138.59f
#define D3b 138.59f
#define D3 146.83f
#define D3s 155.56f
#define E3b 155.56f
#define E3 164.81f
#define F3 174.61f
#define F3s 185.00f
#define G3b 185.00f
#define G3 196.00f
#define G3s 207.65f
#define A3b 207.65f
#define A3 220.00f
#define A3s 233.08f
#define B3b 233.08f
#define B3 246.94f

#define C4 261.63f
#define C4s 277.18f
#define D4b 277.18f
#define D4 293.66f
#define D4s 311.13f
#define E4b 311.13f
#define E4 329.63f
#define F4 349.23f
#define F4s 369.99f
#define G4b 369.99f
#define G4 392.00f
#define G4s 415.30f
#define A4b 415.30f
#define A4 440.00f
#define A4s 466.16f
#define B4b 466.16f
#define B4 493.88f

#define C5 523.25f
#define C5s 554.37f
#define D5b 554.37f
#define D5 587.33f
#define D5s 622.25f
#define E5b 622.25f
#define E5 659.26f
#define F5 698.46f
#define F5s 739.99f
#define G5b 739.99f
#define G5 783.99f
#define G5s 830.61f
#define A5b 830.61f
#define A5 880.00f
#define A5s 932.33f
#define B5b 932.33f
#define B5 987.77f

#define C6 1046.50f
#define C6s 1108.73f
#define D6b 1108.73f
#define D6 1174.66f
#define D6s 1244.51f
#define E6b 1244.51f
#define E6 1318.51f
#define F6 1396.91f
#define F6s 1479.98f
#define G6b 1479.98f
#define G6 1567.98f
#define G6s 1661.22f
#define A6b 1661.22f
#define A6 1760.00f
#define A6s 1864.66f
#define B6b 1864.66f
#define B6 1975.53f

#define C7 2093.00f
#define C7s 2217.46f
#define D7b 2217.46f
#define D7 2349.32f
#define D7s 2489.02f
#define E7b 2489.02f
#define E7 2637.02f
#define F7 2793.83f
#define F7s 2959.96f
#define G7b 2959.96f
#define G7 3135.96f
#define G7s 3322.44f
#define A7b 3322.44f
#define A7 3520.00f
#define A7s 3729.31f
#define B7b 3729.31f
#define B7 3951.07f

#define C8 4186.01f
#define C8s 4434.92f
#define D8b 4434.92f
#define D8 4698.64f
#define D8s 4978.03f
#define E8b 4978.03f
#define E8 5274.04f
#define F8 5587.65f
#define F8s 5919.91f
#define G8b 5919.91f
#define G8 6271.93f
#define G8s 6644.88f
#define A8b 6644.88f
#define A8 7040.00f
#define A8s 7458.62f
#define B8b 7458.62f
#define B8 7902.13f

#endif /* USER CODE END Includes */


void PWM_Controller_Init(void);
void PWM_Controller_Toggle(void);
void PWM_Controller_Next_Song(void);

#endif
