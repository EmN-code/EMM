
#include <stdio.h>
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_send_post_request(void) {
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, 47, 102, 103, 1);

    udp_socket_t socket;
    udp_socket_init(&socket);
    udp_socket_bind(&socket, UIP_HTONS(0));
    udp_socket_connect(&socket, &server_ip, UIP_HTONS(COAP_SERVER_PORT));

    uint8_t buffer[128];
    uint16_t buffer_len = 0;

    // Construct CoAP POST request
    buffer[buffer_len++] = 0x40; // CoAP version and type (CON)
    buffer[buffer_len++] = 0x02; // POST method
    buffer[buffer_len++] = 0x00; // Message ID (low byte)
    buffer[buffer_len++] = 0x00; // Message ID (high byte)

    // Token (optional, not used here)
    buffer[buffer_len++] = 0x00;

    // URI path option
    buffer[buffer_len++] = 0xB1; // Option delta 11, length 1
    buffer[buffer_len++] = '/';
    buffer[buffer_len++] = 0xB5; // Option delta 11, length 5
    buffer[buffer_len++] = 'c';
    buffer[buffer_len++] = 'o';
    buffer[buffer_len++] = 'a';
    buffer[buffer_len++] = 'p';
    buffer[buffer_len++] = '/';
    buffer[buffer_len++] = 't';
    buffer[buffer_len++] = 'e';
    buffer[buffer_len++] = 's';
    buffer[buffer_len++] = 't';

    // Payload
    buffer[buffer_len++] = 0xFF; // Payload marker
    const char *payload = "Hello, COAP Cloud";
    memcpy(&buffer[buffer_len], payload, strlen(payload));
    buffer_len += strlen(payload);

    udp_socket_send(&socket, buffer, buffer_len);
    udp_socket_close(&socket);
}

int main(void) {
    printf("Starting CoAP POST request to %s:%d%s\n", COAP_SERVER_IP, COAP_SERVER_PORT, COAP_URI_PATH);
    coap_send_post_request();
    return 0;
}
