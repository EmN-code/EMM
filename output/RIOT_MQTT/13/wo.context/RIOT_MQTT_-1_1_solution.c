// Include header files in descending alphabetical order by their first letter.
#include "thread.h"
#include "stdio.h"
#include "mqtt.h"
#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "net.h"  

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define MESSAGE_WORK "work"
#define MESSAGE_NOT_WORK "not work"
#define INTERVAL (5U)

static char mqtt_send_buf[128];
static char mqtt_recv_buf[128];
static mqtt_client_t mqtt_client;
static msg_t queue[8];
static volatile int work_state = 0;

static void *publish_thread(void *arg) {
    (void)arg;

    mqtt_client_t *client = (mqtt_client_t *)arg;
    int res;

    while (1) {
        thread_sleep((5 * MS_PER_SEC));

        const char *message = work_state == 0 ? MESSAGE_NOT_WORK : MESSAGE_WORK;
        mqtt_message_t pub_msg;
        memset(&pub_msg, 0, sizeof(pub_msg));
        pub_msg.qos = MQTT_QOS_0;
        pub_msg.payload = (void *)message;
        pub_msg.payload_size = strlen(message);

        if ((res = mqtt_publish(client, TOPIC, &pub_msg)) != MQTT_OK) {
            printf("Error publishing to topic %s [%d]\n", TOPIC, res);
        } else {
            printf("Published `%s` to topic `%s`\n", message, TOPIC);
        }

        work_state = !work_state; // Toggle work state for testing.
    }

    return NULL;
}

static void on_conneted(mqtt_client_t *client,  void *arg){
    (void)arg;
    printf("On connected\n");

    static char pub_stack[THREAD_STACKSIZE_MAIN];
    thread_create(pub_stack, sizeof(pub_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  publish_thread, client, "Publish");
}

int main(void) {
    static mqtt_client_config_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    cfg.broker_addr.family = AF_INET6;
    ipv6_addr_from_str((ipv6_addr_t *)&cfg.broker_addr.addr.ipv6, BROKER_IP);
    cfg.broker_port = BROKER_PORT;
    cfg.client_id = "esp32_device";
    cfg.user = NULL;
    cfg.pass = NULL;
    cfg.keep_alive = 10;
    cfg.transport = MQTT_TRANSPORT_TCP;
    cfg.timeout = 200;

    mqtt_init(&mqtt_client, mqtt_send_buf, sizeof(mqtt_send_buf),
              mqtt_recv_buf, sizeof(mqtt_recv_buf), &cfg);

    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));

    mqtt_register_callback(&mqtt_client, on_conneted);

    int res = mqtt_connect(&mqtt_client);
    if (res != MQTT_OK) {
        printf("Could not connect to MQTT broker: %d\n", res);
        return 1;
    }

    printf("Connected to MQTT broker\n");

    return 0;
}