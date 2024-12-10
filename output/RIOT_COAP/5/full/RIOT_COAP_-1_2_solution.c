#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "msg.h"

#include <stdio.h>
#include <string.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define BUFFER_SIZE 128

int main(void) {
    msg_t main_msg_queue[BUFFER_SIZE];
    msg_init_queue(main_msg_queue, BUFFER_SIZE);

    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    char payload[] = "Hello, COAP Cloud";
    coap_pkt_t pdu;
    uint8_t buf[BUFFER_SIZE];

    size_t len = gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, NULL);
    coap_opt_add_uri_path(&pdu, URI_PATH);
    memcpy(pdu.payload, payload, sizeof(payload));
    len += sizeof(payload);
    
    if (gcoap_req_send(buf, len, &remote) < 0) {
        printf("Error: unable to send CoAP request\n");
    } else {
        printf("CoAP request sent successfully\n");
    }

    return 0;
}