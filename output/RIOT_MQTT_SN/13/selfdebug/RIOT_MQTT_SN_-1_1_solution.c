#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "device/state"
#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void *pub_thread(void *arg) {
    (void)arg;
    
    emcute_topic_t t;
    t.name = TOPIC_NAME;
    t.id = 0; // Will be assigned by the broker
    
    char *state_msg[2] = {"work", "not work"};
    int state = 0;

    while (1) {
        emcute_pub(&t, state_msg[state], strlen(state_msg[state]), EMCUTE_QOS_0);
        state = !state; // Toggle state between 0 and 1
        xtimer_sleep(INTERVAL / 1000000U); // Convert microseconds to seconds
    }
    return NULL;
}

int main(void) {
    msg_t msg_q[8];
    emcute_sock_t sock = { .queue = msg_q, .queue_size = ARRAY_SIZE(msg_q) };

    puts("Starting MQTT-SN example");

    emcute_init(&sock);
    emcute_con_t con;
    con.server.pid = 0; // Local interface
    ipv4_addr_t addr;
    inet_pton(AF_INET, BROKER_IP, &addr);

    if (emcute_con(&con, &addr, BROKER_PORT) != EMCUTE_OK) {
        puts("Connection failed");
        return 1;
    }

    emcute_sub(&topic, EMCUTE_QOS_0);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    return 0;
}