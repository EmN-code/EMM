
#include "thread.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static void read_dht11(void)
{
    dht_data_t data;
    if (dht_read(DHT_TYPE_DHT11, DHT11_PIN, &data) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               data.temperature / 10, data.temperature % 10,
               data.humidity / 10, data.humidity % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

int main(void)
{
    if (gpio_init(DHT11_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize GPIO for DHT11");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
