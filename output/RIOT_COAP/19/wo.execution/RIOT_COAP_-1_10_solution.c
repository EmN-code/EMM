#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "kernel_defines.h"

#define CONFIG_GCOAP_PDU_BUF_SIZE 256
#define PAYLOAD "Hello, COAP Cloud"
#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_URI_PATH "/coap/test"

int main(void) {
    // Initialize required variables and structures
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = {.port = 5683};

    // Convert server IP address string to byte representation
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4) != 1) {
        puts("Failed to convert IP address");
        return -1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Add payload to PDU
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));

    // Finalize the CoAP packet
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    
    // Send the request
    if (gcoap_req_send(buf, pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send CoAP request");
        return -1;
    }

    puts("CoAP request sent successfully");
    return 0;
}