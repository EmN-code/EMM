
#include <stdio.h>
#include <netinet/in.h>
#include <net/ipv6/addr.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mma8x5x_regs.h>
#include <sys/socket.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_WORK "work"
#define MSG_NOT_WORK "not work"

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .port = BROKER_PORT };

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

int main(void)
{
    // Initialize the ESP32 system
    esp_system_init();

    // Set up the gateway endpoint
    if (inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid broker address");
        return 1;
    }

    // Connect to the MQTT-SN broker
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to broker (code: %d)\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (code: %d)\n", res);
        return 1;
    }

    // Publish messages every 5 seconds
    while (1) {
        const char *msg = (rand() % 2 == 0) ? MSG_WORK : MSG_NOT_WORK;
        res = emcute_pub(&topic, msg, strlen(msg), 0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish message (code: %d)\n", res);
        } else {
            printf("Published: %s\n", msg);
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
