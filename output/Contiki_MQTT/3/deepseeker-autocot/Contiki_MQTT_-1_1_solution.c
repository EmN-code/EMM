
#include "contiki.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "lib/random.h"
#include "dev/leds.h"
#include "mqtt.h"
#include "mqtt-transport.h"
#include "mqtt-packet.h"
#include "mqtt-strings.h"
#include "mqtt-process.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "nrf52840_client"

#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

static struct mqtt_connection conn;
static struct etimer publish_timer;

static void
mqtt_connect(void)
{
    mqtt_connect_params_t connect_params = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    mqtt_connect(&conn, &connect_params);
}

static void
mqtt_publish(const char *state)
{
    mqtt_publish_params_t publish_params = {
        .topic = MQTT_TOPIC,
        .payload = state,
        .payload_len = strlen(state),
        .qos = 0,
        .retain = 0
    };

    mqtt_publish(&conn, &publish_params);
}

PROCESS(mqtt_client_process, "MQTT Client Process");
AUTOSTART_PROCESSES(&mqtt_client_process);

PROCESS_THREAD(mqtt_client_process, ev, data)
{
    static char state[12];
    static int work_state = 1;

    PROCESS_BEGIN();

    mqtt_init(&conn, NULL, NULL, 0, NULL, NULL, 0);
    mqtt_transport_open(&conn, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    mqtt_connect();

    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&publish_timer));

        if(work_state) {
            strcpy(state, "work");
        } else {
            strcpy(state, "not work");
        }

        mqtt_publish(state);

        work_state = !work_state;

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&mqtt_client_process, NULL);
    return 0;
}
