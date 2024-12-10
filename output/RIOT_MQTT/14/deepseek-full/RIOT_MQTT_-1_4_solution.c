
#include <evtimer_msg.h>
#include <paho_mqtt.h>
#include <net/sock.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <arpa/inet.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <time_units.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <MQTTClient.h>
#include <esp_system.h>
#include <net/emcute.h>
#include <msg.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"

static MQTTClient client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = CLIENT_ID;

    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTConnect(&client, &options);

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
