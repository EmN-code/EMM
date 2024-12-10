
#include <net/ipv4/addr.h>
#include <event/thread.h>
#include <thread.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_KEEPALIVE 60
#define MQTT_CMD_TIMEOUT 30000

static char stack[STACKSIZE];
static Network network;
static MQTTClient client;

void mqtt_task(void *arg)
{
    (void)arg;

    int rc;
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.keepAliveInterval = MQTT_KEEPALIVE;
    connect_data.cleansession = 1;

    while (1) {
        if ((rc = MQTTConnect(&client, &connect_data)) != 0) {
            printf("MQTT connect failed, return code %d\n", rc);
            xtimer_sleep(1);
            continue;
        }

        printf("MQTT connected\n");

        while (1) {
            MQTTMessage message;
            message.qos = MQTT_QOS;
            message.retained = MQTT_RETAIN;
            message.payload = (void *)MQTT_PAYLOAD;
            message.payloadlen = strlen(MQTT_PAYLOAD);

            if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != 0) {
                printf("MQTT publish failed, return code %d\n", rc);
                break;
            }

            printf("MQTT message published\n");
            xtimer_sleep(5);
        }

        MQTTClose(&client);
    }
}

int main(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = MQTT_BROKER_PORT;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        printf("Invalid IP address\n");
        return 1;
    }

    if (sock_udp_create(&network.my_socket, NULL, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    MQTTClientInit(&client, &network, MQTT_CMD_TIMEOUT, NULL, 0, NULL, 0);

    thread_create(stack, sizeof(stack), PRIORITY, 0, mqtt_task, NULL, "mqtt_task");

    return 0;
}
