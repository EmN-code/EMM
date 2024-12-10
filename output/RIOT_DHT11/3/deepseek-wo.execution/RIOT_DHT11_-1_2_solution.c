
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "xtimer.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static void read_dht11(void) {
    gpio_t dht11_gpio = GPIO_PIN(0, 2);
    gpio_init(dht11_gpio, GPIO_OUT);
    gpio_set(dht11_gpio);
    xtimer_usleep(2000000); // 2 seconds delay to stabilize the sensor

    // Dummy read operation to simulate DHT11 reading
    printf("DHT11 read: Temperature: 25°C, Humidity: 50%%\n");
}

int main(void) {
    while (1) {
        read_dht11();
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return 0;
}
