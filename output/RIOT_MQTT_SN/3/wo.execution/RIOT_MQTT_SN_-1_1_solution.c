#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "stdio.h"

#define EMCUTE_PORT         (1883U)
#define EMCUTE_ID           "device"
#define EMCUTE_TOPIC        "state"
#define SERVER_ADDR         "47.102.103.1"

static emcute_sock_t sock;
static emcute_topic_t _emcute_topic;

void pub_state(void) {
    char* state = "work";  // Alternatively it could be "not work"
    emcute_msg_t msg;
    msg.data = state;
    msg.len = strlen(state);
    msg.type = EMCUTE_MSG_TYPE_PUBQOS1;

    if (emcute_pub(&_emcute_topic, &msg) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    }
    else {
        printf("Successfully published: %s\n", state);
    }
}

int main(void) {
    /* Set EMCUTE parameters */
    emcute_conf_t conf = {
        .id = EMCUTE_ID,
        .port = EMCUTE_PORT,
    };
    
    /* Initialize EMCUTE */
    sock.af_inet = GCOAP_NET_APPLICATION;
    if (emcute_init(&sock, &conf) != EMCUTE_OK) {
        printf("Error: unable to initialize emcute\n");
        return 1;
    }

    /* Resolve the server address */
    if (inet_pton(AF_INET, SERVER_ADDR, &sock.remote.addr.ipv4) != 1) {
        printf("Error: unable to resolve server address\n");
        return 1;
    }
    sock.remote.port = EMCUTE_PORT;

    /* Connect to the server */
    if (emcute_conn(&sock) != EMCUTE_OK) {
        printf("Error: unable to connect to server\n");
        return 1;
    }

    /* Prepare the publish topic */
    _emcute_topic.name = EMCUTE_TOPIC;
    _emcute_topic.id = 0;

    printf("Connected and initialized\n");

    while (1) {
        pub_state();
        xtimer_sleep(5);
    }

    return 0;
}