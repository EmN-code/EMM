#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius

void *dht_sensor_thread(void *arg) {
    (void)arg;
    dht_t dev;
    dev.pin = GPIO_NUM_4; // Example GPIO
    int16_t temperature, humidity;

    while (1) {
        int res = dht_read(&dev, &temperature, &humidity);
        if (res == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(GPIO_NUM_2);  // Turn on LED or warning
            } else {
                gpio_clear(GPIO_NUM_2); // Turn off LED or warning
            }
        } else {
            printf("Error reading from sensor\n");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));  // Wait for 5 seconds
    }
    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  dht_sensor_thread, NULL, "dht_sensor_thread");
    return 0;
}