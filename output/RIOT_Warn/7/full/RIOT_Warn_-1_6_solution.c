#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming LED is connected to pin 2
#define TEMP_THRESHOLD 30      // Temperature warning threshold in Celsius
#define SLEEP_INTERVAL 5       // Sleep interval in seconds between reads

void read_dht11_and_control_led(void) {
    int temp, hum;
    while (1) {
        if (dht_read(&DHT_PARAMS, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED if temperature exceeds threshold
            } else {
                gpio_clear(LED_PIN);  // Turn off LED otherwise
            }
        } else {
            puts("Failed to read DHT11 sensor");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_init(&DHT_PARAMS);

    // Fetch temperature/humidity data every 5 seconds and control LED
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_dht11_and_control_led, NULL, "DHT11 Reader");

    while (1) {
        pause();  // Keep the main thread alive
    }

    return 0;
}
