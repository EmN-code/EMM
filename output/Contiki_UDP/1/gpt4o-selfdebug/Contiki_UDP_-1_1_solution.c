#include "simple-udp.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "contiki.h"

#define UDP_SERVER_IP       "47.102.103.1"
#define UDP_SERVER_PORT     20001
#define SEND_INTERVAL       (5 * CLOCK_SECOND)

PROCESS(udp_sender_process, "UDP sender process");
AUTOSTART_PROCESSES(&udp_sender_process);

static struct simple_udp_connection udp_conn;
static char *device_state = "work";

PROCESS_THREAD(udp_sender_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, 0, NULL, UDP_SERVER_PORT, NULL);

  etimer_set(&timer, SEND_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    printf("Sending device state to %s:%u\n", UDP_SERVER_IP, UDP_SERVER_PORT);
    simple_udp_sendto(&udp_conn, device_state, strlen(device_state), &udp_conn.remote_addr);

    etimer_reset(&timer);
  }

  PROCESS_END();
}