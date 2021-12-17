#ifndef __WS2812B_H
#define __WS2812B_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"

#ifdef __cplusplus
}
#endif
#endif /*__ __WS2812B_H */

#define WS2812B_ret_periods 50

typedef struct {
	size_t led_count;
	TIM_HandleTypeDef *timer;
	uint32_t timer_channel;
} WS2812B_InitTypeDef;

void WS2812B_init(WS2812B_InitTypeDef* init);
void WS2812B_deinit();
void WS2812B_set(size_t offset, uint8_t red, uint8_t green, uint8_t blue);
void WS2812B_set_all(uint8_t red, uint8_t green, uint8_t blue);
void WS2812B_clear(size_t offset);
void WS2812B_clear_now(size_t offset);
void WS2812B_clear_all();
void WS2812B_clear_all_now();
void WS2812B_send();
void WS2812B_on_send_completed();
