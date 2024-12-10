
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <mbox.h>
#include <stddef.h>
#include <xtimer.h>

#define MQTT_SERVER_IP "47.102.103.1"
#define MQTT_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MESSAGE "work or not work"

static sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_SERVER_PORT };
static emcute_topic_t topic;

void publish_state(void) {
    int res;
    res = emcute_pub(&topic, MESSAGE, sizeof(MESSAGE) - 1, 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published successfully\n");
    }
}

int main(void) {
    int res;

    // Initialize the ESP32 system
    esp_system_init();

    // Convert IP address from string to binary form
    res = inet_pton(AF_INET, MQTT_SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    // Connect to the MQTT-SN gateway
    res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN gateway: %d\n", res);
        return 1;
    }

    // Register the topic
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
