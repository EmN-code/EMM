#include <ztimer.h> // Handles timer functionality, allowing for delays and timed actions
#include <netinet/in.h> // Provides definitions for internet protocol family
#include <net/sock/util.h> // Util functions for socket operations, making network programming easier
#include <net/sock/udp.h> // UDP socket functions to enable network communication using UDP protocol
#include <event/thread.h> // Manages events in a multi-threaded context
#include <evtimer_msg.h> // Timer messaging for event-based timer handling
#include <evtimer_mbox.h> // Mailbox for event-timer messaging
#include <time_units.h> // Definitions for time units, aiding time-based operations
#include <paho_mqtt.h> // Simplifies MQTT operations, masks complexities of low-level MQTT protocol
#include "MQTTClient.h" // Provides core functions for establishing MQTT connection and actions

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define STATE_MESSAGE "work"
#define CLIENT_ID "iot_client_esp32"
#define KEEP_ALIVE_INTERVAL 60
#define COMMAND_TIMEOUT 4000
#define PUBLISH_INTERVAL (5LU * US_PER_SEC)

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

event_t timer_event;
evtimer_t my_timer;
evtimer_msg_event_init(my_timer, &timer_event);

void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)STATE_MESSAGE;
    message.payloadlen = strlen(STATE_MESSAGE);
    MQTTPublish(&client, "esp32/state", &message);
}

void *client_thread(void *arg)
{
    (void)arg; // avoid unused warning
    NetworkInit(&network);
    int rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = CLIENT_ID;
    connectData.keepAliveInterval = KEEP_ALIVE_INTERVAL;

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect to MQTT broker, rc=%d\n", rc);
        return NULL;
    }

    evtimer_msg_timing(&my_timer, 0, PUBLISH_INTERVAL, 1);

    while (1) {
        msg_t m;
        msg_receive(&m);
        if (m.type == EVTIMER_MSG_TYPE) {
            publish_state();
        }
    }

    return NULL;
}

int main(void)
{
    thread_create(client_thread, NULL);
    return 0;
}