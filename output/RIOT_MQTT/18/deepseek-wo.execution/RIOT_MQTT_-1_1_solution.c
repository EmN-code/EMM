
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t broker = { .family = AF_INET, .port = MQTT_BROKER_PORT };
static mqtt_client_t client;
static uint8_t sendbuf[1024];
static uint8_t readbuf[1024];

void publish_state(void)
{
    mqtt_publish_params_t pub_params = {
        .topic = MQTT_TOPIC,
        .message = MQTT_MESSAGE,
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
        .message_length = strlen(MQTT_MESSAGE)
    };

    mqtt_publish(&client, &pub_params);
}

int main(void)
{
    // Initialize the network interface
    if (netif_default == NULL) {
        puts("No default network interface found.");
        return 1;
    }

    // Initialize the MQTT client
    mqtt_client_init(&client);

    // Set up the broker endpoint
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker.addr.ipv4);

    // Connect to the MQTT broker
    mqtt_connect_params_t conn_params = {
        .keep_alive = 60,
        .client_id = MQTT_CLIENT_ID,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&client, &broker, &conn_params, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf)) < 0) {
        puts("Failed to connect to MQTT broker.");
        return 1;
    }

    puts("Connected to MQTT broker.");

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    // Disconnect from the MQTT broker
    mqtt_disconnect(&client);

    return 0;
}
