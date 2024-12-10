#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

#define DELAY 5
#define DHT_PIN 2

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    printf("Initializing DHT11 on GPIO %d\n", DHT_PIN);
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }
        ztimer_sleep(ZTIMER_MSEC, DELAY * 1000);
    }

    return 0;
}