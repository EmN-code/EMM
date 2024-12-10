
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static mqtt_client_config_t config;
static mqtt_client_buf_t cbuf;

void publish_state(void) {
    mqtt_publish_params_t pub;
    pub.topic = TOPIC;
    pub.payload = PAYLOAD;
    pub.payload_len = strlen(PAYLOAD);
    pub.qos = MQTT_QOS_0;
    pub.retain = false;
    pub.dup = false;

    mqtt_publish(&client, &pub);
}

int main(void) {
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize MQTT client
    mqtt_client_init(&client, &cbuf);

    // Set up MQTT client configuration
    config.host = BROKER_IP;
    config.port = BROKER_PORT;
    config.client_id = "riot_esp32";
    config.clean_session = true;
    config.keep_alive = 60;
    config.lwt.topic = NULL;
    config.lwt.message = NULL;
    config.lwt.qos = MQTT_QOS_0;
    config.lwt.retain = false;

    // Connect to MQTT broker
    if (mqtt_connect(&client, &config) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from MQTT broker
    mqtt_disconnect(&client);

    return 0;
}
