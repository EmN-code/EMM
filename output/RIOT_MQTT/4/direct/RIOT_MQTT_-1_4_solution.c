#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // Used for sleep function.
#include <xtimer.h>
#include <net/mqtt.h>
#include <net/sock/udp.h>
#include <net/af.h>

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define CLIENT_ID "riot_client"
#define TOPIC "riot/status"
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not_work"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mqtt_client_t mqtt_client;
static sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = MQTT_BROKER_PORT
};

static int publish_message(const char *message) {
    mqtt_publish(&mqtt_client, NULL, TOPIC, (uint8_t *)message, strlen(message), 0);
    printf("Published: %s\n", message);
    return 0;
}

void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        publish_message(MESSAGE_WORK);
        xtimer_usleep(PUBLISH_INTERVAL);
        publish_message(MESSAGE_NOT_WORK);
        xtimer_usleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    printf("RIOT MQTT Publisher Example\n");
    remote.netif = SOCK_ADDR_ANY_NETIF;

    /* Initialize MQTT client */
    mqtt_client_init(&mqtt_client, &remote, NULL);
    sock_udp_ep_t local = SOCK_ADDR_ANY;
    local.port = 0;
    mqtt_connect(&mqtt_client, MQTT_BROKER_ADDR, MQTT_BROKER_PORT, local, 5);

    /* Start publisher thread */
    char stack[THREAD_STACKSIZE_LARGE];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    return 0;
}