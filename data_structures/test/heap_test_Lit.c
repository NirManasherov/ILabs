#include "heap.h"
#include "tests.h"

static void TestHeap(void);
static int CmpFunc(void *heap_data, void *data_to_match);
static int IsMatch(const void *heap_data, const void *data_to_match);

int main(void)
{
    TestHeap();
    PASS;
    
    return 0;
}

static void TestHeap(void)
{
    size_t arr[] = {50, 25, 46, 32, 100, 17, 19, 22, 11, 94, 33};
    size_t expected_root[] = {100, 94, 50, 46, 33, 32, 25, 22, 19, 17, 11};
    size_t arr_size = sizeof(arr) / sizeof(arr[0]);
    size_t idx = 0;
    heap_ty *heap = HeapCreate(CmpFunc);
    
    if (NULL == heap)
    {
        FAILED;
        return;
    }
    
    TEST("HeapSize", HeapSize(heap), 0);
    TEST("HeapIsEmpty", HeapIsEmpty(heap), 1);
    
    for (idx = 0; idx < arr_size; ++idx)
    {
        TEST("HeapPush", HeapPush(heap, arr + idx), 0);
        TEST("HeapSize", HeapSize(heap), idx + 1);
        TEST("HeapIsEmpty", HeapIsEmpty(heap), 0);
    }
    
    for (idx = 0; idx < arr_size; ++idx)
    {
        TEST("HeapPeek", *(size_t *)HeapPeek(heap), expected_root[idx]);
        
        HeapPop(heap);
        
        TEST("HeapSize", HeapSize(heap), arr_size - idx - 1);
        TEST("HeapIsEmpty", HeapIsEmpty(heap), !(arr_size - idx - 1));
    }
    
    for (idx = 0; idx < arr_size; ++idx)
    {
        TEST("HeapPush", HeapPush(heap, arr + idx), 0);
        TEST("HeapSize", HeapSize(heap), idx + 1);
        TEST("HeapIsEmpty", HeapIsEmpty(heap), 0);
    }
    
    for (idx = 0; idx < arr_size; ++idx)
    {
        TEST("HeapRemove",
            (size_t)HeapRemove(heap, IsMatch, arr + idx),
            arr[idx]);
        TEST("HeapSize", HeapSize(heap), arr_size - idx - 1);
        TEST("HeapIsEmpty", HeapIsEmpty(heap), !(arr_size - idx - 1));
    }
    
    HeapDestroy(heap);
}

static int CmpFunc(void *heap_data, void *data_to_match)
{
    return *(size_t *)heap_data - *(size_t *)data_to_match;
}

static int IsMatch(const void *heap_data, const void *data_to_match)
{
    return *(size_t *)heap_data == *(size_t *)data_to_match;
}