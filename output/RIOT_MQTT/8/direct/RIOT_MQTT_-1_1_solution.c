#include <string.h>
#include <stdio.h>
#include <net/emcute.h>
#include <msg.h>
#include <shell.h>

#define _IPV4(...) 
#define MQTT_BROKER_IP        "47.102.103.1"
#define MQTT_BROKER_PORT      1883
#define MQTT_TOPIC_STATE      "device/state"
#define PUBLISH_INTERVAL_SEC  5
#define EMCUTE_PORT           1885
#define EMCUTE_PRIO           (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_ID             "device"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    /* we might want to wait for the main thread to call conn first */
    xtimer_sleep(1);
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int mqtt_publish(char *topic, char *msg) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = topic;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("[mqtt_publish] error: unable to register topic");
        return 1;
    }

    if (emcute_pub(&t, msg, strlen(msg), flags) != EMCUTE_OK) {
        puts("[mqtt_publish] error: unable to publish data");
        return 1;
    }

    printf("[mqtt_publish] success: published `%s` on topic `%s`\n", msg, topic);
    return 0;
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_BROKER_PORT
    };

    char *addr = MQTT_BROKER_IP;

    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr, addr) == NULL) {
        puts("[mqtt_connect] error: unable to parse IPv4 address.");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[mqtt_connect] error: unable to connect to MQTT broker");
        return 1;
    }

    puts("[mqtt_connect] success: connected to MQTT broker");
    return 0;
}

int main(void) {
    kernel_pid_t emcute_pid;

    /* start emcute thread */
    emcute_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                               emcute_thread, NULL, "emcute");
    if (emcute_pid <= KERNEL_PID_UNDEF) {
        puts("[main] error: unable to start emcute thread");
        return 1;
    }

    /* Connect to the MQTT broker */
    if (mqtt_connect() != 0) {
        puts("[main] error: unable to connect to broker");
        return 1;
    }

    while (1) {
        /* Publish state every PUBLISH_INTERVAL_SEC seconds */
        mqtt_publish(MQTT_TOPIC_STATE, "work");
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
        mqtt_publish(MQTT_TOPIC_STATE, "not work");
        xtimer_sleep(PUBLISH_INTERVAL_SEC);
    }

    return 0;
}