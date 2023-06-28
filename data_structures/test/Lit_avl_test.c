#include "avl.h"
#include "tests.h"

#define TREE_SIZE 11
#define MAX_WIDTH 11

typedef struct avl_node avl_node_ty;

struct avl_node
{
    ssize_t height;
    void *data;
    avl_node_ty *children[2];
};

struct avl
{
    avl_node_ty *root;
    cmp_func_ty cmp;
};

static void TestAVL(void);
static int CmpFunc(void *avl_data, void *data_to_match);
static int Print(void *avl_data, void *params);
static void RecursivePrint(avl_node_ty *node, int depth, int direction);

int main(void)
{
    TestAVL();
    PASS;
    
    return 0;
}

/******************************************************************************/

static void TestAVL(void)
{
    avl_ty *avl = AVLCreate(CmpFunc);
    int arr[TREE_SIZE] = {50, 25, 46, 32, 100, 17, 19, 22, 11, 94, 33};
    size_t idx = 0;
    
    if (NULL == avl)
    {
        FAILED;
    }
    else
    {
        TEST("AVLSize", AVLSize(avl), 0);
        TEST("AVLIsEmpty", AVLIsEmpty(avl), 1);
        TEST("AVLHeight", AVLHeight(avl), -1);
        
        for (idx = 0; idx < TREE_SIZE; ++idx)
        {
            TEST("AVLFind", AVLFind(avl, &arr[idx]), 1);
        }
        
        puts("insert:");
        
        for (idx = 0; idx < TREE_SIZE; ++idx)
        {
            TEST("AVLInsert", AVLInsert(avl, &arr[idx]), 0);
            
            printf("\ninsert %d:\n", arr[idx]);
            RecursivePrint(avl->root, 0, 0);
            puts("");
        }
        
        TEST("AVLHeight", AVLHeight(avl), 3);
        
        for (idx = 0; idx < TREE_SIZE; ++idx)
        {
            TEST("AVLFind", AVLFind(avl, &arr[idx]), 0);
        }
        
        TEST("AVLSize", AVLSize(avl), TREE_SIZE);
        TEST("AVLIsEmpty", AVLIsEmpty(avl), 0);
        
        puts("\nfor each:");
        
        puts("\tpre-order:");
        /*  25, 19, 17, 11, 22, 46, 32, 33, 94, 50, 100 */
        TEST("AVLForEach-PRE", AVLForEach(avl, PRE, Print, NULL), 0);
        
        puts("\n\tin-order:");
        /*  11, 17, 19, 22, 25, 32, 33, 46, 50, 94, 100 */
        TEST("AVLForEach-IN", AVLForEach(avl, IN, Print, NULL), 0);
        
        puts("\n\tpost-order:");
        /*  11, 17, 22, 19, 33, 32, 50, 100, 94, 46, 25 */
        TEST("AVLForEach-POST", AVLForEach(avl, POST, Print, NULL), 0);
        
        puts("\nremove:");
        
        for (idx = 0; idx < TREE_SIZE; ++idx)
        {
            printf("\tremove %d:\n", arr[idx]);
            
            AVLRemove(avl, &arr[idx]);
            
            TEST("AVLSize", AVLSize(avl), TREE_SIZE - idx - 1);
            TEST("AVLForEach-IN", AVLForEach(avl, IN, Print, NULL), 0);
            puts("");
            RecursivePrint(avl->root, 0, 0);
            puts("");
        }
        
        TEST("AVLIsEmpty", AVLIsEmpty(avl), 1);
        TEST("AVLHeight", AVLHeight(avl), -1);
        
        AVLDestroy(avl);
    }
}

/******************************************************************************/

static int CmpFunc(void *avl_data, void *data_to_match)
{
    return (*(int *)avl_data - *(int *)data_to_match);
}

static int Print(void *avl_data, void *params)
{
    (void)params;
    
    printf("%d, ", *(int *)avl_data);
    
    return 0;
}

static void RecursivePrint(avl_node_ty *node, int depth, int direction)
{
    int i = 0;
    
    if(NULL == node)
    {
        return;
    }

    for(i = 0; i <= depth; ++i)
    {
        if (1 < (depth - i))
        {
            printf("\t");
        }
        else if (1 == (depth - i))
        {
            if (0 == direction)
            {
                printf("\tL|");
            }
            else
            {
                printf("\tR|");
            }
        }
        else if (0 == (depth - i))
        {
            printf("%d", *(int *)node->data);
        }
    }
    printf("\n");

    RecursivePrint(node->children[0], depth + 1, 0);
    RecursivePrint(node->children[1], depth + 1, 1);
}
