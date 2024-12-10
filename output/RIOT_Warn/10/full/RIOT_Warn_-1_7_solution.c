#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht.h"
#include "dht_params.h"
#include "architecture.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

void read_temperature_and_control_led(dht_t *dev, gpio_t led_pin, int16_t threshold) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp > threshold) {
                gpio_set(led_pin);
                printf("LED ON\n");
            } else {
                gpio_clear(led_pin);
                printf("LED OFF\n");
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    dht_params_t params;
    params.pin = GPIO_PIN(0, 5); // Modify pin number as per your configuration
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED GPIO
    gpio_t led_pin = GPIO_PIN(0, 6); // Modify pin number as per your configuration
    if (gpio_init(led_pin, GPIO_OUT) != 0) {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }

    // Define temperature threshold in tenths of degrees Celsius
    int16_t temperature_threshold = 300; // 30.0°C

    // Read temperature and control LED based on threshold
    read_temperature_and_control_led(&dev, led_pin, temperature_threshold);

    return 0;
}
