#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/conn/ip.h"

#define EMCUTE_ID           "mqttsn"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    (1883)
#define PUBLISH_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t subscriptions[1];
static char topic_name[] = "device/state";
static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    emcute_config_t conf;
    conf.id = EMCUTE_ID;
    conf.stack = stack;
    conf.stack_size = sizeof(stack);
    conf.prio = EMCUTE_PRIO;

    if (emcute_setup(&conf) != EMCUTE_OK) {
        printf("Error setting up emcute\n");
        return NULL;
    }
    
    sock_udp_ep_t gw = {.family = AF_INET, .port = MQTT_BROKER_PORT };
    ipv4_addr_t addr;
    ipv4_addr_from_str(&addr, MQTT_BROKER_IP);
    gw.addr.ipv4 = addr;

    while (1) {
        if (emcute_con(&gw, false) != EMCUTE_OK) {
            printf("Connection failed, retrying in 1 second\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("Successfully connected to broker\n");
            break;
        }
    }
    
    topic.name = topic_name;
    topic.id = 1000; // arbitrary ID

    while (1) {
        const char *data = "work";
        if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publish failed, retrying\n");
        }
        else {
            printf("Published: %s\n", data);
        }
        
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}