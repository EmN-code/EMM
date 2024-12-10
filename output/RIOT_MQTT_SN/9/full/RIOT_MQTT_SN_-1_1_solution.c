#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/af.h>
#include <timex.h>
#include <thread.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_PUBLISH_TOPIC "esp32/status"
#define INTERVAL (5U)

static emcute_sock_t sock;
static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publication on topic: %s, Payload: %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void) {
    int ret;
    sock_ifaddr_t addrs = SOCK_ADDR_ANY;

    /* Initialize the network interface */
    kernel_pid_t if_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, emcute_thread, &sock, "emcute", NULL);

    /* Setup the EMCUTE MQTT-SN client structure */
    memset(&sock, 0, sizeof(sock));
    sock.callback = on_pub;
    sock.data = &topic;
    sock.netif = if_pid;

    /* Attempt to establish a network connection */
    while (1) {
        ret = emcute_netif_set(sock.netif, &addrs, sizeof(addrs));
        if (ret != 0) {
            printf("Error establishing network connection. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Network connection established.\n");
        break;
    }

    /* Attempt to connect to the MQTT-SN broker */
    while (1) {
        ret = emcute_con(&sock, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (ret != EMCUTE_OK) {
            printf("Error connecting to MQTT-SN broker. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Successfully connected to the broker.\n");
        break;
    }

    /* Initialize topic */
    topic.id = 0;
    topic.name = MQTT_PUBLISH_TOPIC;

    /* Publish state every 5 seconds */
    while (1) {
        char *msg = "work";
        ret = emcute_pub(&sock, &topic.id, topic.name, msg, strlen(msg) + 1, EMCUTE_QOS_1);
        if (ret != EMCUTE_OK) {
            printf("Failed to publish message. Retrying...\n");
        } else {
            printf("Published message: %s\n", msg);
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}