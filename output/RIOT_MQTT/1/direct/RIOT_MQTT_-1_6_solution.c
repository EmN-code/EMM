#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqtt.h>
#include <net/emcute.h>

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         1883
#define TOPIC_NAME          "device/status"
#define BROKER_IP           "47.102.103.1"
#define PUBLISH_INTERVAL    (5U * ZTIMER_SEC)

static char *messages[] = {"work", "not work"};

void *mqtt_publisher(void *arg) {
    (void) arg;
    emcute_sock_t sock;
    sock = emcute_open();

    size_t i = 0;

    /* initialize the MQTT broker target address */
    sock.c_rb.nl_ipv4.addr = ntohl(inet_addr(BROKER_IP));
    sock.c_rb.nl_ipv4.port = EMCUTE_PORT;

    /* start connect loop */
    while (1) {
        if (emcute_connect(&sock, EMCUTE_ID) != EMCUTE_OK) {
            printf("Could not connect to broker\n");
            continue;
        }

        /* prepare MQTT message */
        emcute_msg_t pub_msg;
        pub_msg.data = messages[i];
        pub_msg.len = strlen(messages[i]);
        pub_msg.retained = false;

        /* publish to topic */
        if (emcute_pub(&sock, TOPIC_NAME, &pub_msg, EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Could not publish message\n");
        } else {
            printf("Published: %s\n", messages[i]);
        }

        emcute_disconnect(&sock);

        /* swap between work and not work for next publication */
        i = (i + 1) % 2;

        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    pthread_t pub_thread;
    pthread_create(&pub_thread, NULL, mqtt_publisher, NULL);

    pthread_join(pub_thread, NULL);

    return 0;
}