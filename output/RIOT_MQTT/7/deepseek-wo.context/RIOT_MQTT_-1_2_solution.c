
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static MQTTClient mqtt_client;

void connect_to_broker(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, BROKER_IP);

    int res = sock_tcp_connect(&sock, &remote, &local, 0);
    if (res < 0) {
        printf("Error connecting to broker: %d\n", res);
        return;
    }

    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&mqtt_client, (Network*)&sock, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 60;
    data.cleansession = 1;

    int rc = MQTTConnect(&mqtt_client, &data);
    if (rc != 0) {
        printf("Error connecting to MQTT broker: %d\n", rc);
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTPublish(&mqtt_client, TOPIC, &message);
    if (rc != 0) {
        printf("Error publishing message: %d\n", rc);
    } else {
        printf("Published: %s\n", PAYLOAD);
    }
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
