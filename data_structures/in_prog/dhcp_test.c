#include <math.h>       /*  pow             */

#include "dhcp.h"
#include "tests.h"

#define OCTET       8
#define IP_BITS     (IP_OCTET_NUM * OCTET)

static void TestDHCP(void);
static void TestDHCPAllocate(dhcp_ty *dhcp);
static void TestDHCPFree(dhcp_ty *dhcp);

int main(void)
{
    TestDHCP();
    PASS;
    
    return 0;
}

static size_t subnet_bits_num = 0, host_bits_num = 0;
static size_t free_ips = 0;

static void TestDHCP(void)
{
    ip_ty subnet_ip = {192, 168, 111, 0};
    dhcp_ty *dhcp = NULL;
    
    subnet_bits_num = 24;
    host_bits_num = IP_BITS - subnet_bits_num;
    free_ips = pow(2, host_bits_num) - 3;
    
    dhcp = DHCPCreate(subnet_ip, subnet_bits_num);
    
    if (NULL == dhcp)
    {
        FAILED;
        return;
    }
    else
    {
        TEST("DHCPCountFree", DHCPCountFree(dhcp), free_ips);
        
        TestDHCPAllocate(dhcp);
        TestDHCPFree(dhcp);
        
        DHCPDestroy(dhcp);
        dhcp = NULL;
    }
}

static void TestDHCPAllocate(dhcp_ty *dhcp)
{
    ip_ty allocated_ip = {0};
    ip_ty requested_ip[] = {{192, 168, 111, 0},
                            {192, 168, 111, 1},
                            {192, 168, 111, 255},
                            {255, 255, 255, 255},
                            {0, 0, 0, 0},
                            {192, 168, 111, 150},
                            {192, 168, 111, 150}};
    ip_ty result_ip[] = {   {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {192, 168, 111, 150},
                            {192, 168, 111, 151}};
    ip_ty min_ip = {192, 168, 111, 2};
    status_ty res_stat[] = {FAILURE,
                            FAILURE,
                            FAILURE,
                            FAILURE,
                            FAILURE,
                            SUCCESS,
                            SUCCESS};
    status_ty status = SUCCESS;
    size_t idx = 0, cases = sizeof(res_stat) / sizeof(res_stat[0]);
    
    for (idx = 0; idx < cases; ++idx)
    {
        printf("requested_ip: %u.%u.%u.%u\n", requested_ip[idx][0], requested_ip[idx][1], requested_ip[idx][2], requested_ip[idx][3]);
        
        status = DHCPAllocateIP(dhcp, requested_ip[idx], allocated_ip);
        TEST("DHCPAllocateIP", status, res_stat[idx]);
        if (SUCCESS == status)
        {
            --free_ips;
        }
        
        printf("allocated_ip: %u.%u.%u.%u\n", allocated_ip[0], allocated_ip[1], allocated_ip[2], allocated_ip[3]);
        TEST_BYTES("DHCPAllocateIP", allocated_ip, result_ip[idx], IP_OCTET_NUM);
        
        TEST("DHCPCountFree", DHCPCountFree(dhcp), free_ips);
    }
    
    puts("requested_ip: NULL");
    
    status = DHCPAllocateIP(dhcp, NULL, allocated_ip);
    TEST("DHCPAllocateIP", status, SUCCESS);
    if (SUCCESS == status)
    {
        --free_ips;
    }
    
    printf("allocated_ip: %u.%u.%u.%u\n", allocated_ip[0], allocated_ip[1], allocated_ip[2], allocated_ip[3]);
    TEST_BYTES("DHCPAllocateIP", allocated_ip, min_ip, IP_OCTET_NUM);
    
    TEST("DHCPCountFree", DHCPCountFree(dhcp), free_ips);
}

static void TestDHCPFree(dhcp_ty *dhcp)
{
    ip_ty allocated_ip = {0};
    ip_ty ip_to_free[] = {  {192, 168, 111, 0},
                            {192, 168, 111, 1},
                            {192, 168, 111, 255},
                            {255, 255, 255, 255},
                            {0, 0, 0, 0},
                            {192, 168, 111, 150},
                            {192, 168, 111, 150}};
    status_ty res_stat[] = {FAILURE,
                            FAILURE,
                            FAILURE,
                            DOUBLE_FREE_FAILURE,
                            DOUBLE_FREE_FAILURE,
                            SUCCESS,
                            DOUBLE_FREE_FAILURE};
    status_ty status = SUCCESS;
    size_t idx = 0, cases = sizeof(res_stat) / sizeof(res_stat[0]);
    
    for (idx = 0; idx < cases; ++idx)
    {
        printf("ip_to_free: %u.%u.%u.%u\n", ip_to_free[idx][0], ip_to_free[idx][1], ip_to_free[idx][2], ip_to_free[idx][3]);
        status = DHCPFreeIP(dhcp, ip_to_free[idx]);
        TEST("DHCPFreeIP", status, res_stat[idx]);
        printf("result: %u\n", status);
        if (SUCCESS == status)
        {
            ++free_ips;
        }
        
        TEST("DHCPCountFree", DHCPCountFree(dhcp), free_ips);
    }
    
    idx -= 2;
    
    printf("requested_ip: %u.%u.%u.%u\n", ip_to_free[idx][0], ip_to_free[idx][1], ip_to_free[idx][2], ip_to_free[idx][3]);
    status = DHCPAllocateIP(dhcp, ip_to_free[idx], allocated_ip);
    TEST("DHCPAllocateIP", status, res_stat[idx]);
    if (SUCCESS == status)
    {
        --free_ips;
    }
    
    printf("allocated_ip: %u.%u.%u.%u\n", allocated_ip[0], allocated_ip[1], allocated_ip[2], allocated_ip[3]);
    TEST_BYTES("DHCPAllocateIP", allocated_ip, ip_to_free[idx], IP_OCTET_NUM);
    
    TEST("DHCPCountFree", DHCPCountFree(dhcp), free_ips);
}