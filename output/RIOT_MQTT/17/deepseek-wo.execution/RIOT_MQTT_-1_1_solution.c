
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

static void connect_to_broker(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, BROKER_IP);

    int res = sock_tcp_connect(&sock, &remote, &local, 0);
    if (res < 0) {
        puts("Error connecting to broker");
        return;
    }

    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect_info_t conn_info = {
        .client_id = "riot_esp32",
        .keep_alive = 60,
        .clean_session = 1,
    };

    res = mqtt_connect(&client, &conn_info);
    if (res != MQTT_OK) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void publish_state(const char *state) {
    mqtt_publish_info_t pub_info = {
        .topic = TOPIC,
        .payload = state,
        .payload_len = strlen(state),
        .qos = MQTT_QOS_0,
        .retain = 0,
    };

    int res = mqtt_publish(&client, &pub_info);
    if (res != MQTT_OK) {
        puts("Error publishing message");
    } else {
        printf("Published: %s\n", state);
    }
}

int main(void) {
    connect_to_broker();

    bool working = true;
    while (1) {
        if (working) {
            publish_state(PAYLOAD_WORK);
        } else {
            publish_state(PAYLOAD_NOT_WORK);
        }
        working = !working;
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
