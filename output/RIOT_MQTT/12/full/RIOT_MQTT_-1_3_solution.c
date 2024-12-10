#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/mqttsn.h"
#include "net/gcoap.h"
#include "net/af.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000U * 1000U)  // 5 seconds in microseconds

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    int rc;

    NetworkInit(&network);

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.keepAliveInterval = 10;
    connectData.cleansession = 1;

    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void*)"work or not work";
    message.payloadlen = strlen((char*)message.payload);

    while (1) {
        rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            xtimer_usleep(1 * 1000 * 1000); // 1 second
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    while (1) {
        rc = MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
        if (rc != 0) {
            printf("MQTT client initialization failed. Retrying...\n");
            xtimer_usleep(1 * 1000 * 1000);
            continue;
        }
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            xtimer_usleep(1 * 1000 * 1000);
            NetworkDisconnect(&network);
            continue;
        } else {
            printf("MQTT connected successfully.\n");
        }

        while (1) {
            rc = MQTTPublish(&client, "state_topic", &message);
            if (rc != 0) {
                printf("MQTT publish failed. Attempting reconnect...\n");
                MQTTDisconnect(&client);
                NetworkDisconnect(&network);
                break;
            } else {
                printf("MQTT message published successfully.\n");
            }
            xtimer_usleep(PUBLISH_INTERVAL);
        }
    }

    return 0;
}