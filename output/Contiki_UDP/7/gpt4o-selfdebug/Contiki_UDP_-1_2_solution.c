#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/simple-udp.h"

#include <stdio.h>
#include <string.h>

PROCESS(udp_client_process, "UDP Client Process");
AUTOSTART_PROCESSES(&udp_client_process);

static struct simple_udp_connection udp_conn;

static void send_packet(struct simple_udp_connection *c) {
    const char *state = "work";  // Change to "not work" if needed
    uip_ipaddr_t dest_ip;
    uip_ip6addr(&dest_ip, 0x47, 0x102, 0x103, 0x1, 0, 0, 0, 0x1); // IPv6 conversion

    simple_udp_sendto(c, state, strlen(state) + 1, &dest_ip);
    printf("Sent state: %s\n", state);
}

PROCESS_THREAD(udp_client_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    simple_udp_register(&udp_conn, 1234, NULL, 20001, NULL); // local port set to 1234

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        send_packet(&udp_conn);
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    PROCESS_BEGIN();
    process_start(&udp_client_process, NULL);
    PROCESS_END();
    return 0;
}