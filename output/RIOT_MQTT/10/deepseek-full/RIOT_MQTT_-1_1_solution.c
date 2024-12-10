
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/af.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static mqtt_client_opt_t client_opt;
static mqtt_connect_opt_t conn_opt;
static mqtt_publish_opt_t pub_opt;

static void _mqtt_connect(void)
{
    client_opt.keep_alive = 60;
    client_opt.client_id = "riot_esp32";
    client_opt.clean_session = 1;
    client_opt.will_flag = 0;

    conn_opt.client_opt = &client_opt;
    conn_opt.broker_ip = BROKER_IP;
    conn_opt.broker_port = BROKER_PORT;

    mqtt_connect(&client, &conn_opt);
}

static void _mqtt_publish(const char *payload)
{
    pub_opt.qos = MQTT_QOS_0;
    pub_opt.retain = 0;
    pub_opt.topic = TOPIC;
    pub_opt.payload = payload;
    pub_opt.payload_len = strlen(payload);

    mqtt_publish(&client, &pub_opt);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize MQTT client
    _mqtt_connect();

    // Main loop
    bool work_state = true;
    while (1) {
        if (work_state) {
            _mqtt_publish(PAYLOAD_WORK);
        } else {
            _mqtt_publish(PAYLOAD_NOT_WORK);
        }
        work_state = !work_state;

        xtimer_usleep(INTERVAL);
    }

    return 0;
}
