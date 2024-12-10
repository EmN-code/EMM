#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"
#include "gpio.h"
#include "MQTTClient.h"

#define TEMP_THRESHOLD 30
#define DHT_PIN GPIO_PIN(PORT_B, 5)  // Specify your DHT11 connected pin
#define LED_PIN GPIO_PIN(PORT_B, 0)  // Specify your LED connected pin

void initialize_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
}

int read_dht(void) {
    dht_t dev;
    dev.type = DHT11;
    dev.pin = DHT_PIN;

    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        return temp / 10;
    } else {
        printf("Failed to read from DHT sensor!\n");
        return -1;
    }
}

void control_led(int temperature) {
    if (temperature > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    printf("DHT11 Reader Initialized\n");
    initialize_gpio();

    while (1) {
        int temperature = read_dht();
        if (temperature != -1) {
            control_led(temperature);
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}