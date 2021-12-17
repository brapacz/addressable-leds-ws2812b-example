#ifndef __neopixel_H
#define __neopixel_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"

#ifdef __cplusplus
}
#endif
#endif /*__ __neopixel_H */

#define Neopixel_ret_periods 50

typedef struct {
	size_t led_count;
	TIM_HandleTypeDef *timer;
	uint32_t timer_channel;
} Neopixel_InitTypeDef;

void Neopixel_init(Neopixel_InitTypeDef* init);
void Neopixel_deinit();
void Neopixel_set(size_t offset, uint8_t red, uint8_t green, uint8_t blue);
void Neopixel_set_all(uint8_t red, uint8_t green, uint8_t blue);
void Neopixel_clear(size_t offset);
void Neopixel_clear_all();
void Neopixel_send();
void Neopixel_on_send_completed();
