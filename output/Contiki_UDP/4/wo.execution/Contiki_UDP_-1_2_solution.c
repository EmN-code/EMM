#include <contiki.h>
#include <net/ipv6/uip-ds6-route.h>
#include <net/ipv6/uip-ds6-nbr.h>
#include <net/ipv6/uip-ds6.h>
#include <net/ipv6/uip.h>
#include <net/ipv6/uiplib.h>
#include <net/ipv6/uip-nameserver.h>
#include <net/ipv6/uip-icmp6.h>
#include <net/ipv6/simple-udp.h>
#include <net/ipv6/tcp-socket.h>
#include <net/ipv6/tcpip.h>
#include <net/ipv6/udp-socket.h>
#include <net/linkaddr.h>
#include <net/mac/framer/framer.h>
#include <net/mac/mac.h>
#include <net/netstack.h>
#include <net/routing/routing.h>
#include <sys/autostart.h>
#include <sys/ctimer.h>
#include <sys/etimer.h>
#include <sys/log.h>
#include <sys/pt.h>
#include <sys/process.h>
#include <sys/rtimer.h>
#include <sys/timer.h>
#include <dev/etc/rgb-led/rgb-led.h>
#include <contiki-default-conf.h>

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define UDP_SERVER_IP "47.102.103.1"
#define UDP_SERVER_PORT 20001
#define MESSAGE_INTERVAL (5 * CLOCK_SECOND)

static struct simple_udp_connection udp_conn;
static char buf[32];

PROCESS(udp_client_process, "UDP client process");
AUTOSTART_PROCESSES(&udp_client_process);

PROCESS_THREAD(udp_client_process, ev, data) {
  static struct etimer timer;
  static uip_ipaddr_t server_ipaddr;

  PROCESS_BEGIN();

  uiplib_ipaddrconv(UDP_SERVER_IP, &server_ipaddr);
  simple_udp_register(&udp_conn, UDP_SERVER_PORT, NULL, UDP_SERVER_PORT, NULL);

  etimer_set(&timer, MESSAGE_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    snprintf(buf, sizeof(buf), "Device state: %s", "work");
    simple_udp_sendto(&udp_conn, buf, strlen(buf) + 1, &server_ipaddr);

    etimer_reset(&timer);
  }
  PROCESS_END();
}