#include <thread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/conn.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define STATE_TOPIC "esp32/state"

#define MSG_BUFFER_SIZE 128

static mqtt_client_t mqtt_client;
static mqtt_connect_client_info_t ci = MQTT_CONNECT_INFO_INIT;
static sock_udp_t sock;

static char mqtt_buf[256];
static char mqtt_topic[64];

static void on_pub_complete(void *arg, int status)
{
    (void)arg;
    if (status != 0) {
        printf("[MQTT] Publish failed with status %d\n", status);
    }
}

int main(void)
{
    int res;
    char state_msg[20];

    sock_udp_ep_t gw = { .family = AF_INET6 };
    inet_pton(AF_INET6, SERVER_IP, &gw.addr);
    gw.port = SERVER_PORT;

    /* Initialize MQTT Client */
    ci.client_id = "esp32_device";
    ci.user = NULL;
    ci.pass = NULL;

    mqtt_client_init(&mqtt_client, &sock);

    res = mqtt_connect(&mqtt_client, &gw, &ci, 0, mqtt_buf, sizeof(mqtt_buf),
                       on_pub_complete, NULL);
    if (res != 0) {
        printf("[MQTT] Connection failed: %d\n", res);
        return res;
    }

    printf("[MQTT] Connected successfully\n");

    while (1) {
        /* Publish state every 5 seconds */
        strcpy(state_msg, (rand() % 2 == 0) ? "work" : "not work");
        snprintf(mqtt_topic, sizeof(mqtt_topic), STATE_TOPIC);

        res = mqtt_publish(&mqtt_client, mqtt_topic, state_msg, strlen(state_msg),
                           MQTT_QOS_0, 0);

        if (res != 0) {
            printf("[MQTT] Publish failed: %d\n", res);
        }

        xtimer_sleep(5);
    }

    /* Cleanup */
    mqtt_disconnect(&mqtt_client);
    sock_udp_close(&sock);

    return 0;
}