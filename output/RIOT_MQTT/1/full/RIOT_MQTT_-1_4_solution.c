#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "mutex.h"
#include "msg.h"
#include "dht_params.h"
#include "dht.h"
#include "xtimer.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

static char stack[THREAD_STACKSIZE_MAIN];
static const char MQTT_BROKER_IP[] = "47.102.103.1";
static const uint16_t MQTT_BROKER_PORT = 1883;
static sock_udp_ep_t remote;
static sock_udp_t sock;

void *mqtt_publish_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *state_msg = "work or not work";
        int result = sock_udp_send(&sock, state_msg, strlen(state_msg), &remote);
        if (result < 0) {
            printf("Failed to send message via UDP: %d\n", result);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    /* Initialize remote endpoint */
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    /* Create UDP sock */
    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        printf("Error creating UDP socket\n");
        return -1;
    }

    /* Create thread for publishing MQTT data */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_publish_thread, NULL, "mqtt_publisher");

    /* Main thread does nothing, just sleep indefinitely */
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}