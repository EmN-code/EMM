#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "periph/gpio.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "thread.h"
#include "mbox.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "timex.h"

#define DHT_PIN 4
#define LED_PIN 2
#define TEMPERATURE_THRESHOLD 30

void init_gpio(int pin) {
    // Initialize GPIO pin for output
    gpio_set(pin);
}

bool read_temperature(int *temperature) {
    // Simulate reading temperature from DHT11
    *temperature = 25; // Dummy value for temperature
    return true;
}

void control_led(bool on) {
    if (on) {
        printf("LED ON\n");
    } else {
        printf("LED OFF\n");
    }
}

void main_loop(void) {
    init_gpio(DHT_PIN);
    init_gpio(LED_PIN);

    int temperature = 0;

    while (true) {
        if (read_temperature(&temperature)) {
            printf("Temperature: %d\n", temperature);
            if (temperature > TEMPERATURE_THRESHOLD) {
                control_led(true);
            } else {
                control_led(false);
            }
        } else {
            printf("Failed to read temperature\n");
        }
        sleep(5); // Wait for 5 seconds before next read
    }
}

int main(void) {
    main_loop();
    return 0;
}