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

static uint16_t exti_line_state;
static void gpio_setup(void)
{
    //enable clock for four led gpios(on PF6,PF7,PF8,PF9)
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPFEN);

    //gpios settings of four leds
    gpio_set_mode(GPIOF, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO6|GPIO7|GPIO8|GPIO9);

    gpio_set(GPIOF, GPIO6|GPIO7|GPIO8|GPIO9);
}

static void button_setup(void)
{
    //enable clock for buttons(PB8)
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN);

    //set GPIO1(in Port B) to 'input open-drain'
    //gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO1);
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO1);
    gpio_port_write(GPIOB, 1<<1);
}

int main(void)
{
	int i;

	gpio_setup();
    button_setup();

	while (1) {

        //press 'ENTER' key on the five-direction button will make four leds blink
        exti_line_state = GPIOB_IDR;
        if((exti_line_state & (1<<1)) == 0){
            gpio_clear(GPIOF, GPIO6|GPIO7|GPIO8|GPIO9);	/* LED on */
            for (i = 0; i < 800000; i++)	/* Wait a bit. */
                __asm__("nop");
            gpio_set(GPIOF, GPIO6|GPIO7|GPIO8|GPIO9);	/* LED off */

        }
    }

}
