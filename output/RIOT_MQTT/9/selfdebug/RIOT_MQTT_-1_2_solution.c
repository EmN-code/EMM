#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"
#include "net/nanocoap.h"
#include "net/nanocoap_sock.h"
#include "net/mqtt.h"
#include "net/sock/udp.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define STATE_TOPIC "device/state"
#define WORK_STATE "work"
#define NOT_WORK_STATE "not work"

// MQTT client configuration
#define MQTT_TIMEOUT (1000U)

// Buffer sizes for sending and receiving
#define BUFF_SIZE (128U)

static char send_buf[BUFF_SIZE];
static char recv_buf[BUFF_SIZE];

// This flag will help us simulate state changing
static bool is_working = true;

// The timer interval is set to 5 seconds (in microseconds)
#define TIMER_INTERVAL (5U * US_PER_SEC)

static void *mqtt_pub_thread(void *arg) {
    (void)arg;

    mqtt_client_t client;
    mqtt_client_config_t client_config = {
        .broker = MQTT_BROKER_IP,
        .port = MQTT_BROKER_PORT,
        .timeout = MQTT_TIMEOUT,
        .sendbuf = send_buf,
        .sendbuf_size = BUFF_SIZE,
        .recvbuf = recv_buf,
        .recvbuf_size = BUFF_SIZE,
    };

    mqtt_conn_params_t params = {
        .id = "mqtt_example",
        .clean_session = 1,
        .will_policy = MQTT_WILL_POLICY_DEFAULT,
        .will_topic = NULL,
        .will_message = NULL
    };

    while (1) {
        // Toggle working state
        is_working = !is_working;

        const char *state = is_working ? WORK_STATE : NOT_WORK_STATE;

        if (mqtt_connect(&client, &client_config, &params) != MQTT_OK) {
            printf("MQTT: Connection failed\n");
        } else {
            mqtt_message_t message;
            message.qos = MQTT_QOS_0;
            message.payload = state;
            message.payloadlen = strlen(state);
            if (mqtt_publish(&client, STATE_TOPIC, &message) != MQTT_OK) {
                printf("MQTT: Publish failed\n");
            }
        }

        mqtt_disconnect(&client);

        // Sleep for the interval
        xtimer_sleep(TIMER_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_pub_thread, NULL, "mqtt_pub_thread");

    // Allow shell access (e.g., this is non-blocking and mainly for debug purposes)
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}