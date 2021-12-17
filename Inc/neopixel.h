#define PIXELS_COUNT 2

uint16_t pwmData[2 + PIXELS_COUNT][24] = { 0 };

void neopixel_send(uint8_t red, uint8_t green, uint8_t blue, uint8_t offset) {
	// offset should be >= 0 and < PIXELS_COUNT
	uint32_t colour = ((blue) << 16) | ((green) << 8) | (red);
	uint16_t HI = (htim1.Init.Period + 1) * 2 / 3;
	uint16_t LO = (htim1.Init.Period + 1) * 1 / 3;
	for (int i = 0; i < 24; i++)
		if (colour & (1 << (23 - i)))
//		if(i % 2 == 0)
			pwmData[offset][i] = HI;
		else
			pwmData[offset][i] = LO;
//			pwmData[i] = ;
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*) pwmData,
//		sizeof(pwmData) / sizeof(pwmData[0])
			24 * PIXELS_COUNT + 40);
}
