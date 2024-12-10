
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define CLIENT_ID "esp32_client"
#define MESSAGE "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static mqtt_connect_info_t conn_info;

void publish_state(void) {
    mqtt_publish_t pub;
    pub.topic = TOPIC;
    pub.payload = MESSAGE;
    pub.payload_len = strlen(MESSAGE);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;

    mqtt_publish(&client, &pub);
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Initialize the MQTT client
    mqtt_init(&client, &sock);

    // Set up the connection info
    conn_info.client_id = CLIENT_ID;
    conn_info.keep_alive = 60;
    conn_info.clean_session = 1;
    conn_info.will_flag = 0;
    conn_info.username = NULL;
    conn_info.password = NULL;

    // Connect to the MQTT broker
    if (mqtt_connect(&client, BROKER_IP, BROKER_PORT, &conn_info) < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&client);

    return 0;
}
