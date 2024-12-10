
#include <net/sock.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <ztimer.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>  // Ensure this is the last header file included

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000

static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void mqtt_message_arrived(MessageData* data) {
    (void)data;
}

int main(void) {
    // Initialize network
    NetworkInit(&network);

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &network, MQTT_COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set up connection parameters
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    // Connect to MQTT broker
    int rc = MQTTConnect(&mqtt_client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    // Publish state every 5 seconds
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    while (1) {
        rc = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    // Disconnect from MQTT broker
    MQTTDisconnect(&mqtt_client);

    return 0;
}
