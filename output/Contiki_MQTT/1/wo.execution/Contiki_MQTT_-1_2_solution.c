#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uiplib.h"
#include "net/mac/framer/framer.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki.h"
#include "mqtt-prop.h"
#include "mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define KEEP_ALIVE 60
#define CLEAN_SESSION 1
#define CLIENT_ID "nrf52840_client"
#define PUBLISH_INTERVAL (5 * CLOCK_SECOND)

PROCESS(mqtt_example_process, "MQTT Example Process");
AUTOSTART_PROCESSES(&mqtt_example_process);

static struct mqtt_connection conn;
static char *state_topic = "device/state";
static char *state_message = "work or not work";
static struct etimer publish_timer;

PROCESS_THREAD(mqtt_example_process, ev, data) {
    PROCESS_BEGIN();

    // Erase default process control using the Contiki etimer
    etimer_set(&publish_timer, PUBLISH_INTERVAL);

    printf("Connecting to MQTT broker at %s:%d ...\n", BROKER_IP, BROKER_PORT);
    mqtt_connect(&conn, BROKER_IP, BROKER_PORT, KEEP_ALIVE, CLEAN_SESSION);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && etimer_expired(&publish_timer));

        printf("Publishing state...\n");
        mqtt_publish(&conn, NULL, state_topic, (const uint8_t *)state_message, strlen(state_message), 0, 0, NULL);

        etimer_reset(&publish_timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&mqtt_example_process, NULL);
    PROCESS_WAIT_EVENT_UNTIL(0);
    return 0;
}