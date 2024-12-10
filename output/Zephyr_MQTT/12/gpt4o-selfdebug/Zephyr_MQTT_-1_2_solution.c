#include <zephyr/net/mqtt.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/net_context.h>
#include <zephyr/kernel.h>
#include <string.h>

#define MQTT_CLIENTID CONFIG_MQTT_CLIENT_ID
#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_KEEPALIVE 60
#define STACK_SIZE 4096
#define THREAD_PRIORITY 6

static struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];

static struct sockaddr_storage broker;

static void prepare_fds(struct mqtt_client *c) {
    /* Prepare socket structures and set broker address */
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;
    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(MQTT_BROKER_PORT);
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker4->sin_addr);
    
    c->broker = &broker;
    
    mqtt_client_init(c, MQTT_CLIENTID, &broker4->sin_addr, mqtt_evt_cb, rx_buffer, sizeof(rx_buffer), tx_buffer, sizeof(tx_buffer));
    
    /* Set keepalive interval */
    c->keepalive = MQTT_KEEPALIVE;
}

void mqtt_evt_cb(struct mqtt_client *const c, const struct mqtt_evt *evt) {
    /* Handle MQTT events, necessary for MQTT protocol */
}

void mqtt_publish_work_state(struct mqtt_client *c) {
    /* Setup publish parameters */
    struct mqtt_publish_param param;
    param.message.topic.qos = MQTT_QOS_0_AT_MOST_ONCE;
    param.message.topic.topic.utf8 = "work/state";
    param.message.topic.topic.size = strlen(param.message.topic.topic.utf8);
    param.message.payload.data = "work";
    param.message.payload.len = strlen(param.message.payload.data);

    mqtt_publish(c, &param);
}

void mqtt_thread(void) {
    prepare_fds(&client);
    mqtt_connect(&client);
    while (1) {
        mqtt_publish_work_state(&client);
        k_sleep(K_SECONDS(5));
    }
}

K_THREAD_DEFINE(mqtt_thread_id, STACK_SIZE, mqtt_thread, NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);