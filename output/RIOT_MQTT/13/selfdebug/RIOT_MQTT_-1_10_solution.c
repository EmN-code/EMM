#include "xtimer.h"
#include "net/sock/udp.h"
#include "mqtt.h"
#include "msg.h"

#define BROKER_IP "47.102.103.1"
#define MQTT_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define RUN_INTERVAL (5 * 1000000U)  // in microseconds

typedef struct {
    mqtt_client_t client;
    mqtt_endpoint_t gw;
    mqtt_client_con_t conn;
} mqtt_client_data_t;

static mqtt_client_data_t mqtt_data;
static char stack[THREAD_STACKSIZE_DEFAULT];
static mqtt_client_conn_params_t conn_params;

static void on_pub_request(void *arg, const char *topic, const void *val, size_t val_len) {
    (void)arg;
    (void)topic;
    (void)val;
    (void)val_len;
    printf("Published \n");
}

void *mqtt_thread(void *arg) {
    (void)arg;
    while(1) {
        mqtt_client_ping(&mqtt_data.client);
        xtimer_usleep(RUN_INTERVAL);
        mqtt_client_publish(&mqtt_data.client, TOPIC, "work", 4, 0, 0);
    }
    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    mqtt_data.gw.host = MQTT_HOST;
    mqtt_data.gw.port = MQTT_PORT;
    conn_params.client_id = CLIENT_ID;
    conn_params.user = NULL;
    conn_params.password = NULL;

    mqtt_client_con_t *conn = mqttsn_client_con_init(&mqtt_data.client, &conn_params,
                                                       &mqtt_data.gw, 0);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                        THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    if (pid < 0) {
        printf("Error: unable to create thread\n");
    }

    return 0;
}