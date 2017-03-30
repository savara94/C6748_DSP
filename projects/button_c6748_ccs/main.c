/*
 * This a "bare-metal" example that uses polling method for detection of the pressed button(S2) on LCDK C6748 board.
 * D5 LED is being toggled on the button press.
 * Contact: <stephon.savic@gmail.com>
 */

#include <stdint.h>

#define PINMUX_6			(*(volatile uint32_t *)0x01C14138)
#define PINMUX_13			(*(volatile uint32_t *)0x01C14154)
#define DIR_23				(*(volatile uint32_t *)0x01E26038)
#define DIR_67				(*(volatile uint32_t *)0x01E26088)
#define IN_DATA23			(*(volatile uint32_t *)0x01E26048)
#define OUT_DATA67			(*(volatile uint32_t *)0x01E2608C)
#define SET_DATA67			(*(volatile uint32_t *)0x01E26090)
#define CLR_DATA67			(*(volatile uint32_t *)0x01E26094)
#define CLR_RIS_TRIG23		(*(volatile uint32_t *)0x01E26050)
#define CLR_RIS_TRIG67		(*(volatile uint32_t *)0x01E260A0)
#define CLR_FAL_TRIG23		(*(volatile uint32_t *)0x01E26058)
#define CLR_FAL_TRIG67		(*(volatile uint32_t *)0x01E260A8)

void init(void);
void configure_PINMUX(void);
void configure_PINMUX_6(void);
void configure_PINMUX_13(void);
void configure_GPIO(void);
void configure_GPIO_23(void);
void configure_GPIO_67(void);
void drive_GPIO_67(uint8_t on);
int8_t is_gpio_24_high(void);
int8_t is_pressed(void);
void delay(volatile int16_t delay);

int main()
{
	uint8_t on_off = 0x00U;

	init();
	while(1)
	{
		delay(50);
		drive_GPIO_67(on_off);

		if (is_pressed())
		{
			on_off ^= 0x01;
		}
	}
}

void init(void)
{
	configure_PINMUX();
	configure_GPIO();
}

void configure_PINMUX()
{
	configure_PINMUX_6();
	configure_PINMUX_13();
}

void configure_PINMUX_6(void)
{
	PINMUX_6 |= 0x8000;
}
void configure_PINMUX_13(void)
{
	PINMUX_13 |= 0x8000;
}

void configure_GPIO(void)
{
	configure_GPIO_23();
	configure_GPIO_67();
}

void configure_GPIO_23(void)
{
	uint32_t mask = 1 << 4;

	CLR_RIS_TRIG23 |= mask;
	CLR_FAL_TRIG23 |= mask;

	DIR_23 |= ~mask;
}

void configure_GPIO_67(void)
{
	uint32_t mask = 1 << 12;

	CLR_RIS_TRIG67 |= mask;
	CLR_FAL_TRIG67 |= mask;

	DIR_67 &= ~mask;
	CLR_DATA67 |= mask;
}

void drive_GPIO_67(uint8_t on)
{
	uint32_t mask = 1 << 12;

	switch(on)
	{
		case 0 :
			CLR_DATA67 |= mask;
			break;
		case 1 :
			SET_DATA67 |= mask;
			break;
		default:
			/* Should not happen */
			break;
	}
}

int8_t is_gpio_24_high(void)
{
	uint32_t mask = 1 << 4;

	return IN_DATA23 & mask;
}

int8_t is_pressed(void)
{
	if (!is_gpio_24_high())
	{
		delay(100);
		if (!is_gpio_24_high())
		{
			return 1;
		}
	}
	
	return 0;
}

void delay(volatile int16_t delay)
{
	int16_t cookie;

	while(delay > 0)
	{
		cookie = INT16_MAX;
		while(cookie > 0)
		{
			cookie--;
		}
		delay--;
	}
}
