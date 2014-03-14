/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <stdio.h>
#include <errno.h>

#define RCC_GPIO_LED RCC_GPIOF
#define GPIO_PORT_LED GPIOF
#define GPIO_PIN_LED (GPIO6 | GPIO7 | GPIO8 | GPIO9)

int _write(int file, char *ptr, int len);

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOA clock (for LED GPIOs). */
	rcc_periph_clock_enable(RCC_GPIO_LED);

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART1);
}

static void usart_setup(void)
{
	/* Enable the USART1 interrupt. */
//	nvic_enable_irq(NVIC_USART1_IRQ);

	/* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port B for receive. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX);

	/* Enable USART1 Receive interrupt. */
	//USART_CR1(USART1) |= USART_CR1_RXNEIE;

	/* Finally enable the USART. */
	usart_enable(USART1);
}

static void gpio_setup(void)
{
	gpio_set(GPIO_PORT_LED, GPIO_PIN_LED);

	/* Setup LED GPIO (in GPIO port F) */
	gpio_set_mode(GPIO_PORT_LED, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO_PIN_LED);
}

int _write(int file, char *ptr, int len)
{
    int i;
    if(file == 1)
    {
        for(i = 0; i < len; i++)
            usart_send_blocking(USART1, ptr[i]);
        return i;
    }
    errno= EIO;
    return -1;
}

int main(void)
{
    int counter = 0;
    float fcounter = 0.0;
    double dcounter = 0.0;

	clock_setup();
	gpio_setup();
	usart_setup();

	/* Wait forever and do nothing. */
	while (1)
    {
        gpio_toggle(GPIO_PORT_LED, GPIO_PIN_LED);
        printf("Hello World! %i %f %f\r\n", counter, fcounter, dcounter);
        counter++;
        fcounter += 0.01;
        dcounter += 0.01;
    }

	return 0;
}
