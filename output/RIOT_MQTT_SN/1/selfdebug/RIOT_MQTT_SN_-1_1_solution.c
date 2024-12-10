#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/conn.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC_NAME "device/state"
#define PUBLICATION_INTERVAL (ZTIMER_MSEC_TO_USEC(5000))

static emcute_topic_t topics[1];
static char stack[THREAD_STACKSIZE_DEFAULT];
static char mqtt_state_msg[50];
static emcute_con_t mqtt_connection;

static void *mqtt_thread(void *arg) {
    (void)arg;
    while (1) {
        strcpy(mqtt_state_msg, "work");  // or "not work" depending on your condition
        emcute_publish(&mqtt_connection, &topics[0], mqtt_state_msg, strlen(mqtt_state_msg), EMCUTE_QOS_0);
        printf("Published state: %s\n", mqtt_state_msg);
        ztimer_sleep(ZTIMER_USEC, PUBLICATION_INTERVAL);
    }
    return NULL;
}

int main(void) {
    mqtt_connection.broker.port = BROKER_PORT;
    conn_ip_ntohl(BROKER_IP, &mqtt_connection.broker.addr.ipv4);

    memset(topics, 0, sizeof(topics));
    topics[0].name = MQTT_TOPIC_NAME;

    emcute_con(&mqtt_connection, true);
    emcute_sub(&mqtt_connection, topics, 1, NULL);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_pub");

    return 0;
}