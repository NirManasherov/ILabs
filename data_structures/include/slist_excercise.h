#ifndef __SLIST_EXERCISE_H__
#define __SLIST_EXERCISE_H__ 

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

/*  Reverses the order of a given slist                                       */
node_t *Flip(node_t *head);

/*  Tells whether a given slist has loop                                      */
int HasLoop(const node_t *head);

/*  Returns a pointer to a first node mutual to both slists, if any           */
node_t *FindIntersection(node_t *head_1, node_t *head_2);

#endif /*__SLIST_EXERCISE_H__*/
