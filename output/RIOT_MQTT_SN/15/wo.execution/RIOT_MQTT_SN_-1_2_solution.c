#include "thread.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "net/af.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL 5
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];
static kernel_pid_t emcute_thread_pid;

typedef struct {
    emcute_topic_t topic;
    char *name;
    char *msg;
} mqtt_publish_t;

static void *mqtt_thread(void *arg) {
    (void)arg;
    struct sockaddr_in broker_addr;
    mqtt_publish_t pub_state = {
        .name = "state",
        .msg = "work"
    };

    /* setup the address */
    memset(&broker_addr, 0, sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.sin_addr);

    while (1) {
        emcute_topic_t topic;
        unsigned flags = EMCUTE_QOS_0;

        topic.name = pub_state.name;
        emcute_sub(&topic, 0 /* no topic ID type */);

        /* Publish message */
        if (emcute_pub(&topic, pub_state.msg, strlen(pub_state.msg), flags) != EMCUTE_OK) {
            puts("error: unable to publish data");
        } else {
            puts("successfully published");
        }

        /* wait PUB_INTERVAL seconds */
        sleep(PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    emcute_thread_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO,
                                      THREAD_CREATE_STACKTEST, mqtt_thread,
                                      NULL, "emcute_mqtt");
    /* Let the thread execute */
    thread_sleep();
    return 0;
}