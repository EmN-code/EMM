#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 4) // Use appropriate GPIO pin for your setup
#define LED_PIN GPIO_PIN(0, 5) // Use appropriate GPIO pin for LED
#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning

void *sensor_thread(void *arg){
    (void)arg;
    int16_t temperature, humidity;
    dht_t dev;
    esp_err_t res;

    dht_init(&dev, &dht_params[0]);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            puts("Failed to read from DHT sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature/10, temperature%10, humidity/10, humidity%10);

            // Check temperature threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void){
     gpio_init(LED_PIN, GPIO_OUT);
    
    kernel_pid_t pid;
    char stack[THREAD_STACKSIZE_MAIN];
    pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                        0, sensor_thread, NULL, "sensor_thread");

    // Thread should be created successfully, otherwise handle error
    if (pid <= KERNEL_PID_UNDEF) {
        puts("[Error] Failed to create thread");
        return 1;
    }

    // Keep the main function running to prevent exiting
    while(1) {
        thread_yield();
    }

    return 0;
}