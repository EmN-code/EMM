
#include <stdio.h>
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "coap-request-state.h"
#include "udp-socket.h"
#include "tcp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

struct coap_blocking_request_state {
  coap_request_state_t state;
  struct pt pt;
  struct process *process;
} coap_blocking_request_state_t;

int main(void) {
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, COAP_SERVER_IP);

    coap_blocking_request_state_t request_state;
    coap_request_state_init(&request_state.state);

    coap_request_state_set_method(&request_state.state, COAP_METHOD_POST);
    coap_request_state_set_uri(&request_state.state, COAP_URI_PATH);
    coap_request_state_set_payload(&request_state.state, "Hello, COAP Cloud", 16);

    udp_socket_t socket;
    udp_socket_init(&socket);
    udp_socket_connect(&socket, &server_ip, COAP_SERVER_PORT);

    coap_request_state_send(&request_state.state, &socket);

    return 0;
}
