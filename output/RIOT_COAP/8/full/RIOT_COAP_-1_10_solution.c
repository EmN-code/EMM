#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <kernel_defines.h>
#include <msg.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mutex.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht.h>
#include <dht_params.h>
#include <gcoap.h>
#include <mqtt_client.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>

#define SERVER_PORT 5683
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

int main(void) {
    gcoap_resp_handler_t _resp_handler = (gcoap_resp_handler_t)NULL;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    int rc;

    // Initialize the remote endpoint
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    rc = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (rc != 1) {
        puts("Error: unable to parse IP address");
        return 1;
    }

    // Initialize CoAP packet
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_NON);

    // Copy the payload into the packet's memory
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    while (1) {
        rc = gcoap_req_send(&buf[0], pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if(rc == -ENOMEM){
            printf("gcoap_req_send: Out of memory. Retrying in 1 second...\n");
            xtimer_sleep(1);
        } else if (rc < 0) {
            printf("gcoap_req_send: Error occurred (%d).\n", rc);
            xtimer_sleep(1);
        } else {
            printf("CoAP message sent successfully\n");
            break;
        }
    }
    return 0;
}