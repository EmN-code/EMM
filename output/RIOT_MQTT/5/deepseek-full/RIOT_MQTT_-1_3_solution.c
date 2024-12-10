
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/ipv4/addr.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static MQTTClient client;

int connect_to_broker(void) {
    Network network;
    int rc;

    NetworkInit(&network);
    rc = NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT);
    if (rc != 0) {
        return -1;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        return -1;
    }

    return 0;
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    int rc;

    MQTTClientInit(&client, NULL, TIMEOUT, NULL, 0, NULL, 0);

    rc = connect_to_broker();
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
