#include <string.h>
#include <stdio.h>
#include <net/emcute.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC_NAME "device/state"
#define PUB_INTERVAL (5U * US_PER_SEC)

static kernel_pid_t emcute_pid = 0;
static char stack[THREAD_STACKSIZE_MAIN];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(SERVER_PORT, CLIENT_ID);
    return NULL;
}

int main(void) {
    msg_t q[8];

    emcute_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                               THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    if (emcute_pid <= KERNEL_PID_UNDEF) {
        puts("Error: unable to create emcute thread.");
        return 1;
    }

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = SERVER_PORT
    };
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    if (emcute_con(&gw, true, CLIENT_ID, NULL, 0, 0, NULL) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway.");
        return 1;
    }
    puts("Connected to gateway.");

    emcute_topic_t t;
    t.name = TOPIC_NAME;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return 1;
    }
    puts("Topic registered.");

    while (1) {
        int res = emcute_pub(&t, "work", strlen("work"), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            puts("Error: unable to publish data.");
        } else {
            puts("Data published!");
        }

        xtimer_sleep(5);
    }

    return 0;
}