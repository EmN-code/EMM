#include <stdio.h>
#include "net/nanocoap.h"
#include "net/coap.h"
#include "net/sock/udp.h"
#include "thread.h"
#include "msg.h"
#include "shell.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT_COAP "5683"
#define MESSAGE_DATA "Hello, COAP Cloud"
#define URI_PATH "/coap/test"

static sock_udp_ep_t remote;

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    
    /* resolve host address */
    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("ERROR: unable to parse ipv6 address");
        return 1;
    }
    remote.port = atoi(SERVER_PORT_COAP);
    remote.family = AF_INET6;

    coap_pkt_t pdu;
    uint8_t buf[64];
    coap_uri_t uri;
    coap_uri_parse(URI_PATH, &uri);

    size_t len = coap_build(buf, &pdu, COAP_TYPE_CON, &uri, COAP_METHOD_POST, COAP_FORMAT_TEXT, (uint8_t *)MESSAGE_DATA, strlen(MESSAGE_DATA));

    if (sock_udp_send(NULL, buf, len, &remote) < 0) {
        puts("Error sending packet");
    } 
    else {
        puts("CoAP POST sent successfully!");
    }
    
    return 0;
}