/*
 * This a "bare-metal" example that is being used for blinking D5 LED on the LCDK C6748 board.
 * Contact: <stephon.savic@gmail.com>
 */

#include <stdint.h>

#define PINMUX_13			(*(volatile uint32_t *)0x01C14154)
#define DIR_67				(*(volatile uint32_t *)0x01E26088)
#define OUT_DATA67			(*(volatile uint32_t *)0x01E2608C)
#define SET_DATA67			(*(volatile uint32_t *)0x01E26090)
#define CLR_DATA67			(*(volatile uint32_t *)0x01E26094)
#define CLR_RIS_TRIG67		(*(volatile uint32_t *)0x01E260A0)
#define CLR_FAL_TRIG67		(*(volatile uint32_t *)0x01E260A8)

void configure_PINMUX_13(void);
void configure_GPIO67(void);
void init(void);
void drive_GPIO67(uint8_t on);
void delay(volatile int16_t delay);

int main()
{
	uint8_t on_off = 0x00U;

	init();
	while(1)
	{
		delay(50);
		drive_GPIO67(on_off);

		on_off ^= 0x01;
	}
}

void init(void)
{
	configure_PINMUX_13();
	configure_GPIO67();
}

void configure_PINMUX_13(void)
{
	PINMUX_13 |= 0x8000;
}

void configure_GPIO67(void)
{
	uint32_t mask = 1 << 12;

	CLR_RIS_TRIG67 |= mask;
	CLR_FAL_TRIG67 |= mask;

	DIR_67 &= ~mask;
	CLR_DATA67 |= mask;
}

void drive_GPIO67(uint8_t on)
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
