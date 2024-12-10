
#include "thread.h"
#include "paho_mqtt.h"
#include "dht_params.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht.h"
#include "mma8x5x_params.h"
#include "event/thread.h"

#define LED_PIN            GPIO_PIN(0, 13)
#define DHT_PIN            GPIO_PIN(0, 14)
#define TEMP_THRESHOLD     30
#define SLEEP_TIME         5000000

static volatile int temperature = 0;
static volatile int humidity = 0;

void read_dht(void) {
    // Simulated DHT reading
    temperature = 35; // Simulated temperature value
    humidity = 50;    // Simulated humidity value
}

void check_temperature(void) {
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        read_dht();
        check_temperature();
        xtimer_usleep(SLEEP_TIME);
    }

    return 0;
}
