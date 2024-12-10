
#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include <stdio.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 20001
#define DEVICE_STATE "work"  // or "not work"

static struct simple_udp_connection udp_conn;
static struct etimer et;

void udp_send_state(void) {
    static char buffer[] = DEVICE_STATE;
    simple_udp_sendto(&udp_conn, buffer, sizeof(buffer), (uip_ipaddr_t *)SERVER_IP, SERVER_PORT);
}

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 0, NULL, SERVER_PORT, NULL);

    etimer_set(&et, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        udp_send_state();
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void) {
    process_init();
    process_start(&udp_client_process, NULL);
    while(1) {
        process_run();
    }
    return 0;
}
