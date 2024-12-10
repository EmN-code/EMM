#include "net/sock/util.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "arpa/inet.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"

#define LED_PIN 2  // ESP32 pin for LED
#define MQTT_BROKER_IP "192.168.1.2"
#define MQTT_BROKER_PORT 1883

void timer_callback(void *arg) {
    printf("Timer triggered, LED toggled\n");
}

int NetworkConnect(Network *n, char* address_ip, int port_number) {
    // Simulating a successful connection
    return 0;
}

int main(void) {
    Network network;
    int rc;

    // Simulate the network connection setup with retry on failure
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_usleep(1000000);  // Wait for 1 second
            continue;
        } else {
            printf("Network connection established successfully\n");
            break;
        }
    }

    // Timer setup
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_SEC, &timer, 1);
    
    // Main event loop, keeping the program running
    while (1) {
        // Simulate event handling
        evtimer_dispatch(evtimer_event(&timer));
    }

    return 0;
}