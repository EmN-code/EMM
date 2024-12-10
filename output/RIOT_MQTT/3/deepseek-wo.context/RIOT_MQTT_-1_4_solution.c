
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void mqtt_publish(MQTTClient *client, const char *topic, const char *payload, int payload_len) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)payload;
    message.payloadlen = payload_len;

    MQTTPublish(client, topic, &message);
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_ep_t broker = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(broker.addr.ipv4, BROKER_IP);

    if (sock_tcp_connect(&sock, &broker, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to broker");
        return 1;
    }

    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_publish(&client, TOPIC, PAYLOAD, PAYLOAD_LEN);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
