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
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "picortos.h"
#include "queue.h"
#include "ipc_com.h"
#include "input_control.h"

static uint32_t input_adc_pin;
static uint32_t input_key_pin;
static uint32_t last_key_state;

static void adc_irq_fifo_handler()
{
    event_t event = {
        .type = event_type_adc,
        .value = adc_fifo_get() + adc_fifo_get() + adc_fifo_get() + adc_fifo_get()
    };
    xQueueSendToBackFromISR(event_queue, &event, 0);
}

static int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    (void)id;
    uint32_t new_key_state = gpio_get(input_key_pin);

    if (new_key_state != last_key_state) {
        event_t event = {
            .type = event_type_key,
            .value = new_key_state
        };
        xQueueSendToBackFromISR(event_queue, &event, 0);
    }

    last_key_state = new_key_state;
    return 10000;
} 

void input_init(uint32_t adc_pin, uint32_t key_pin)
{
    input_adc_pin = adc_pin;
    input_key_pin = key_pin;

    /* init input key */
    gpio_init(input_key_pin);
    gpio_set_dir(input_key_pin, GPIO_IN);
    last_key_state = gpio_get(input_key_pin);
    add_alarm_in_ms(10, alarm_callback, nullptr, false);
    
    /* init ADC */
    adc_init();
    adc_gpio_init(input_adc_pin);
    adc_select_input(0);
    adc_set_clkdiv(47999);
    adc_irq_set_enabled(true);
    adc_fifo_setup(true, false, 4, false, false);
    adc_run(true);
    
    /* set-up IRQ for the ADC FIFO */
    irq_set_exclusive_handler(ADC_IRQ_FIFO, adc_irq_fifo_handler);
    irq_set_priority(ADC_IRQ_FIFO, 3);
    irq_set_enabled(ADC_IRQ_FIFO, true);
}
