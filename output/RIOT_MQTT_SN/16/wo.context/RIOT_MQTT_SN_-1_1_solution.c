#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <thread.h>
#include <net/emcute.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval
#define TOPIC_NAME "device/state"

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published %u bytes to topic '%s'\n", (unsigned)len, topic->name);
}

static void *publisher_thread(void *arg) {
    (void)arg;
    emcute_con_t con;

    con.def_port = MQTT_SN_BROKER_PORT;
    inet_pton(AF_INET, MQTT_SN_BROKER_IP, &con.def_addr);

    if (emcute_con(&con, true, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", MQTT_SN_BROKER_IP, MQTT_SN_BROKER_PORT);
        return NULL;
    }

    topic.name = TOPIC_NAME;
    topic.id = 0;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic '%s'\n", TOPIC_NAME);
        emcute_discon();
        return NULL;
    }

    while (1) {
        printf("Publishing state...\n");
        const char *state = "work";  // For demonstration, publishing "work"
        if (emcute_pub(&topic, state, strlen(state) + 1, EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(PUB_INTERVAL);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publisher_thread, NULL, "publisher");

    // Keep main thread running
    while (1) {
        thread_yield();
    }
    return 0;
}
