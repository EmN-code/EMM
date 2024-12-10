#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <net/emcute.h>
#include <net/emcuteSock.h>

#define BROKER_IP      "47.102.103.1"
#define BROKER_PORT    1883
#define TOPIC_NAME     "device/state"
#define INTERVAL       5

static emcute_sock_t sock;
static char topic[MAX_TOPIC_LEN];

void *pub_state_thread(void *arg) {
    (void)arg;

    emcute_topic_t t;
    t.name = TOPIC_NAME;

    while (1) {
        int pub_res = emcute_pub(&sock, &t, "work", strlen("work"), EMCUTE_QOS_0);
        if (pub_res != EMCUTE_OK) {
            printf("Publication error: %d\n", pub_res);
        } else {
            printf("Published 'work' to topic: %s\n", t.name);
        }

        sleep(INTERVAL);

        pub_res = emcute_pub(&sock, &t, "not work", strlen("not work"), EMCUTE_QOS_0);
        if (pub_res != EMCUTE_OK) {
            printf("Publication error: %d\n", pub_res);
        } else {
            printf("Published 'not work' to topic: %s\n", t.name);
        }

        sleep(INTERVAL);
    }
}

int main(void) {
    sock.dev = riot_ifdemo_init();
    sock.port = BROKER_PORT;
    strcpy(sock.addr.ipv4, BROKER_IP);

    int con_res = emcute_sock_connect(&sock);
    if (con_res != EMCUTE_OK) {
        printf("EMCUTE socket connection failed: %d\n", con_res);
        return 1;
    }

    if (thread_create(pub_state_thread)) {
        printf("Failed to create publication thread\n");
        return 1;
    }

    pause();  // Keep main thread running for the pub_state_thread to work
    return 0;
}