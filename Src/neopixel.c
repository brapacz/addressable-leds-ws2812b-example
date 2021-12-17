#include "neopixel.h"
#include <stdlib.h>

size_t Neopixel_led_count = 0;
TIM_HandleTypeDef *Neopixel_timer = NULL;
uint32_t Neopixel_timer_channel;
uint16_t *Neopixel_PwmData = NULL;
size_t Neopixel_PwmData_size = 0;
uint16_t Neopixel_HI;
uint16_t Neopixel_LO;
volatile uint8_t Neopixel_DMA_Started = 0;

void Neopixel_init(Neopixel_InitTypeDef* init) {
	Neopixel_HI = (init->timer->Init.Period + 1) * 2 / 3;
	Neopixel_LO = (init->timer->Init.Period + 1) * 1 / 3;

	Neopixel_led_count = init->led_count;
	Neopixel_timer = init->timer;
	Neopixel_timer_channel = init->timer_channel;

	Neopixel_PwmData_size = init->led_count * 24 + Neopixel_ret_periods;
	Neopixel_PwmData = malloc(sizeof(uint16_t) * Neopixel_PwmData_size);

	// I had issues with memset
	for (size_t i = 0; i < Neopixel_PwmData_size; i++)
		Neopixel_PwmData[i] = 0x0;
	Neopixel_clear_all();
}

void Neopixel_deinit() {
	free(Neopixel_PwmData);
}

void Neopixel_clear(size_t offset) {
	Neopixel_set(offset, 0, 0, 0);
}

void Neopixel_clear_all() {
	for (size_t i = 0; i < Neopixel_led_count; i++)
		Neopixel_clear(i);
}

void Neopixel_set_all(uint8_t red, uint8_t green, uint8_t blue) {
	for (size_t i = 0; i < Neopixel_led_count; i++)
		Neopixel_set(i, red, green, blue);
}

void Neopixel_set(size_t offset, uint8_t red, uint8_t green, uint8_t blue) {
	if (offset >= Neopixel_led_count)
		return;

	uint32_t colour = ((blue) << 16) | ((green) << 8) | (red);
	for (size_t i = 0; i < 24; i++) {
		size_t idx = offset * 24 + i;
		if (colour & (1 << (23 - i)))
			Neopixel_PwmData[idx] = Neopixel_HI;
		else
			Neopixel_PwmData[idx] = Neopixel_LO;
	}
}
void Neopixel_send() {
	while(Neopixel_DMA_Started);
	Neopixel_DMA_Started = 1;
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*) Neopixel_PwmData,
			Neopixel_PwmData_size);
}

void Neopixel_on_send_completed() {
	HAL_TIM_PWM_Stop_DMA(Neopixel_timer, Neopixel_timer_channel);
	Neopixel_DMA_Started = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	Neopixel_on_send_completed();
}
