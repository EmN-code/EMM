
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "fmt.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void read_dht11(void)
{
    int16_t temp, hum;
    if (dht_read_temperature_humidity(DHT_PIN, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    if (gpio_init(DHT_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
