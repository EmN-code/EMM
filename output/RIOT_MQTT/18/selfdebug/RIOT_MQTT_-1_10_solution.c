#include <xtimer.h>
#include <string.h>
#include <net/mqtt.h>
#include <net/sock/udp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_INTERVAL (5 * 1000000U) // 5 seconds

static sock_udp_ep_t remote = {
    .family = AF_INET6,
    .port = MQTT_BROKER_PORT
};

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
mqtt_client_t client;
mqtt_client_config_t client_config;
static char mqtt_buf[128];
static char mqtt_rbuf[64];

static void *mqtt_thread(void *arg)
{
    (void)arg;
    msg_init_queue(queue, 8);
    while(1) {
        // Publish to the broker
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), 0);
        xtimer_usleep(MQTT_INTERVAL);
    }
    return NULL;
}

static int mqtt_init(void)
{
    remote.addr.ipv6[0] = 0x47;
    remote.addr.ipv6[1] = 0x66;
    remote.addr.ipv6[2] = 0x67;
    remote.addr.ipv6[3] = 0x01;
    printf("Connecting to MQTT broker at [%s]:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    client_config.hostname = MQTT_BROKER_IP;
    client_config.port = MQTT_BROKER_PORT;
    client_config.clientid = "esp32client";
    client_config.userdata = NULL;
    client_config.topic_data = mqtt_buf;
    client_config.topic_data_maxlen = sizeof(mqtt_buf);
    client_config.rx_buffer = mqtt_rbuf;
    client_config.rx_buffer_size = sizeof(mqtt_rbuf);
    client_config.cb = NULL;
    client_config.use_specify_con_handler = 0;

    mqtt_connect(&client, &client_config, &remote);
    printf("Connected to MQTT broker\n");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}

int main(void)
{
    mqtt_init();
    return 0;
}