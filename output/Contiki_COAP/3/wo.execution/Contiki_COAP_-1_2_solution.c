#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uiplib.h"
#include "contiki.h"
#include "contiki-net.h"
#include "contiki-conf.h"
#include "contiki-lib.h"
#include <stdlib.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define CLIENT_PORT 8765
#define COAP_URI_PATH "/coap/test"

PROCESS(coap_client_process, "CoAP Client Process");
AUTOSTART_PROCESSES(&coap_client_process);

static struct udp_socket client_socket;
static uip_ipaddr_t server_ipaddr;

static void udp_rx_callback(struct udp_socket *c, void *ptr,
                            const uip_ipaddr_t *source_addr,
                            uint16_t source_port,
                            const uip_ipaddr_t *dest_addr,
                            uint16_t dest_port,
                            const uint8_t *data, uint16_t datalen) {
    printf("Received response from server: %.*s\n", datalen, data);
}

PROCESS_THREAD(coap_client_process, ev, data) {
    PROCESS_BEGIN();

    uiplib_ip6addrconv(SERVER_IP, &server_ipaddr);

    udp_socket_register(&client_socket, NULL, udp_rx_callback);
    udp_socket_bind(&client_socket, CLIENT_PORT);
    udp_socket_connect(&client_socket, &server_ipaddr, SERVER_PORT);

    uint8_t payload[] = "Hello, COAP Cloud";
    uip_udp_packet_sendto(client_socket.udp_conn, payload, sizeof(payload),
                          &server_ipaddr, UIP_HTONS(SERVER_PORT));

    printf("Sent CoAP POST request to %s:%d\n", SERVER_IP, SERVER_PORT);

    PROCESS_END();
}