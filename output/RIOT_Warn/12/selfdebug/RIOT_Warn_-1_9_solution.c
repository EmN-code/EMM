#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "saul.h"
#include "saul_reg.h"
#include "thread.h"
#include "periph/gpio.h"

#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval
#define TEMP_THRESHOLD 30 // Temperature threshold for LED warning

static gpio_t led_pin = GPIO_PIN(PORT_A, 0); // Define the LED pin, change as necessary

void *sensor_reader(void *arg) {
    (void)arg;
    saul_reg_t *dev = saul_reg_find_type(SAUL_SENSE_TEMP);
    if (!dev) {
        printf("Error: No temperature sensor found\n");
        return NULL;
    }
    
    while (1) {
        phydat_t res;
        int result = saul_reg_read(dev, &res);
        if (result == 0) {
            printf("Temperature: %d C\n", res.val[0]);
            if (res.val[0] > TEMP_THRESHOLD) {
                gpio_set(led_pin);
                printf("Warning: Temperature exceeded threshold!\n");
            } else {
                gpio_clear(led_pin);
            }
        } else {
            printf("Error reading sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("RIOT DHT11 Temperature Warning System\n");
    gpio_init(led_pin, GPIO_OUT);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sensor_reader, NULL, "sensor_reader");

    // The main thread can handle other tasks or go to sleep
    while (1) {
        xtimer_usleep(INTERVAL);
    }

    return 0;
}