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
 |  Author: Mihai Baneu                           Last modified: 05.Oct.2024  |
 |                                                                            |
 |___________________________________________________________________________*/

#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "picortos.h"
#include "task.h"
#include "queue.h"
#include "ipc_com.h"
#include "input_control.h"
#include "motor_control.h"

QueueHandle_t event_queue;

static void vTaskLED(void *pvParameters)
{
    (void)pvParameters;

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (;;) {
        TickType_t xLastWakeTime;

        xLastWakeTime = xTaskGetTickCount();
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelayUntil(&xLastWakeTime, 250 / portTICK_PERIOD_MS);

        xLastWakeTime = xTaskGetTickCount();
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        vTaskDelayUntil(&xLastWakeTime, 250 / portTICK_PERIOD_MS);
    }
}

static void vTaskInput(void *pvParameters)
{
    (void)pvParameters;

    input_init(26, PICO_DEFAULT_KEY_PIN);

    for (;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

static void vTaskMotor(void *pvParameters)
{
    (void)pvParameters;
    uint8_t state = 0;

    motor_init(13, 14, 15);

    for (;;) {
        event_t event;
        if (xQueueReceive(event_queue, &event, portMAX_DELAY) == pdPASS) {
            switch (event.type) {

                case event_type_adc:
                    motor_set_speed(event.value);
                    break;

                case event_type_key:
                    if (event.value == 0) {
                        switch (state) {
                            case 0:
                                motor_set_dir(motor_dir_cw);
                                motor_start();
                                vTaskDelay(1000 / portTICK_PERIOD_MS);
                                motor_set_dir(motor_dir_ccw);
                                motor_start();
                                state = 1;
                                break;
                            case 1:
                                motor_stop();
                                state = 2;
                                break;
                            case 2:
                                motor_set_dir(motor_dir_ccw);
                                motor_start();
                                vTaskDelay(1000 / portTICK_PERIOD_MS);
                                motor_set_dir(motor_dir_cw);
                                motor_start();
                                state = 3;
                                break;
                            case 3:
                                motor_stop();
                                state = 0;
                                break;

                            default:
                                state = 0;
                                break;
                        }
                    }
                    break;
            }
        }
    }
}

int main(void)
{
    /* init IO for logging */
    stdio_init_all();

    /* create the necessary queues & semaphores */
    event_queue = xQueueCreate(10, sizeof(event_t));

    /* create the tasks specific to this application. */
    xTaskCreateAffinitySet(vTaskLED,   "vTaskLED",   configMINIMAL_STACK_SIZE,   NULL, 1, 0x01, NULL);
    xTaskCreateAffinitySet(vTaskInput, "vTaskInput", configMINIMAL_STACK_SIZE,   NULL, 2, 0x01, NULL);
    xTaskCreateAffinitySet(vTaskMotor, "vTaskMotor", configMINIMAL_STACK_SIZE*2, NULL, 3, 0x02, NULL);

    /* start the scheduler. */
    vTaskStartScheduler();

    /* should never get here ... */
    return 0;
}
