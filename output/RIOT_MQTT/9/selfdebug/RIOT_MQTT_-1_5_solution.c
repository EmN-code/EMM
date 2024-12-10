
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>

#define SERVER_ADDR     "47.102.103.1"
#define SERVER_PORT     1883
#define CLIENT_ID       "esp32_client"
#define PUB_TOPIC       "devices/esp32/status"
#define MSG_INTERVAL    (5U * US_PER_SEC)  // 5 seconds

static sock_udp_ep_t gw_endpoint;

static void publish_state(mqttsn_con_t *con) {
    mqttsn_pub_t pub_data;
    pub_data.topic = PUB_TOPIC;
    const char *message = "work";  // or "not work"
    pub_data.data = (uint8_t *)message;
    pub_data.data_len = strlen(message);

    if (mqttsn_pub(con, &pub_data) != MQTT_SN_RC_ACCEPTED) {
        printf("Error: Could not publish state\n");
    } else {
        printf("Successfully published state: %s\n", message);
    }
}

int main(void) {
    mqttsn_con_t connection;
    mqttsn_init(&connection, &gw_endpoint, CLIENT_ID);

    memset(&gw_endpoint, 0, sizeof(gw_endpoint));
    gw_endpoint.family = AF_INET;
    gw_endpoint.netif = SOCK_ADDR_ANY_NETIF;
    gw_endpoint.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_ADDR, &gw_endpoint.addr);

    if (mqttsn_con(&connection) == MQTT_SN_RC_ACCEPTED) {
        printf("Connected to MQTT broker\n");
        while (1) {
            publish_state(&connection);
            xtimer_sleep(MSG_INTERVAL / US_PER_SEC);
        }
    } else {
        printf("Error: Connection to broker failed\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}