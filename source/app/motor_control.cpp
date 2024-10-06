/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2024 Mihai Baneu                                             |
 │                                                                            |
 | Permission is hereby  granted,  free of charge,  to any person obtaining a |
 | copy of this software and associated documentation files (the "Software"), |
 | to deal in the Software without restriction,  including without limitation |
 | the rights to  use, copy, modify, merge, publish, distribute,  sublicense, |
 | and/or sell copies  of  the Software, and to permit  persons to  whom  the |
 | Software is furnished to do so, subject to the following conditions:       |
 |                                                                            |
 | The above  copyright notice  and this permission notice  shall be included |
 | in all copies or substantial portions of the Software.                     |
 |                                                                            |
 | THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS |
 | OR   IMPLIED,   INCLUDING   BUT   NOT   LIMITED   TO   THE  WARRANTIES  OF |
 | MERCHANTABILITY,  FITNESS FOR  A  PARTICULAR  PURPOSE AND NONINFRINGEMENT. |
 | IN NO  EVENT SHALL  THE AUTHORS  OR  COPYRIGHT  HOLDERS  BE LIABLE FOR ANY |
 | CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT |
 | OR OTHERWISE, ARISING FROM,  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  |
 | THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                 |
 |____________________________________________________________________________|
 |                                                                            |
 |  Author: Mihai Baneu                           Last modified: 03.Oct.2024  |
 |                                                                            |
 |___________________________________________________________________________*/

#include <stdint.h>
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "motor_control.h"

static uint32_t mototr_en_pin;
static uint32_t mototr_dir_pin;
static uint32_t mototr_step_pin;

static uint64_t step_delay_us;

static int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    (void)id;
    
    gpio_xor_mask(1u << mototr_step_pin);
    return *(uint64_t*)user_data;
} 

void motor_init(uint32_t en_pin, uint32_t dir_pin, uint32_t step_pin)
{
    mototr_en_pin = en_pin;
    mototr_dir_pin = dir_pin;
    mototr_step_pin = step_pin;

    gpio_init(mototr_en_pin);
    gpio_init(mototr_dir_pin);
    gpio_init(mototr_step_pin);

    gpio_set_dir(mototr_en_pin, GPIO_OUT);
    gpio_set_dir(mototr_dir_pin, GPIO_OUT);
    gpio_set_dir(mototr_step_pin, GPIO_OUT);

    gpio_put(mototr_en_pin, 1);
    gpio_put(mototr_dir_pin, 0);
    gpio_put(mototr_step_pin, 0);

    step_delay_us = 1000000 / 200; // one rotation / sec
    add_alarm_in_us(step_delay_us, alarm_callback, &step_delay_us, false);
}

void motor_set_speed(uint32_t rot_per_sec)
{
    if (rot_per_sec < 128 * 4) {
        rot_per_sec = 128 * 4;
    }
    if (rot_per_sec > 4096 * 4) {
        rot_per_sec = 4096 * 4;
    }
    step_delay_us = rot_per_sec;
}

void motor_set_dir(motor_dir_e dir)
{
    gpio_put(mototr_dir_pin, dir == motor_dir_cw ? 1 : 0);
}

void motor_start()
{
    gpio_put(mototr_en_pin, 0);
}

void motor_stop()
{
    gpio_put(mototr_en_pin, 1);
}