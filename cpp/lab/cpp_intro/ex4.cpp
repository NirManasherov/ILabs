typedef struct Person 
{
    int age; 
    char *name; 
    int id_num; 
}Person_t; 

typedef struct List 
{
    Person_t person;
}List_t; 

void *ll_find(List_t *ll, int key);

void foo(List_t *ll, int key, int age)
{
    void *vv = ll_find(ll, key); 
    
    /*Person_t *p = (Person_t *)ll;*/
    Person_t *p = static_cast<Person_t *>(vv); 
    p->age = age; 
}
