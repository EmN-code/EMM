#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <socket.h>
#include <arpa/inet.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       "esp32_client"
#define SERVER_IP       "47.102.103.1"
#define TOPIC_NAME      "device/state"
#define MESSAGE_INTERVAL 5 

static char *message_state = "work";
static emcute_topic_t topic;
static sock_udp_ep_t server;

void publish_state(void){
    if (emcute_pub(&topic, message_state, strlen(message_state), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("[Error] unable to publish data");
    }
    else{
        printf("Published: %s\n", message_state);
    }
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    server.family = AF_INET;
    server.port = EMCUTE_PORT;

    if (inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4) == 0) {
        puts("[Error] unable to parse IP");
        return 1;
    }

    if (emcute_con(&server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[Error] unable to connect");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("[Error] unable to obtain topic ID");
        return 1;
    }

    while (1) {
        publish_state();
        sleep(MESSAGE_INTERVAL);
    }

    return 0;
}