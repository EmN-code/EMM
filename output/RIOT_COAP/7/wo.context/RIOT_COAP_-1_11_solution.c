#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP  "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

int main(void) {
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    ssize_t res;
    
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Error: unable to parse IP address");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, sizeof(PAYLOAD));

    res = gcoap_req_send(buf, pdu.hdr_len + pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (res <= 0) {
        printf("Error sending packet: %d\n", (int)res);
        return -1;
    }
    
    return 0;
}