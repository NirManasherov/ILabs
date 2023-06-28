#include <stdio.h>

#include "dhcp.h"
#include "tests.h"

static void TestDHCP(void);

int main(void)
{
	TestDHCP();
	PASS;
	return 0;
}

static void TestDHCP(void)
{
	int i = 0;
	unsigned char buffer[4] = {0};
	unsigned char host_105[4] = {'x', 'y', '1', (char)105};
	unsigned char host_2[4] = {'x', 'y', '1', (char)2};
	unsigned char diff_subnet[4] = {'x', 'y', '2', (char)106};
	unsigned char forbidden_ip1[4] = {'x', 'y', '1', (char)0};
	unsigned char forbidden_ip2[4] = {'x', 'y', '1', (char)1};
	unsigned char ip[4] = {'x', 'y', '1', 'y'};
	dhcp_ty *handle = NULL;
	
	handle = DHCPCreate(ip, 24);
	
	if (NULL == handle)
	{
		printf("Create Failed\n");
	}
	
	TEST("test count free - empty ", DHCPCountFree(handle), 253);
	
	/* check allocations */
	
	/* check allocation - valid */
	DHCPAllocateIP(handle, host_105, buffer);
	
	/* alloc host: 105 */
	for (i = 0 ; i < 3 ; ++i)
	{
		printf("%d.",buffer[i]);
	}
	
	printf("%d\n",buffer[3]);
	
	TEST("test count free - after 1 alloc ", DHCPCountFree(handle), 252);
	
	/* check allocation - taken, so the next one */
	DHCPAllocateIP(handle, host_105, buffer);
	
	/* alloc host: 106 */
	for (i = 0 ; i < 3 ; ++i)
	{
		printf("%d.",buffer[i]);
	}
	
	printf("%d\n",buffer[3]);
	
	TEST("test count free - after 2 alloc ", DHCPCountFree(handle), 251);
	
	/* check allocation - not valid cuz of subnet part isnt the same */
	TEST("alloc but subnet is diff", DHCPAllocateIP(handle, diff_subnet, buffer), FAILURE);
	
	/* check allocation - not valid cuz saved IP */
	TEST("saved ip", DHCPAllocateIP(handle, forbidden_ip1, buffer), FAILURE);
	
	/* check allocation - not valid cuz saved IP */
	TEST("saved ip", DHCPAllocateIP(handle, forbidden_ip2, buffer), FAILURE);
	
	/* test DHCPFreeIP */
	
	/* check free ip - free host 105 */
	TEST("valid free ip",DHCPFreeIP(handle, host_105), SUCCESS);
	
	TEST("test count free - after 1 free ", DHCPCountFree(handle), 252);
	
	/* check free ip - double free one */
	TEST("double free one",DHCPFreeIP(handle, host_105), DOUBLE_FREE_FAILURE);
	
	/* check free ip - out of range because of subnet */
	TEST("double free one", DHCPFreeIP(handle, diff_subnet), FAILURE);
	
	/* check free ip - forbidden free one */
	TEST("forbidden free",DHCPFreeIP(handle, forbidden_ip1), FAILURE);
	
	/* check free ip - more forbidden */
	TEST("more forbidden free",DHCPFreeIP(handle, forbidden_ip2), FAILURE);
	
	/* check allocations after some frees */
	/* alloc host: 105 */
	DHCPAllocateIP(handle, host_105, buffer);
	
	for (i = 0 ; i < 3 ; ++i)
	{
		printf("%d.",buffer[i]);
	}
	
	printf("%d\n",buffer[3]);
	
	TEST("test count free - alloc for the freed ip ", DHCPCountFree(handle), 251);
	
	/* alloc host: 107 (105 taken, and 107 taken as well) */
	DHCPAllocateIP(handle, host_105, buffer);
	
	for (i = 0 ; i < 3 ; ++i)
	{
		printf("%d.",buffer[i]);
	}
	
	printf("%d\n",buffer[3]);
	
	TEST("test count free - after more alloc ", DHCPCountFree(handle), 250);
	
	/* check allocations for NULL (min ip) */
	/* alloc host: 2 */
	DHCPAllocateIP(handle, NULL, buffer);
	
	for (i = 0 ; i < 3 ; ++i)
	{
		printf("%d.",buffer[i]);
	}
	
	printf("%d\n",buffer[3]);
	
	TEST("test count free - alloc min ip ", DHCPCountFree(handle), 249);
	
	/* alloc host: 3 */
	DHCPAllocateIP(handle, NULL, buffer);
	
	for (i = 0 ; i < 3 ; ++i)
	{
		printf("%d.",buffer[i]);
	}
	
	printf("%d\n",buffer[3]);
	
	TEST("test count free - alloc min ip ", DHCPCountFree(handle), 248);
	
	/* free host 2 */
	TEST("valid free ip",DHCPFreeIP(handle, host_2), SUCCESS);
	
	TEST("test count free - free one min ip ", DHCPCountFree(handle), 249);
	
	DHCPDestroy(handle);
}
