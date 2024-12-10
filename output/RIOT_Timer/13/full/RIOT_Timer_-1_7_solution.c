#include <sys/socket.h>  // For socket programming
#include <stddef.h>       // For various type definitions
#include <stdlib.h>       // For memory allocation
#include <stdatomic.h>    // For atomic operations
#include <stdbool.h>      // For boolean type
#include <stdio.h>        // For standard input/output
#include <arpa/inet.h>    // For handling internet addresses
#include <netinet/in.h>   // For INET constants and types
#include <net/emcute.h>   // For emcute related network functions
#include <net/gcoap.h>    // For GCOAP network functions
#include <net/sock/util.h> // For socket utility functions
#include <net/sock/udp.h> // For UDP socket functions
#include <thread.h>       // For thread functions
#include <event/thread.h> // For event thread functions
#include <msg.h>          // For message queue functions
#include <mutex.h>        // For mutexes
#include <xtimer.h>       // For timer functions
#include <timex.h>        // For time definitions
#include <time_units.h>   // For conversion of time units
#include <unistd.h>       // For standard symbolic constants and types
#include <ztimer.h>       // For ztimer functions
#include <mbox.h>         // For mailbox event functions
#include <evtimer_msg.h>  // For evtimer message functions
#include <evtimer_mbox.h> // For evtimer mailbox functions
#include "MQTTClient.h"  // For MQTT client functions
#include "kernel_defines.h"  // For kernel defined constants

#define LED_PIN GPIO_PIN(0, 2)
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883

static evtimer_t evtimer;
static msg_t timer_msg_queue[8]; // Must be power of two, e.g., 8

void setup_timer(void) {
    evtimer_mbox_t evtimer_event;
    mbox_t mbox;
    mbox_init(&mbox, timer_msg_queue, sizeof(timer_msg_queue) / sizeof(msg_t));
    evtimer_init_mbox(&evtimer);
    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox);

    /* In practice, this function would perform some action such as toggling an LED. */
    while (1) {
        evtimer_set_msg(&evtimer_event, 1 * US_PER_SEC, NULL);
        msg_t msg;
        msg_receive(&msg);

        // Mock-up for LED toggle function
        printf("LED Toggled\n");
    }
}

void mqtt_example(void) {
    // Initialize the MQTT client.
    MQTTClient client;
    Network network;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, malloc(100), 100, malloc(100), 100);

    rc = 0;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network successfully connected\n");
            break;
        }
    }
    
    // Connect to the MQTT broker.
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32_client";
    rc = MQTTConnect(&client, &connectData);

    if (rc != 0) {
        printf("MQTT connect failed\n");
    } else {
        printf("MQTT connected\n");
    }
}

int main(void) {
    setup_timer();
    mqtt_example();
    return 0;
}