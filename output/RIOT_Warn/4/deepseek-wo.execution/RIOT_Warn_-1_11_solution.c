
#include "periph/gpio.h"
#include "ztimer.h"
#include "fmt.h"

#define DHT_PIN GPIO_PIN(0, 18)
#define LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30

void read_dht_and_check_temp(void) {
    int16_t temp, hum;
    // Simulate reading from DHT11
    // In a real implementation, you would replace this with actual DHT11 reading code
    temp = 35; // Simulated temperature value
    hum = 50;  // Simulated humidity value

    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        read_dht_and_check_temp();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
