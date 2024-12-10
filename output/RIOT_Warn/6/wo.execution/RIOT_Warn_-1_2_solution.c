#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "dht.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 5) // Example pin for DHT11
#define LED_PIN GPIO_PIN(0, 2) // Example pin for LED

#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning

void *temperature_check_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(DHT_DHT11, DHT_PIN, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("DHT11 Temperature Warning System");

    // Initialize the GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Start the temperature check thread
    kernel_pid_t pid = thread_create(
        stack,
        sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        temperature_check_thread,
        NULL,
        "temp_check"
    );
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start the temperature check thread\n");
        return 1;
    }

    return 0;
}