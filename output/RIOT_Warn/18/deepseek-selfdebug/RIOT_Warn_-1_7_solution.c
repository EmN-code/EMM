
#include "periph/gpio.h"
#include "periph/dht.h"
#include "xtimer.h"
#include "fmt.h"

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static void read_dht11(void) {
    dht_data_t dht_data;
    int status = dht_read(DHT_TYPE_DHT11, DHT_PIN, &dht_data);

    if (status == DHT_OK) {
        int16_t temperature = dht_data.temperature;
        printf("Temperature: %d.%d C\n", (temperature / 10), (temperature % 10));

        if (temperature / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Error reading DHT11 sensor\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht11();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
