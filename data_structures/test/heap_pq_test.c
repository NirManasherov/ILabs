#include "heap.h"
#include "p_queue.h"
#include "tests.h"

/******************************************************************************/

static void TestPQueue(void);
static int PriorityCmp(void *ds_data, void *new_data);
static int IsMatch(const void *data, void *param);

/******************************************************************************/

static size_t cases[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
static size_t tests = sizeof(cases) / sizeof(cases[0]);
static size_t total_nodes = 0;

/******************************************************************************/

int main(void)
{
    TestPQueue();
    PASS;
    
    return 0;
}

/******************************************************************************/

static void TestPQueue(void)
{
    p_queue_ty *p_queue = PQueueCreate(PriorityCmp);
    size_t idx = 0;
    
    if (NULL == p_queue)
    {
        FAILED;
    }
    else
    {
        TEST("PQueueIsEmpty", PQueueIsEmpty(p_queue), 1);
        TEST("PQueueSize", PQueueSize(p_queue), 0);
        
        for (idx = 0; idx < tests; ++idx)
        {
            PQueueEnqueue(p_queue, (const void *)(cases + idx));
            
            ++total_nodes;
        }
        
        TEST("PQueueIsEmpty", PQueueIsEmpty(p_queue), 0);
        TEST("PQueueSize", PQueueSize(p_queue), total_nodes);
        
        for (idx = 0; idx < tests; ++idx)
        {
            TEST("PQueuePeek", 
                *(size_t *)PQueuePeek(p_queue),
                cases[tests - idx - 1]);
            
            TEST("PQueueSize", PQueueSize(p_queue), total_nodes);
            
            PQueueDequeue(p_queue);
            TEST("PQueueDequeue", PQueueSize(p_queue), --total_nodes);
        }
        
        TEST("PQueueIsEmpty", PQueueIsEmpty(p_queue), 1);
        
        for (idx = 0; idx < tests; ++idx)
        {
            PQueueEnqueue(p_queue, (const void *)(cases + idx));
        }
        
        PQueueClear(p_queue);
        TEST("PQueueClear", PQueueIsEmpty(p_queue), 1);
        TEST("PQueueClear", PQueueSize(p_queue), 0);
        
        for (idx = 0; idx < tests; ++idx)
        {
            PQueueEnqueue(p_queue, (const void *)(cases + idx));
        }
        
        for (idx = 0; idx < tests; idx = 2 * idx + 1)
        {
            TEST("PQueueErase", 
                (size_t)PQueueErase(p_queue, IsMatch, cases + idx),
                cases[idx]);
        }
        
        PQueueDestroy(p_queue);
    }
}

/******************************************************************************/

static int PriorityCmp(void *ds_data, void *new_data)
{
    return *(size_t *)ds_data - *(size_t *)new_data;
}

static int IsMatch(const void *data, void *param)
{
    return *(size_t *)data == *(size_t *)param;
}
