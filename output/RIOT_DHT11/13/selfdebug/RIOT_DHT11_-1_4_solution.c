#include <xtimer.h>
#include <periph/gpio.h>
#include <stdio.h>

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO 2 for the DHT11 sensor
#define SLEEP_INTERVAL (5U * 1000000U)  // 5 seconds in microseconds

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(DHT11_PIN, &temp, &hum) != DHT_OK) {
        printf("Error reading DHT11\n");
    } else {
        printf("Temperature: %d.%dC, Humidity: %d.%d%%\n",
                temp / 10, temp % 10, hum / 10, hum % 10);
    }
}

int main(void) {
    gpio_init(DHT11_PIN, GPIO_IN);
    printf("Starting DHT11 read every 5 seconds on GPIO2\n");

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return 0;
}