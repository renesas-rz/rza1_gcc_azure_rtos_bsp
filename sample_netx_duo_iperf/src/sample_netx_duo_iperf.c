/* This is the iPerf demonstration for Renesas RZ RSK CPU Board.  Simply execute this 
   demonstration and follow the direction in the NetX_Iperf_Demonstration.pdf that is included
   in this demonstration. Note that the IP address is setup in the nx_ip_create call below.  */

/* By default, enable DHCP for IP address. The IP address will be printed to the debugger console.  */
#define NX_ENABLE_DHCP

#include "tx_api.h"
#include "nx_api.h"
#include "nxd_dhcp_client.h"

#include "compiler_settings.h"

/* Define the ThreadX and NetX object control blocks...  */

NX_PACKET_POOL    pool_0;
NX_IP             ip_0;  

#ifdef NX_ENABLE_DHCP
/* Don't cache packet pool! */
NX_DHCP	NO_CACHE_SECTION	dhcp_client __attribute__ ((aligned (32)));
#endif
TX_THREAD         demo_thread;

/* Define the IP thread's stack area.  */

ULONG             ip_thread_stack[4 * 1024 / sizeof(ULONG)];
ULONG             demo_thread_stack[4096 / sizeof(ULONG)];


/* Define transmit packet pool for the demonstration.  */

#define NX_PACKET_PAYLOAD_SIZE  1536
#define NX_PACKET_POOL_SIZE     ((NX_PACKET_PAYLOAD_SIZE + sizeof(NX_PACKET)) * 60)
/* Add 32 to allow 32-byte alignment of rx buffers. */
/* Divide because this is a ULONG (not byte) array. */
/* Don't cache packet pool! */
ULONG	NO_CACHE_SECTION	packet_pool_area[NX_PACKET_POOL_SIZE/sizeof(ULONG) + 32/sizeof(ULONG)] __attribute__ ((aligned (32)));

/* Define the ARP cache area.  */

ULONG             arp_space_area[512 / sizeof(ULONG)];


/* Define a 4K space for the HTTP thread stack.*/

ULONG             http_thread_stack_area[4 * 1024 / sizeof(ULONG)];
                                                           

/* Define a 2K space for the IPerf Thread stack. */

ULONG             iperf_thread_stack_area[4 * 1024 / sizeof(ULONG)];
                                                           

/* Define an error counter.  */

ULONG             error_counter;


/* Define external references.  */ 

void    nx_iperf_entry(NX_PACKET_POOL *pool_ptr, NX_IP *ip_ptr, UCHAR* http_stack, ULONG http_stack_size, UCHAR *iperf_stack, ULONG iperf_stack_size);
extern  VOID nx_driver_renesas_rz(NX_IP_DRIVER*);
extern  VOID hwsetup(void);
static  VOID thread_entry(ULONG thread_input);

//initialise_monitor_handles();
int main()
{
    
    /* Enter the ThreadX kernel.  */
    tx_kernel_enter();
}



/* Define what the initial system looks like.  */
void    tx_application_define(void *first_unused_memory)
{

UINT  status;

    /* Create the main demo thread.  */
    tx_thread_create(&demo_thread, "Demo Thread", thread_entry, 
                     0,  demo_thread_stack, sizeof(demo_thread_stack), 
                     17, 17, TX_NO_TIME_SLICE, TX_AUTO_START); 
    
     
    /* Initialize the NetX system.  */
    nx_system_initialize();
    
    /* Create a packet pool. */
    /* Ensure packet pool area is 32-byte aligned. */
    status =  nx_packet_pool_create(&pool_0, "NetX Main Packet Pool", NX_PACKET_PAYLOAD_SIZE,  (ULONG*)(((int)packet_pool_area + 31) & ~31) , NX_PACKET_POOL_SIZE);

    /* Check for pool creation error.  */
    if (status)
        error_counter++;

    /* Create an IP instance.  */
    status = nx_ip_create(&ip_0, 
                          "NetX IP Instance 0", 
#ifdef NX_ENABLE_DHCP
                          IP_ADDRESS(0,0,0,0),
                          IP_ADDRESS(0,0,0,0), 
#else
                          IP_ADDRESS(192, 168, 1, 222), 	/* Static IP Address goes here!  */
                          0xFFFFFF00UL, 
#endif
                          &pool_0, nx_driver_renesas_rz,
                          (UCHAR*)ip_thread_stack,
                          sizeof(ip_thread_stack),
                          1);

    
     /* Check for IP create errors.  */
    if (status)
        error_counter++;
        
    /* Enable ARP and supply ARP cache memory for IP Instance 0.  */
    status =  nx_arp_enable(&ip_0, (void *)arp_space_area, sizeof(arp_space_area));

    /* Check for ARP enable errors.  */
    if (status)
        error_counter++;

    /* Enable TCP traffic.  */
    status =  nx_tcp_enable(&ip_0);
    
    /* Check for TCP enable errors.  */
    if (status)
        error_counter++;
    
    /* Enable UDP traffic.  */
    status =  nx_udp_enable(&ip_0);

    
    /* Check for UDP enable errors.  */
    if (status)
        error_counter++;

    /* Enable ICMP.  */
    status =  nx_icmp_enable(&ip_0);
    
     if (status)
        error_counter++;   
     
     status =  nx_ip_fragment_enable(&ip_0);

    /* Check for errors.  */
    if (status)
        error_counter++;
    
    /* Start the iperf web server based test.  This test serves a web page 
       at the IP address specified above.  */   
    nx_iperf_entry(&pool_0, &ip_0, (UCHAR*)http_thread_stack_area, sizeof(http_thread_stack_area), (UCHAR*)iperf_thread_stack_area, sizeof(iperf_thread_stack_area)); 
}


static  VOID thread_entry(ULONG thread_input)
{
ULONG ip_address;
ULONG network_mask;
    
#ifdef NX_ENABLE_DHCP
UINT  status;
ULONG actual_status;


    /* Create the DHCP instance.  */
    printf("DHCP In Progress...\n");

    nx_dhcp_create(&dhcp_client, &ip_0, "dhcp_client");

    /* Start the DHCP Client.  */
    nx_dhcp_start(&dhcp_client);
    
    /* Wait util address is solved. */
    status = nx_ip_status_check(&ip_0, NX_IP_ADDRESS_RESOLVED, &actual_status, 10000);
    
    if (status)
    {
        
        /* Failed */
        printf("Can't solve address\n");
    }
    else
#endif
    {
        
        /* Get IP address. */
        nx_ip_address_get(&ip_0, &ip_address, &network_mask);

        /* Output IP address. */
        printf("IP address: %d.%d.%d.%d\nMask: %d.%d.%d.%d", 
               (ip_address >> 24),
               ((ip_address >> 16) & 0xFF),
               ((ip_address >> 8) & 0xFF),
               (ip_address & 0xFF),               
               (network_mask >> 24),
               ((network_mask >> 16) & 0xFF),
               ((network_mask >> 8) & 0xFF),
               (network_mask & 0xFF));
    }
}

