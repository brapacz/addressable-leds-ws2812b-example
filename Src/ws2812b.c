#include "ws2812b.h"
#include <stdlib.h>

size_t WS2812B_led_count = 0;
TIM_HandleTypeDef *WS2812B_timer = NULL;
uint32_t WS2812B_timer_channel;
uint16_t *WS2812B_PwmData = NULL;
size_t WS2812B_PwmData_size = 0;
uint16_t WS2812B_HI;
uint16_t WS2812B_LO;
volatile uint8_t WS2812B_DMA_Started = 0;

void WS2812B_init(WS2812B_InitTypeDef* init) {
	WS2812B_HI = (init->timer->Init.Period + 1) * 2 / 3;
	WS2812B_LO = (init->timer->Init.Period + 1) * 1 / 3;

	WS2812B_led_count = init->led_count;
	WS2812B_timer = init->timer;
	WS2812B_timer_channel = init->timer_channel;

	WS2812B_PwmData_size = init->led_count * 24 + WS2812B_ret_periods;
	WS2812B_PwmData = malloc(sizeof(uint16_t) * WS2812B_PwmData_size);

	// I had issues with memset
	for (size_t i = 0; i < WS2812B_PwmData_size; i++)
		WS2812B_PwmData[i] = 0x0;
	WS2812B_clear_all();
}

void WS2812B_deinit() {
	free(WS2812B_PwmData);
}

void WS2812B_clear(size_t offset) {
	WS2812B_set(offset, 0, 0, 0);
}

void WS2812B_clear_all() {
	for (size_t i = 0; i < WS2812B_led_count; i++)
		WS2812B_clear(i);
}

void WS2812B_set_all(uint8_t red, uint8_t green, uint8_t blue) {
	for (size_t i = 0; i < WS2812B_led_count; i++)
		WS2812B_set(i, red, green, blue);
}

void WS2812B_set(size_t offset, uint8_t red, uint8_t green, uint8_t blue) {
	if (offset >= WS2812B_led_count)
		return;

	uint32_t colour = ((blue) << 16) | ((green) << 8) | (red);
	for (size_t i = 0; i < 24; i++) {
		size_t idx = offset * 24 + i;
		if (colour & (1 << (23 - i)))
			WS2812B_PwmData[idx] = WS2812B_HI;
		else
			WS2812B_PwmData[idx] = WS2812B_LO;
	}
}

void WS2812B_send() {
	while (1 == WS2812B_DMA_Started)
		;
	WS2812B_DMA_Started = 1;
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*) WS2812B_PwmData,
			WS2812B_PwmData_size);
}

void WS2812B_on_send_completed() {
	if (1 == WS2812B_DMA_Started) {
		HAL_TIM_PWM_Stop_DMA(WS2812B_timer, WS2812B_timer_channel);
		WS2812B_DMA_Started = 0;
	}
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim == WS2812B_timer)
		WS2812B_on_send_completed();
}
