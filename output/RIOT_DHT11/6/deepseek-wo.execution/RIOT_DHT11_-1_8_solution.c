
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];

void read_dht11(void *arg)
{
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    while (1) {
        if (dht_init(&dev, &dht_params_t{ .pin = DHT_PIN }) == 0) {
            if (dht_read(&dev, &temp, &hum) == 0) {
                printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                       temp / 10, temp % 10, hum / 10, hum % 10);
            } else {
                puts("Failed to read from DHT11 sensor");
            }
        } else {
            puts("Failed to initialize DHT11 sensor");
        }
        xtimer_usleep(5 * 1000 * 1000); // Sleep for 5 seconds
    }
}

int main(void)
{
    thread_create(stack, sizeof(stack), PRIORITY, 0, read_dht11, NULL, "dht11_reader");
    return 0;
}
