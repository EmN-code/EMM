
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <timex.h>
#include <dht_params.h>
#include <time_units.h>
#include <event/thread.h>
#include <net/gcoap.h>
#include <esp_system.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/sock/util.h>
#include <mbox.h>
#include <sys/socket.h>
#include <ztimer.h>
#include <paho_mqtt.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work or not work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 30000
#define SEND_BUF_SIZE 1024
#define READ_BUF_SIZE 1024

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

static void _network_init(void)
{
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
}

static void _mqtt_client_init(MQTTClient* client)
{
    Network network;
    NetworkInit(&network);
    MQTTClientInit(client, &network, MQTT_COMMAND_TIMEOUT_MS,
                   (unsigned char*)malloc(SEND_BUF_SIZE), SEND_BUF_SIZE,
                   (unsigned char*)malloc(READ_BUF_SIZE), READ_BUF_SIZE);
}

static void _mqtt_connect(MQTTClient* client)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";
    MQTTConnect(client, &options);
}

static void _mqtt_publish(MQTTClient* client)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);
    MQTTPublish(client, MQTT_TOPIC, &message);
}

int main(void)
{
    MQTTClient client;

    _network_init();
    _mqtt_client_init(&client);
    _mqtt_connect(&client);

    while (1) {
        _mqtt_publish(&client);
        xtimer_sleep(5);
    }

    return 0;
}
