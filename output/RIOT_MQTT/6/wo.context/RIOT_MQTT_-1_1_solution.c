#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "net/mqtt.h"
#include "net/emcute.h"

#define MQTT_BROKER_IP   "47.102.103.1"
#define MQTT_PORT        1883
#define KEEP_ALIVE       60

#define CLIENT_ID        "esp32-client"
#define TOPIC            "esp32/status"

static mqtt_client_t client;
static mqtt_connect_client_info_t client_info = {
    .client_id   = CLIENT_ID,
    .user        = NULL,
    .password    = NULL,
    .will_topic  = NULL,
    .will_message = NULL,
    .keep_alive  = KEEP_ALIVE
};

void publish_status(const char* status) {
    mqtt_publish(&client, TOPIC, status, strlen(status), MQTT_QOS_0, 0);
    printf("Published status: %s\n", status);
}

int main(void) {
    /* Initialize the network components */
    mqtt_snipcart_init();
    mqtt_iface_init();

    /* Connect to the MQTT broker */
    sock_udp_ep_t remote = { .family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF, .port = MQTT_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr);

    if (mqtt_connect(&client, &remote, &client_info) != MQTT_OK) {
        puts("ERROR: Unable to connect to the MQTT broker");
        return -1;
    }
    puts("Connected to MQTT broker");

    while (1) {
        /* Alternate the status between work and not work */
        publish_status("work");
        sleep(5);
        publish_status("not work");
        sleep(5);
    }

    return 0;
}