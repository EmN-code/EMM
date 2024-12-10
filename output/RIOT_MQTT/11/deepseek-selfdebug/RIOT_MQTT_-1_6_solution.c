
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_CLIENT_ID "esp32_client"

static void mqtt_connect(void)
{
    struct sockaddr_in broker_addr;
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        puts("Error creating socket");
        return;
    }

    if (connect(sock, (struct sockaddr *)&broker_addr, sizeof(broker_addr)) < 0) {
        puts("Error connecting to broker");
        close(sock);
        return;
    }

    mqtt_client_t client;
    mqtt_client_config_t config = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_client_connect(&client, sock, &config) < 0) {
        puts("Error connecting to MQTT broker");
        close(sock);
        return;
    }

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);
        xtimer_sleep(5);
    }

    mqtt_client_disconnect(&client);
    close(sock);
}

int main(void)
{
    puts("Starting MQTT client");
    mqtt_connect();
    return 0;
}
