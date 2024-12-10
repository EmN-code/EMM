
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <mutex.h>
#include <dht_params.h>
#include <ztimer.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000

static Network network;
static MQTTClient client;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    int result = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (result != 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    result = MQTTConnect(&client, &options);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
