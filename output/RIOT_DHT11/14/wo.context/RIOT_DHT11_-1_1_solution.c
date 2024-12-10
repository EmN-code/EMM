#include <timer.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <dht.h>
#include <xtimer.h>

#define DHT_PIN GPIO_PIN(0, 2)  // use GPIO 2 for DHT sensor
#define READ_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static dht_t dht11;

void *dht11_reading_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;

    while(1) {
        if (dht_read(&dht11, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                temperature / 10, temperature % 10, 
                humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dht11, &params) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return 1;
    }
    
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht11_reading_thread, NULL, 
                  "dht11_thread");

    return 0;
}
