#include "DS18B20.hpp"


DS18B20::DS18B20(GPIO_TypeDef *_GPIOx,uint16_t _GPIO_Pin)
{
	GPIOx = _GPIOx;
	GPIO_Pin = _GPIO_Pin;
}

void DS18B20::gpio_set_input(void)
{
	GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DS18B20::gpio_set_output(void)
{
	GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void DS18B20::write(uint8_t data)
{
	gpio_set_output ();   // set as output

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			gpio_set_output ();  // set as output
			HAL_GPIO_WritePin (GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // pull the pin LOW
			DWT_Delay_us (1);  // wait for  us

			gpio_set_input ();  // set as input
			DWT_Delay_us (60);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			gpio_set_output ();
			HAL_GPIO_WritePin (GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // pull the pin LOW
			DWT_Delay_us (60);  // wait for 60 us

			gpio_set_input ();
		}
	}
}

uint8_t DS18B20::read (void)
{
	uint8_t value=0;
	gpio_set_input ();

	for (int i=0;i<8;i++)
	{
		gpio_set_output ();   // set as output

		HAL_GPIO_WritePin (GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // pull the data pin LOW
		DWT_Delay_us (2);  // wait for 2 us

		gpio_set_input ();  // set as input
		if (HAL_GPIO_ReadPin (GPIOx, GPIO_Pin))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		DWT_Delay_us (60);  // wait for 60 us
	}
	return value;
}

float DS18B20::ReadSensor(void)
{
	ds18b20_init ();
	write (0xCC);  // skip ROM
	write (0x44);  // convert t

	//HAL_Delay (800);
	osDelay(800);

	ds18b20_init ();
	write (0xCC);  // skip ROM
	write (0xBE);  // Read Scratchpad

	temp_l = read();
	temp_h = read();
	temp = (temp_h<<8)|temp_l;
	temperature = (float)temp/16;
	return temperature;
}

uint8_t DS18B20::ds18b20_init(void)
{
	gpio_set_output ();   // set the pin as output
	HAL_GPIO_WritePin (GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // pull the pin low
	DWT_Delay_us (480);   // delay according to datasheet

	gpio_set_input ();    // set the pin as input
	DWT_Delay_us (80);    // delay according to datasheet

	if (!(HAL_GPIO_ReadPin (GPIOx, GPIO_Pin)))    // if the pin is low i.e the presence pulse is there
	{
		DWT_Delay_us (400);  // wait for 400 us
		return 0;
	}

	else
	{
		DWT_Delay_us (400);
		return 1;
	}
}
