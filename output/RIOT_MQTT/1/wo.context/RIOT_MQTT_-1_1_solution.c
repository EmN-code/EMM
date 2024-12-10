#include <timer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/emcute.h>
#include <msg.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_mqtt_client"
#define MQTT_TOPIC "esp32/state"
#define MQTT_WORK_STATE "work"
#define MQTT_NOT_WORK_STATE "not_work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)
#define MQTT_QOS EMCUTE_QOS_0

static emcute_con_t mqtt_client;
char msg_queue[32];

static void *mqtt_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_t m;
    m.type = 1;  // Dummy message type

    while (1) {
        msg = msg_try_receive(&m);
        puts("Publishing MQTT message...");
        emcute_pub(MQTT_TOPIC, (strcmp(arg, "1")==0) ? MQTT_WORK_STATE : MQTT_NOT_WORK_STATE, strlen(arg), MQTT_QOS);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue));

    mqtt_client.conn = (emcute_conn_t){
        .port = MQTT_BROKER_PORT,
    };

    strcpy(mqtt_client.conn.addr, MQTT_BROKER_IP);

    if (emcute_con(&mqtt_client.conn, true, MQTT_CLIENT_ID, NULL, NULL, 0) != EMCUTE_OK) {
        puts("Could not connect to MQTT broker.");
        return 1;
    }

    kernel_pid_t mqtt_pid = thread_create(msg_queue, sizeof(msg_queue), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, "1", "mqtt_thread");

    if (mqtt_pid < 0) {
        puts("Error: Unable to create MQTT thread.");
        return 1;
    }

    while (1) {
        // Main process which might contain additional functionality
        xtimer_sleep(60);
    }

    return 0;
}