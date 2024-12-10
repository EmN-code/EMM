#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define INTERVAL (1LU * US_PER_SEC) // 1-second interval
#define LED_PIN GPIO_PIN(0, 2) // Example LED pin

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED to simulate turning it on
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the ztimer
    ztimer_t timer;
    ztimer_set_msg(&timer, ZTIMER_MSEC, INTERVAL, thread_getpid());
    ztimer_start(&timer);

    // MQTT Network connection
    Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connect failed, retrying...\n");
            xtimer_sleep(1); // delay before retry
            continue;
        } else {
            printf("Network connect successful\n");
            break;
        }
    }
    
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}