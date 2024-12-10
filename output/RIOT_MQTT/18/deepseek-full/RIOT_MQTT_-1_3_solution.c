
#include "hashes/sha256.h"
#include "net/sock/tcp.h"
#include "mutex.h"
#include "event/thread.h"
#include "net/sock/dtls.h"
#include "net/sock.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "thread.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"
#include "net/mqttsn.h"
#include "sys/socket.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTPublish(client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    }
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    int rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }

    while (1) {
        publish_state(&client);
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}
