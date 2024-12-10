#include <thread.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/emcute_sock.h>

#define BROKER "47.102.103.1"
#define PORT 1883
#define TOPIC "state/topic"
#define PUB_INTERVAL (5U) // publish every 5 seconds

void *mqtt_pub_thread(void *arg) {
    (void)arg;
    emcute_sock_t sock;
    emcute_left_t fst;
    memset(&sock, 0, sizeof(emcute_sock_t));

    sock.port = PORT;
    sock.host = BROKER;

    if (emcute_sock_connect(&sock) != USART_OK) {
        printf("Error: Unable to connect to broker!\n");
        return NULL;
    }

    while (1) {
        char *message = "work";  // message content
        emcute_publish(&sock, TOPIC, message, strlen(message), EMCUTE_QOS_[1], false);
        printf("Published: %s\n", message);
        sleep(PUB_INTERVAL);
    }

    emcute_sock_disconnect(&sock);
    return NULL;
}

int main(void) {
    thread_t thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  mqtt_pub_thread, NULL, "mqtt_pub");
    return 0;
}
