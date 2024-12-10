#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "fmt.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define READ_INTERVAL (5 * US_PER_SEC)  // 5 seconds

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT_TYPE, DHT_PIN, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                temp / 10, temp % 10, hum / 10, hum % 10);
    }
    else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void) {
    dht_init(DHT_PIN);
    while (1) {
        read_dht11();
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }
    return 0;
}
