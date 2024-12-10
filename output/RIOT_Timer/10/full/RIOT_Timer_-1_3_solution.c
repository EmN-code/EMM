#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "MQTTClient.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Example placeholder for LED pin

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED logic here
    printf("LED toggled\n");
}

static int connect_network() {
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0){
            puts("Connect failed, retrying...");
            xtimer_sleep(1);
            continue;
        } else {
            puts("Connected successfully");
            break;
        }
    }
    return rc;
}

int main(void) {
    // Initialize the LED pin
    // gpio_init(LED_PIN, GPIO_OUT);

    // Setup the timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.interval = ZTIMER_MSEC(1000);
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    // Connect to the network
    connect_network();

    // Keep the main thread alive
    while(1) {
        thread_yield();
    }

    return 0;
}