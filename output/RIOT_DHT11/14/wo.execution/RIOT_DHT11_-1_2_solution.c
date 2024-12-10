#include <thread.h>
#include <event/thread.h>
#include <dht.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)

static char stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        /* read the sensor */
        if (dht_read(&temp, &hum, DHT11, DHT_PIN) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } 
        else {
            printf("Error reading DHT11 sensor\n");
        }
        
        /* sleep 5 seconds */
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, dht_thread, NULL, "dht");
    return 0;
}