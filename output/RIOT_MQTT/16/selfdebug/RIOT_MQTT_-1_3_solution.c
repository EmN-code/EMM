#include <ztimer.h>  // Ensure this is included for ztimer_sleep
#include <stdio.h>
#include <msg.h>  // Ensure this is included for msg_t type
#include <net/emcute.h>
#include <net/sock/udp.h>  // Ensure inclusion for sock_ types and functions

#define SLEEP_INTERVAL (5U)
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT (1883)

typedef struct mqtt_message {
    const char *topic;
    const char *message;
} mqtt_message_t;

static msg_t msg_queue[8];

void publish_state(sock_udp_ep_t *remote) {
    mqtt_message_t mqtt_msg = {
        .topic = "device/state",
        .message = "work or not work"
    };

    size_t msg_len = strlen(mqtt_msg.message);
    sock_udp_send(NULL, mqtt_msg.message, msg_len, remote);
}

int main(void) {
    puts("MQTT publish state example\n");

    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    sock_udp_ep_t remote = {
        .family = AF_INET,
        .netif = SOCK_ADDR_ANY_NETIF,
        .port = MQTT_BROKER_PORT
    };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr);

    while (1) {
        publish_state(&remote);
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL * 1000U);
    }
    return 0;
}