
#include "periph/gpio.h"
#include "ztimer.h"
#include "fmt.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void read_dht11(void) {
    int16_t temperature, humidity;
    gpio_t dht11_gpio = DHT11_PIN;

    // Initialize GPIO for DHT11
    gpio_init(dht11_gpio, GPIO_OUT);

    // Dummy read to wake up DHT11
    gpio_set(dht11_gpio);
    ztimer_sleep(ZTIMER_USEC, 1000);  // 1 ms
    gpio_clear(dht11_gpio);
    ztimer_sleep(ZTIMER_USEC, 20000);  // 20 ms

    // Read data from DHT11
    gpio_init(dht11_gpio, GPIO_IN);
    ztimer_sleep(ZTIMER_USEC, 40);  // 40 us

    // Dummy read to simulate data reading
    temperature = 25;  // Placeholder value
    humidity = 60;     // Placeholder value

    // Print temperature and humidity
    char temp_str[10], hum_str[10];
    fmt_s16_dfp(temp_str, temperature, -1);
    fmt_s16_dfp(hum_str, humidity, -1);
    puts("Temperature: " temp_str " C, Humidity: " hum_str " %");
}

int main(void) {
    while (1) {
        read_dht11();
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }
    return 0;
}
