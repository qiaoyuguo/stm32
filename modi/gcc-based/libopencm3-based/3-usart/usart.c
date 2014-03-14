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

#define GPIO_LEDS (GPIO6|GPIO7|GPIO8|GPIO9)

static void led_setup(void)
{
    //enable clock for four led gpios(on PF6,PF7,PF8,PF9)
    //rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPFEN);
    rcc_periph_clock_enable(RCC_GPIOF);

    //gpios settings of four leds
    gpio_set_mode(GPIOF, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,GPIO_LEDS);

    //light off all leds
    gpio_set(GPIOF, GPIO_LEDS);
}

static void usart_setup(void)
{
    //enable clock for GPIO Port A(for USART_TX and USART_RX) and USART1
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USART1);

    //set USART1 TX to 'output Alternate function push pull'
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
    //set USART1 RX to 'input floating'
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

    //USART_BRR(USART1) = (uint16_t)(72000000<<4) / (115200 * 16);
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX | USART_MODE_RX);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

    usart_enable(USART1);
}

int main(void)
{
	int i,c=0,j=0;

	led_setup();
    usart_setup();

	while (1) {

        gpio_toggle(GPIOF, GPIO_LEDS);
        usart_send_blocking(USART1, c + 'a');
        c = (c == 25) ? 0 : c+1 ;
        if((j++ % 80) == 0)
        {
            usart_send_blocking(USART1, '\r');
            usart_send_blocking(USART1, '\n');
        }
        for(i = 0; i < 800000; i++)
            __asm__("NOP");
    }
    return 0;
}
