#include "xtimer.h"
#include "ztimer/periodic.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_GPIO_PIN GPIO_PIN(0, 2)  // GPIO pin where DHT11 is connected
#define INTERVAL (5U * 1000000U)    // 5 seconds interval

static dht_t dev;
static ztimer_periodic_t timer;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read from sensor\n");
    }
}

int main(void) {
    if (dht_init(&dev, DHT11, DHT_GPIO_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }
    ztimer_periodic_init(ZTIMER_USEC, &timer, read_dht11);
    ztimer_periodic_start(&timer, INTERVAL);

    while (1) {
        // Main loop doing other tasks
        xtimer_sleep(1);
    }

    return 0;
}