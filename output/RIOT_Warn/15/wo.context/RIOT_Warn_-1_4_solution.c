#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define LED_PIN GPIO_PIN(PORT_B, 5)
#define DHT_PIN GPIO_PIN(PORT_A, 6)
#define THRESHOLD_TEMP 30
#define SLEEP_TIME 5

void init_pins(void) {
    gpio_init(LED_PIN, GPIO_OUT);
}

int main(void) {
    int16_t temp, hum;
    init_pins();
    
    while (1) {
        if (dht_read(DHT_PIN, &temp, &hum) != DHT_OK) {
            printf("DHT read error\n");
        } else {
            printf("Temperature: %d
", temp);
            if (temp > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold - LED ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature normal - LED OFF\n");
            }
        }
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}