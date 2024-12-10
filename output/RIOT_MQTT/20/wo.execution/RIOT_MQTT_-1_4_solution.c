#include "arpa/inet.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "sys/socket.h"
#include "msg.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "unistd.h"
#include "time_units.h"
#include "dht_params.h"
#include "xtimer.h"
#include "net/emcute.h"
#include "esp_system.h"
#include "event/thread.h"
#include "stdio.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "netinet/in.h"
#include "evtimer_mbox.h"
#include "net/sock/udp.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_PUB_TOPIC "state/topic"
#define MQTT_PUB_MSG "work"
#define MQTT_NOT_WORK_MSG "not work"

static msg_t _main_msg_queue[8]; // Define a message queue

void mqtt_publish_state(void) {
    while (1) {
        // Simulate a device work state condition
        int work_state = (ztimer_now(ZTIMER_MSEC) / 1000 % 2);
        const char *msg = work_state ? MQTT_PUB_MSG : MQTT_NOT_WORK_MSG;

        // Normally, you’d use mqtt_publish() from the MQTT client library
        // Here we would prepare the data and call specific MQTT publish functions
        printf("Publishing: %s\n", msg);
        sleep(5); // Wait for 5 seconds before republishing
    }
}

int main(void)
{
    msg_init_queue(_main_msg_queue, sizeof(_main_msg_queue) / sizeof(msg_t));

    // Initialize MQTT Client: Normally you'd need to configure and connect it
    MQTTClient client; // Normally initialized with networking context and buffers

    // Connect to the broker
    // Use inet_pton() for converting IP address from text to binary form
    struct sockaddr_in broker_addr;
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &broker_addr.sin_addr) <= 0) {
        printf("Error: Invalid IP address\n");
        return 1;
    }

    // Would normally create a socket, connect, and prepare MQTT client handling here.

    // Start publishing state
    mqtt_publish_state();

    return 0;
}