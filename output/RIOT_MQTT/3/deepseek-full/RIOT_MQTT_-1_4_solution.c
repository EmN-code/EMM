
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <mutex.h>
#include <timex.h>
#include <kernel_defines.h>
#include <msg.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work or not work"
#define PAYLOAD_LEN 16
#define COMMAND_TIMEOUT_MS 10000
#define SEND_BUF_SIZE 1024
#define READ_BUF_SIZE 1024

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;
static MQTTClient client;
static Network network;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

static void _publish_state(void)
{
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&client, TOPIC, &message);
}

static void _timer_callback(void *arg)
{
    (void)arg;
    _publish_state();
}

int main(void)
{
    /* Initialize the network */
    NetworkInit(&network);

    /* Initialize the MQTT client */
    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    /* Set up the remote endpoint */
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    /* Create the UDP socket */
    sock_udp_create(&sock, &local, &remote, 0);

    /* Connect to the MQTT broker */
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";
    MQTTConnect(&client, &options);

    /* Set up the periodic timer for publishing state */
    evtimer_msg_init_periodic(&timer, _timer_callback, NULL, 5 * US_PER_SEC);

    /* Main loop */
    while (1) {
        msg_t msg;
        if (evtimer_msg_recv(&msg) > 0) {
            _publish_state();
        }
    }

    return 0;
}
