uint16_t pwmData[24+40] = {0};

void neopixel_send(uint8_t red, uint8_t green, uint8_t blue) {
	uint32_t colour = (blue << 16) | (green << 8) | (red);
	colour &= 0x00FFFFFF;
	for (int i = 23; i >= 0; i--)
		if (colour & (1 << i))
//		if(i % 2 == 0)
			pwmData[i] = (htim1.Init.Period+1)*2/3;
		else
			pwmData[i] = (htim1.Init.Period+1)*1/3;
//			pwmData[i] = ;
	HAL_TIM_PWM_Start_DMA(
		&htim1,
		TIM_CHANNEL_1,
		(uint32_t*) pwmData,
//		sizeof(pwmData) / sizeof(pwmData[0])
		24+40
	);
}
