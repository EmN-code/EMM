#include <zephyr.h>
#include <string.h>
#include <stdio.h>
#include <net/mqtt.h>
#include <net/socket.h>
#include <net/net_ip.h>

#define BROKER_IP_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENTID "esp32_client"

#define STACK_SIZE 1024
#define THREAD_PRIORITY K_PRIO_PREEMPT(6)

static struct mqtt_client client;
static uint8_t rx_buffer[128];
static uint8_t tx_buffer[128];
static struct sockaddr_storage broker;

/* Buffers for MQTT payload */
static char payload[32];

void prepare_fds(struct mqtt_client *client, struct pollfd *fds)
{
    fds->fd = client->transport.tcp.sock;
    fds->events = POLLIN;
}

void mqtt_evt_handler(struct mqtt_client *const c, const struct mqtt_evt *evt)
{
    switch (evt->type) {
    case MQTT_EVT_CONNACK:
        if (evt->result != 0) {
            printf("MQTT connect failed %d\n", evt->result);
            break;
        }
        printf("MQTT client connected!\n");
        break;

    case MQTT_EVT_DISCONNECT:
        printf("MQTT client disconnected %d\n", evt->result);
        break;

    case MQTT_EVT_PUBLISH:
        printf("MQTT publish received: [ignored]\n");
        break;

    case MQTT_EVT_PUBACK:
        if (evt->result != 0) {
            printf("MQTT PUBACK error %d\n", evt->result);
            break;
        }

        printf("PUBACK packet id: %u\n", evt->param.puback.message_id);
        break;
    default:
        break;
    }
}

int publish(struct mqtt_client *client, enum mqtt_qos qos)
{
    struct mqtt_publish_param param;

    param.message.topic.qos = qos;
    param.message.topic.topic.utf8 = (uint8_t *)"state/update";
    param.message.topic.topic.size = strlen("state/update");
    param.message.payload.data = payload;
    param.message.payload.len = strlen(payload);
    param.message_id = 1;
    param.dup_flag = 0U;
    param.retain_flag = 0U;

    return mqtt_publish(client, &param);
}

void broker_init(void)
{
    struct sockaddr_in *broker4 = (struct sockaddr_in *)&broker;

    broker4->sin_family = AF_INET;
    broker4->sin_port = htons(BROKER_PORT);
    net_addr_pton(AF_INET, BROKER_IP_ADDR, &broker4->sin_addr);
}

void main(void)
{
    int blink_status = 0;

    printf("MQTT simple publisher sample\n");

    mqtt_client_init(&client);

    broker_init();

    client.broker = &broker;
    client.evt_cb = mqtt_evt_handler;
    client.client_id.utf8 = (uint8_t *)MQTT_CLIENTID;
    client.client_id.size = strlen(MQTT_CLIENTID);
    client.rx_buf = rx_buffer;
    client.rx_buf_size = sizeof(rx_buffer);
    client.tx_buf = tx_buffer;
    client.tx_buf_size = sizeof(tx_buffer);

    mqtt_connect(&client);

    struct pollfd fds;

    while (1) {
        prepare_fds(&client, &fds);

        if (poll(&fds, 1, K_SECONDS(1)) < 0) {
            printf("ERROR: poll error\n");
            continue;
        }

        /* Check if data is available to read */
        mqtt_input(&client);

        /* Process changing state */
        blink_status = !blink_status;
        strcpy(payload, blink_status ? "work" : "not work");
        publish(&client, MQTT_QOS_1_AT_LEAST_ONCE);

        k_sleep(K_SECONDS(5));
    }
}