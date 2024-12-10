#include <ztimer.h>
#include <sys/socket.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <paho_mqtt.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <timex.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "state"
#define MESSAGE "work or not work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize the structures
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Failed to connect to the network. Retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("Connected to the network successfully.\n");
            break;
        }
    }

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32_Client";

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("Failed to connect to MQTT broker. Retrying...\n");
            NetworkDisconnect(&network);
            ztimer_sleep(ZTIMER_SEC, 1);
            NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
            continue;
        }
        printf("Connected to MQTT broker successfully.\n");
        break;
    }

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)MESSAGE;
    message.payloadlen = strlen(MESSAGE);

    while (1) {
        rc = MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message. Retrying...\n");
            MQTTConnect(&client, &connectData); // Reconnect if necessary
        }
        printf("Published message: %s\n", MESSAGE);
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    return 0;
}