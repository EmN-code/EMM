#include "thread_arch.h"
#include "thread.h"
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_TYPE DHT11        // Specify the type of DHT sensor
#define DHT_PIN GPIO_PIN(0, 2) // GPIO pin where the DHT11 is connected
#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds expressed in microseconds

static dht_t dht_dev;

void *dht_read_loop(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        xtimer_usleep(SLEEP_INTERVAL); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.type = DHT_TYPE;
    params.pin = DHT_PIN;
    
    if (dht_init(&dht_dev, &params) != 0) {
        printf("Failed to initialize the DHT11 sensor\n");
        return 1;
    }

    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, dht_read_loop, NULL, "dht_reader");
    
    return 0;
}