#include <ztimer.h>
#include <unistd.h>
#include <msg.h>
#include <netinet/in.h>  
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define STACKSIZE           (THREAD_STACKSIZE_DEFAULT)
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         (1883U)
#define PUB_INTERVAL        (5U)

static char stack[STACKSIZE];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static emcute_topic_t topic;
static char mqtt_client_id[20];
static char *state = "work";

static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    while (1) {
        int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            printf("[INFO] Connected to broker at %s:%d\n", BROKER_IP, BROKER_PORT);
            break;
        } else {
            printf("[ERROR] Connection to broker failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }

    return 0;
}

static void *pub_thread(void *arg) {
    (void)arg;
    char buf[64];

    while (1) {
        snprintf(buf, sizeof(buf), "%s", state);

        topic.name = "sensor/state";
        topic.id = 0;

        int res = emcute_reg(&topic);
        if (res != EMCUTE_OK) {
            printf("[ERROR] Registration failed: %d\n", res);
            continue;
        }

        res = emcute_pub(&topic, buf, strlen(buf), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[ERROR] Publish failed: %d\n", res);
        } else {
            printf("[INFO] Published %s\n", state);
        }

        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));

    strncpy(mqtt_client_id, "ESP32_Client", sizeof(mqtt_client_id) - 1);
    mqtt_client_id[sizeof(mqtt_client_id) - 1] = '\0';

    if (connect_to_broker() != 0) {
        printf("[ERROR] Failed to connect to broker\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, pub_thread, NULL, "pub_thread");

    return 0;
}