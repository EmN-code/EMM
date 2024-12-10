#include "xtimer.h"
#include "msg.h"
#include "dht.h"
#include "board.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO2
#define SLEEP_TIME (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    if (dht_init(&dev, DHT_PIN) != 0) {
        puts("DHT initialization failed");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, 
                   hum / 10, hum % 10);
        }

        xtimer_sleep(SLEEP_TIME / 1000000U); // Convert microseconds to seconds for xtimer_sleep
    }
    return 0;
}