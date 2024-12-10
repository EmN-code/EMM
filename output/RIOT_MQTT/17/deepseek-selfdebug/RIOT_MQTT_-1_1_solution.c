
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static mqtt_connect_info_t conn_info;

void mqtt_publish(void) {
    mqtt_publish_t pub;
    pub.topic = TOPIC;
    pub.payload = PAYLOAD;
    pub.payload_len = strlen(PAYLOAD);
    pub.qos = MQTT_QOS_0;
    pub.retain = 0;
    pub.dup = 0;

    mqtt_publish(&client, &pub);
}

int main(void) {
    // Initialize the network stack
    if (net_init() < 0) {
        puts("Error initializing network stack");
        return 1;
    }

    // Create a UDP socket for MQTT
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    // Initialize MQTT client
    mqtt_client_init(&client, &sock);

    // Set up connection info
    conn_info.client_id = "riot_esp32";
    conn_info.keep_alive = 60;
    conn_info.clean_session = 1;
    conn_info.will_topic = NULL;
    conn_info.will_message = NULL;
    conn_info.username = NULL;
    conn_info.password = NULL;

    // Connect to the MQTT broker
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (mqtt_connect(&client, &remote, &conn_info) < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        mqtt_publish();
        xtimer_usleep(INTERVAL);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&client);

    return 0;
}
