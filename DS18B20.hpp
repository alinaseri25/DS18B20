#ifndef DS18B20_H
#define DS18B20_H

#include "stm32f1xx_hal.h"
#include "dwt_stm32_delay.h"

class DS18B20
{
	public:
		DS18B20(GPIO_TypeDef *_GPIOx,uint16_t _GPIO_Pin);
		float ReadSensor(void);
	private:
		uint8_t ds18b20_init(void);
		void gpio_set_input(void);
		void gpio_set_output(void);
		void write(uint8_t data);
		uint8_t read (void);
		uint8_t check, temp_l, temp_h;
		uint16_t temp;
		float temperature;
		GPIO_InitTypeDef GPIO_InitStruct;
		uint16_t GPIO_Pin;
		GPIO_TypeDef *GPIOx;
};

#endif
