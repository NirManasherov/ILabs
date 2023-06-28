#ifndef __SIMPLE_STRING__
#define __SIMPLE_STRING__

#include <cstddef>  /*size_t*/

#include "utils.h" /*match_func, action_func*/

namespace irld
{

class Slist
{
public:

    class Iter; 
    explicit Slist(); 
    //explicit Slist(Slist o_) = delete; 
    //String& operator=(const String& o_) = delete; 
    ~Slist(); 
    
    const Iter Insert(Iter where,  void *data);
    const Iter Remove(Iter iter_to_remove);
    size_t Count() const;
    const Iter IterBegin();
    const Iter IterEnd();
    void Append(slist_ty *src_slist);
        
private: 
    Slist& operator=(const Slist& o_); //do not impliment
    explicit Slist(Slist o_) = delete; //do not impliment
    Node *front;
}

class Slist::Iter
{
//generate copy and assignment
public: 
    const Iter Next();
    bool IsEqual(Iter iterator) const;
    void SetData(const void *new_data) const;
    const void *IterGetData() const;
    
private: 
    class Node; 
    Node *IterToNode(); 
    friend class Slist; 
} 

class Slist::Node
{
public:
    explicit Node(Node *next, void *data); 
private 
    void *data; 
    Node *next;
    
}

}//end namespace

const Iter Find(const Iter from, const Iter to, 
                                      is_match_func_ty match_func, void &param);
                                      
int ForEach(Iter from, Iter to, action_func_ty action, void *param);

#endif //__SIMPLE_STRING__
