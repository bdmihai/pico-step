#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "picortos.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

QueueHandle_t key_event_queue, time_event_queue;
SemaphoreHandle_t mutex;

static void vTaskLED(void *pvParameters)
{
    (void)pvParameters;

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (;;) {
        
            TickType_t xLastWakeTime;

            xLastWakeTime = xTaskGetTickCount();
            if( xSemaphoreTake( mutex, portMAX_DELAY ) == pdTRUE ) {
                gpio_put(PICO_DEFAULT_LED_PIN, 1);
                xSemaphoreGive(mutex);
            }
            vTaskDelayUntil(&xLastWakeTime, 250 / portTICK_PERIOD_MS);

            xLastWakeTime = xTaskGetTickCount();
            if( xSemaphoreTake( mutex, portMAX_DELAY ) == pdTRUE ) {
                gpio_put(PICO_DEFAULT_LED_PIN, 0);
                xSemaphoreGive(mutex);
            }
            vTaskDelayUntil(&xLastWakeTime, 250 / portTICK_PERIOD_MS);
    }
}

static void vTaskLog(void *pvParameters)
{
    (void)pvParameters;

    for (;;) {
        uint32_t event;
        if (xQueueReceive(key_event_queue, &event, portMAX_DELAY) == pdPASS) {
            printf("Core %d: Key event received: %ld\n", get_core_num(), event);
        }
    }
}

static void vTaskKey(void *pvParameters)
{
    (void)pvParameters;

    gpio_init(PICO_DEFAULT_KEY_PIN);
    gpio_set_dir(PICO_DEFAULT_KEY_PIN, GPIO_IN);

    uint32_t last_key_state = gpio_get(PICO_DEFAULT_KEY_PIN);
    for (;;) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
        uint32_t new_key_state = gpio_get(PICO_DEFAULT_KEY_PIN);

        if (new_key_state != last_key_state) {
            xQueueSendToBack(key_event_queue, &new_key_state, (TickType_t) 1);

            if (new_key_state) {
                xSemaphoreTake(mutex, portMAX_DELAY);
            } else {
                xSemaphoreGive(mutex);
            }
        }

        last_key_state = new_key_state;
    }
}

bool repeating_timer_callback(struct repeating_timer *t) {
    (void) t;
    uint64_t current_time = time_us_64();
    xQueueSendToBackFromISR(time_event_queue, &current_time, 0);
    return true;
}

static void vTaskTime(void *pvParameters)
{
    (void)pvParameters;

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

    for (;;) {
        uint64_t event;
        if (xQueueReceive(time_event_queue, &event, portMAX_DELAY) == pdPASS) {
            printf("Core %d: Time at %lld\n", get_core_num(), event);
        }
    }
}

int main(void)
{
    stdio_init_all();

    /* create the necessary queues & semaphores */
    key_event_queue  = xQueueCreate(1, sizeof(uint32_t));
    time_event_queue = xQueueCreate(1, sizeof(uint64_t));
    mutex = xSemaphoreCreateMutex();

    /* create the tasks specific to this application. */
    xTaskCreateAffinitySet(vTaskLED,  "vTaskLED",  configMINIMAL_STACK_SIZE,   NULL, 1, 0x02, NULL);
    xTaskCreateAffinitySet(vTaskKey,  "vTaskKey",  configMINIMAL_STACK_SIZE,   NULL, 2, 0x01, NULL);
    xTaskCreateAffinitySet(vTaskLog,  "vTaskLog",  configMINIMAL_STACK_SIZE*2, NULL, 2, 0x02, NULL);
    xTaskCreateAffinitySet(vTaskTime, "vTaskTime", configMINIMAL_STACK_SIZE*2, NULL, 3, 0x03, NULL);

    /* start the scheduler. */
    vTaskStartScheduler();

    /* should never get here ... */
    return 0;
}
