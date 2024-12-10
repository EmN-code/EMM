#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "unistd.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "net/gcoap.h"
#include "MQTTClient.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"
#include "mutex.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "arpa/inet.h"
#include "thread.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"
#define RECONNECT_SLEEP 1
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

int main(void) {
    emcute_topic_t topic;
    msg_t queue[8];
    ztimer_sleep_t sleep_pub;

    // Initialize the message queue
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Configure the remote address
    sock_udp_ep_t gw = {.family = AF_INET, .port = SERVER_PORT};
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse IPV4 address.");
        return -1;
    }

    // Connect to MQTT-SN gateway with error handling
    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("Successfully connected to %s:%d\n", SERVER_IP, SERVER_PORT);
            break;
        } else {
            printf("Failed to connect to %s:%d, retrying...\n", SERVER_IP, SERVER_PORT);
            xtimer_sleep(RECONNECT_SLEEP);
        }
    }

    // Register topic
    strncpy(topic.name, TOPIC_NAME, sizeof(topic.name));
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return -1;
    }

    // Allocate thread ztimer
    ztimer_alloc(ztimer_usec, &sleep_pub);

    // Publish every 5 seconds
    while (1) {
        if (emcute_pub(&topic, MESSAGE, strlen(MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message.");
        } else {
            printf("Published: %s to topic: %s\n", MESSAGE, TOPIC_NAME);
        }
        ztimer_sleep_usec(&sleep_pub, PUBLISH_INTERVAL);
    }

    // Disconnect cleanly
    if (emcute_discon() != EMCUTE_OK) {
        puts("Error: unable to disconnect.");
    }

    return 0;
};