#include <thread.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC         "test/topic"
#define MQTT_BROKER_PORT   1883
#define SERVER_ADDR        "47.102.103.1"
#define BUF_SIZE           128
#define STACK_SIZE         (THREAD_STACKSIZE_MAIN)

static char stack[STACK_SIZE];
static msg_t queue[8];

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };

void *publisher_thread(void *arg) {
    (void)arg;
    
    const char *message = "work";
    
    while (1) {
        int res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data: %d\n", res);
        }
        xtimer_sleep(5);  // sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    /* initialize emcute */
    emcute_conf_t econf = {
        .stack = stack,
        .stack_size = STACK_SIZE,
        .priority = EMCUTE_PRIO,
        .queue = queue,
        .queue_size = ARRAY_SIZE(queue),
    };

	// Set address of the MQTT broker
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, SERVER_ADDR);

    if (emcute_init(&econf) != EMCUTE_OK) {
        puts("Error: unable to initialize emcute");
        return 1;
    }

    // Connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }

    // Setup the topic
    strncpy(topic.name, MQTT_TOPIC, strlen(MQTT_TOPIC));
    topic.id = 0;  // The topic ID will be set by the broker

    // Create the publisher thread
    if (thread_create(stack, STACK_SIZE, EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                      publisher_thread, NULL, "publisher") <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create thread");
        return 1;
    }

    return 0;
}