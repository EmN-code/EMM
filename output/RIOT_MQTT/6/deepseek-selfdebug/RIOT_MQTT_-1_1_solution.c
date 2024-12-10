
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static uint8_t sendbuf[128];
static uint8_t readbuf[128];

void mqtt_connect(void) {
    mqtt_connect_info_t conn_info = {
        .client_id = "riot_esp32",
        .keep_alive = 60,
        .clean_session = 1,
        .will_flag = 0,
        .username = NULL,
        .password = NULL,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0
    };

    mqtt_client_init(&client, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    mqtt_connect(&client, BROKER_IP, BROKER_PORT, &conn_info);
}

void mqtt_publish(void) {
    mqtt_publish_info_t pub_info = {
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = strlen(PAYLOAD),
        .qos = 0,
        .retain = 0
    };

    mqtt_publish(&client, &pub_info);
}

int main(void) {
    // Initialize the network stack
    sock_udp_create(&sock, NULL, NULL, 0);

    // Connect to the MQTT broker
    mqtt_connect();

    while (1) {
        // Publish the state
        mqtt_publish();

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
