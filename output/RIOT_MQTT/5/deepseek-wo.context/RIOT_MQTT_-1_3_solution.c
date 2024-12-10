
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <net/mqtt_client.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void connect_to_broker(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Published message: %s\n", PAYLOAD);
    }
}

int main(void) {
    printf("Starting MQTT client example\n");

    /* Initialize network */
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;

    /* Initialize MQTT client */
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Connect to MQTT broker */
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    int sock = sock_tcp_create(&client.ipstack.my_socket, &local, &remote, 0);
    if (sock < 0) {
        printf("Failed to create socket, return code %d\n", sock);
        return 1;
    }

    connect_to_broker();

    /* Publish state every 5 seconds */
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
