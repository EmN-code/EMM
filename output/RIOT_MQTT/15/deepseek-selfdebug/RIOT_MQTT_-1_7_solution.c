
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_KEEPALIVE 60
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t mqtt_sock;
static sock_udp_ep_t mqtt_broker = { .family = AF_INET, .port = MQTT_BROKER_PORT };

static void mqtt_connect(void)
{
    inet_addr_t broker_addr;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr);
    mqtt_broker.addr.ipv4 = broker_addr;

    if (sock_udp_create(&mqtt_sock, NULL, &mqtt_broker, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    mqtt_client_t client;
    mqtt_client_init(&client);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != SUCCESS) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void mqtt_publish(void)
{
    mqtt_client_t client;
    mqtt_client_init(&client);

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != SUCCESS) {
        puts("Error publishing message");
        return;
    }

    puts("Message published");
}

int main(void)
{
    puts("Starting MQTT client");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
