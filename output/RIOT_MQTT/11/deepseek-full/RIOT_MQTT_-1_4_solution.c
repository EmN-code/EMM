
#include "paho_mqtt.h"
#include "net/sock.h"
#include "net/af.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "net/sock/udp.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include "MQTTClient.h"
#include "esp_system.h"
#include "net/emcute.h"
#include "net/sock/util.h"
#include "sys/socket.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEPALIVE 20

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];
static int rc;

void mqtt_connect(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    IPStackConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = KEEPALIVE;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }
    printf("Connected to MQTT Broker\n");
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
        printf("Failed to publish, return code %d\n", rc);
    } else {
        printf("Published message: %s\n", PAYLOAD);
    }
}

void mqtt_disconnect(void) {
    if ((rc = MQTTDisconnect(&client)) != 0) {
        printf("Failed to disconnect, return code %d\n", rc);
    } else {
        printf("Disconnected from MQTT Broker\n");
    }
    NetworkDisconnect(&network);
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish();
        vTaskDelay(pdMS_TO_TICKS(5000)); // Delay for 5 seconds
    }

    mqtt_disconnect();
    return 0;
}
