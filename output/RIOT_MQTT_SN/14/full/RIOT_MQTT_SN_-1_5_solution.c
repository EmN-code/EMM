#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stddef.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <mbox.h>
#include <msg.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <architecture.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <MQTTClient.h>
#include <kernel_defines.h>
#include <arpa/inet.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)
#define TOPIC_NAME "device/state"

static emcute_topic_t _mqtt_topic;

void establish_connection(sock_udp_ep_t *gw)
{
    int res;
    while (1) {
        res = emcute_con(gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("[MQTT] Connected successfully");
            break;
        } else {
            printf("[MQTT] Connection failed with code %d. Retrying...\n", res);
            sleep(1);
        }
    }
}

void publish_state(void)
{
    char message[20];
    sprintf(message, "State: %s", "work");
    int res = emcute_pub(&_mqtt_topic, message, strlen(message), 0);
    if (res != EMCUTE_OK) {
        printf("[MQTT] Publish failed with code %d\n", res);
    } else {
        puts("[MQTT] Publish success");
    }
}

int main(void)
{
    char stack[THREAD_STACKSIZE_MAIN];
    sock_udp_ep_t gw = {.family = AF_INET, .netif = SOCK_ADDR_ANY_NETIF};

    /* Set gateway IP and port */
    inet_pton(AF_INET, BROKER_IP, &(gw.addr.ipv4));
    gw.port = BROKER_PORT;

    /* Connect to MQTT-SN broker */
    establish_connection(&gw);

    /* Prepare topic */
    _mqtt_topic.name = TOPIC_NAME;

    while (1) {
        publish_state();
        /* Wait before next publication */
        usleep(PUB_INTERVAL);
    }

    /* start main thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, thread_main, NULL, "main");

    /* Should never be reached */
    return 0;
}