#include <stdio.h>  /*printf, puts*/
#include <stdlib.h> /*malloc, free*/
/*******************************************************************************
                                 enum
*******************************************************************************/

enum {SUCCESS = 0, FAIL = 1}; /*ONLY FOR THIS FILE! DO NOT INCLUDE THIS FILE!!*/

/*******************************************************************************
                                 Typedef
*******************************************************************************/

typedef void (*func_ptr_1_arg)(void *); 

typedef void (*func_ptr_1_int_arg)(int); 

typedef void (*func_ptr_2_arg)(void *, void *);

typedef int (*int_func_ptr_2_arg)(void *);

typedef struct PublicTransport PublicTransport_ty;  

typedef struct Minibus Minibus_ty;

typedef struct PublicTransport Taxi_ty;

typedef Taxi_ty SpecialTaxi_ty;

/*******************************************************************************
                   Func decleration for compilation
*******************************************************************************/

void PublicTransport_PublicTransport_Dtor(PublicTransport_ty *const this_);

void PublicTransport_display(PublicTransport_ty *const this_); 

void Minibus_Minibus_Dtor(Minibus_ty *const this_); 
 
void Minibus_display(Minibus_ty *const this_); 

void Minibus_wash(int minutes, Minibus_ty *const this_); 

void Taxi_Taxi_Dtor(Taxi_ty *const this_); 

void Taxi_display(Taxi_ty *const this_); 

void SpecialTaxi_SpecialTaxi_Dtor(SpecialTaxi_ty *const this_);

void SpecialTaxi_display(SpecialTaxi_ty *const this_);

/*******************************************************************************
                               Vtable def
*******************************************************************************/
typedef struct PublicTransport_Vtable PublicTransport_Vtable_ty;

struct PublicTransport_Vtable 
{
    void (*Dtor)(PublicTransport_ty *const);
    void (*display)(PublicTransport_ty *const);
}   g_PublicTransport_Vtable = 
    {
        PublicTransport_PublicTransport_Dtor, 
        PublicTransport_display
    };

typedef struct Minibus_Vtable Minibus_Vtable_ty; 

struct Minibus_Vtable 
{
    void (*Dtor)(Minibus_ty *const);
    void (*display)(Minibus_ty *const);
    void (*wash)(int , Minibus_ty *const);
}   g_Minibus_Vtable = 
    {
        Minibus_Minibus_Dtor, 
        Minibus_display, 
        Minibus_wash
    };

typedef struct Taxi_Vtable Taxi_Vtable_ty;

struct Taxi_Vtable 
{
    void (*Dtor)(Taxi_ty *const);
    void (*display)(Taxi_ty *const);
}   g_Taxi_Vtable = 
    {
        Taxi_Taxi_Dtor, 
        Taxi_display
    };
    
typedef struct SpecialTaxi_Vtable SpecialTaxi_Vtable_ty;

struct SpecialTaxi_Vtable 
{
    void (*Dtor)(SpecialTaxi_ty *const);
    void (*display)(SpecialTaxi_ty *const);
}   g_SpecialTaxi_Vtable = 
    {
        SpecialTaxi_SpecialTaxi_Dtor, 
        SpecialTaxi_display
    };

/******************************************************************************/

/*******************************************************************************
                                 Global
*******************************************************************************/

int PublicTransport_s_count = 0;

/******************************************************************************/

/*******************************************************************************
                               PublicTransport
*******************************************************************************/

struct PublicTransport
{
    PublicTransport_Vtable_ty *Vtable; 
    int PublicTransport_m_license_plate;
};


/***********************************public*************************************/

void PublicTransport_PublicTransport_Ctor(PublicTransport_ty *const this_)
{
    this_->Vtable = &g_PublicTransport_Vtable;
    /* this is the first member PublicTransport_Vtable*(void **)this_ = PublicTransport_Vtable */
    
    this_->PublicTransport_m_license_plate = ++PublicTransport_s_count; 
    
    printf("PublicTransport::Ctor() %d\n", this_->PublicTransport_m_license_plate); 
}

void PublicTransport_PublicTransport_Dtor(PublicTransport_ty *const this_)
{
    --PublicTransport_s_count;
    
    printf("PublicTransport::Dtor() %d\n", this_->PublicTransport_m_license_plate);
}

void PublicTransport_PublicTransport_CCtor(const PublicTransport_ty *other, 
                                                PublicTransport_ty *const this_)
{
    /* if other is not PublicTransport */
    this_->Vtable = &g_PublicTransport_Vtable;
    
    this_->PublicTransport_m_license_plate = ++PublicTransport_s_count; 
    
    printf("PublicTransport::CCtor() %d\n", this_->PublicTransport_m_license_plate);
} 

void PublicTransport_display(PublicTransport_ty *const this_)
{
    printf("PublicTransport::Display() %d\n", this_->PublicTransport_m_license_plate);
}

void PublicTransport_print_count(void)
{
        printf("s_count: %d\n", PublicTransport_s_count);
}

/*********************************protected************************************/

int PublicTransport_get_ID(PublicTransport_ty *const this_)
{
    return this_->PublicTransport_m_license_plate;
}

/**********************************private*************************************/

/* disabled - no need to write (this is for myself) */
/*PublicTransport_ty *PublicTransport_operator_equal(const PublicTransport_ty *o_, 
                                               PublicTransport_ty *const this_);*/

/******************************************************************************/

/*******************************************************************************
                               Minibus
*******************************************************************************/
  
struct Minibus
{
    PublicTransport_ty PublicTransport; /*Minibus inherent PublicTransport*/
    int Minibus_m_numSeats;
};

/******************************************************************************/
/***********************************public*************************************/

void Minibus_Minibus_Ctor(Minibus_ty *const this_)
{
    PublicTransport_PublicTransport_Ctor((PublicTransport_ty *const)this_); 
    
    this_->PublicTransport.Vtable = ((PublicTransport_Vtable_ty *)&g_Minibus_Vtable);
    
    this_->Minibus_m_numSeats = 20; 
    
    puts("Minibus::Ctor()"); 
}

void Minibus_Minibus_CCtor(const Minibus_ty *other, Minibus_ty *const this_)
{    
    PublicTransport_PublicTransport_CCtor((PublicTransport_ty *const)other,
                                              (PublicTransport_ty *const)this_); 
    
    /* if other is not Minibus */
    this_->PublicTransport.Vtable = ((PublicTransport_Vtable_ty *)&g_Minibus_Vtable); 
    
    this_->Minibus_m_numSeats = other->Minibus_m_numSeats;
    
    puts("Minibus::CCtor()\n"); 
}

void Minibus_Minibus_Dtor(Minibus_ty *const this_)
{
    puts("Minibus::Dtor()\n"); 
    
    /* if the base Dtor will activate virtual member functions */
    this_->PublicTransport.Vtable = &g_PublicTransport_Vtable;
    
    PublicTransport_PublicTransport_Dtor((PublicTransport_ty *)this_);
}

void Minibus_display(Minibus_ty *const this_)
{
    printf("Minibus::display() ID: %d", 
                             PublicTransport_get_ID(&(this_->PublicTransport))); 
                             
    printf(" num seats: %d\n", this_->Minibus_m_numSeats);
}

void Minibus_wash(int minutes, Minibus_ty *const this_)
{
    printf("Minibus::wash(%d) ID: %d\n", minutes,
                             PublicTransport_get_ID(&(this_->PublicTransport)));
}

/*******************************************************************************
                                     Taxi
*******************************************************************************/
/***********************************public*************************************/

void Taxi_Taxi_Ctor(Taxi_ty *const this_)
{
    PublicTransport_PublicTransport_Ctor((PublicTransport_ty *const)this_); 
    
    this_->Vtable = ((PublicTransport_Vtable_ty *)&g_Taxi_Vtable);
        
    puts("Taxi::Ctor()\n"); 
}

void Taxi_Taxi_CCtor(const Taxi_ty *other, Taxi_ty *const this_)
{    
    PublicTransport_PublicTransport_CCtor((PublicTransport_ty *const)other, 
                                              (PublicTransport_ty *const)this_); 
    
    /* if other is not Taxi */
    this_->Vtable = ((PublicTransport_Vtable_ty *)&g_Taxi_Vtable); 
    
    puts("Minibus::CCtor()\n"); 
}

void Taxi_Taxi_Dtor(Taxi_ty *const this_)
{
    puts("Taxi::Dtor()\n"); 
    
    /* if the base Dtor will activate virtual member functions */
    this_->Vtable = &g_PublicTransport_Vtable;
    
    PublicTransport_PublicTransport_Dtor((PublicTransport_ty *)this_);
}

void Taxi_display(Taxi_ty *const this_)
{
    puts("Taxi::Dtor()\n"); 
}

/******************************************************************************/

/*******************************************************************************
                                Special Taxi
*******************************************************************************/
/***********************************public*************************************/

void SpecialTaxi_SpecialTaxi_Ctor(SpecialTaxi_ty *const this_)
{
    Taxi_Taxi_Ctor((Taxi_ty *const)this_); 
    
    this_->Vtable = ((PublicTransport_Vtable_ty *)&g_SpecialTaxi_Vtable);
        
    puts("SpecialTaxi::Ctor()\n"); 
}

void SpecialTaxi_SpecialTaxi_CCtor(const SpecialTaxi_ty *other, SpecialTaxi_ty *const this_)
{    
    Taxi_Taxi_CCtor((Taxi_ty *const)other, 
                                              (Taxi_ty *const)this_); 
    
    /* if other is not SpecialTaxi */
    this_->Vtable = ((PublicTransport_Vtable_ty *)&g_SpecialTaxi_Vtable); 
    
    puts("Minibus::CCtor()\n"); 
}

void SpecialTaxi_SpecialTaxi_Dtor(SpecialTaxi_ty *const this_)
{
    puts("SpecialTaxi::Dtor()\n"); 
    
    /* if the base Dtor will activate virtual member functions */
    this_->Vtable = (PublicTransport_Vtable_ty *)&g_Taxi_Vtable;
    
    Taxi_Taxi_Dtor((Taxi_ty *)this_);
}

void SpecialTaxi_display(SpecialTaxi_ty *const this_)
{
    puts("SpecialTaxi::Dtor()\n"); 
}

/******************************************************************************/

/*******************************************************************************
                            global Functions
*******************************************************************************/

void print_info_PT(PublicTransport_ty *a)
{
    a->Vtable->display(a);
}

void print_info_V(void)
{
    PublicTransport_print_count();
}

void print_info_MB(Minibus_ty *m)
{
    ((Minibus_Vtable_ty *)(m->PublicTransport.Vtable))->wash(3, m);
}

void print_info_I(int i, PublicTransport_ty *ret_val)
{
    Minibus_ty ret;
    Minibus_Minibus_Ctor(&ret);
    
    puts("print_info(int i)\n");
    
    /* minibus define in this scope - we know what func to use */
    Minibus_display(&ret);
    
    PublicTransport_PublicTransport_CCtor((PublicTransport_ty *)&ret, ret_val);
        
    /* minibus define in this scope - we know what func to use */
    Minibus_Minibus_Dtor(&ret);
}

void taxi_display(Taxi_ty *s)
{
    /* getting by value = we know type */
    Taxi_display(s);
    
    /* the arg by value is this func variable -> it is in its responsibility */
    Taxi_Taxi_Dtor(s); 
}

inline int max_func(const int *t1, const int *t2)
{
    return ((*t1 > *t2) ? *t1 : *t2);
}

/******************************************************************************/

int main(void)
{
    Minibus_ty m;
    Minibus_ty ret_print_info;
    PublicTransport_ty *array[3];  
    Taxi_ty arr2[3]; 
    size_t i; 
    Minibus_ty tmp_mini; 
    Taxi_ty tmp_taxi; 
    Minibus_ty m2;
    Minibus_ty arr3[4]; 
    size_t *arr4_metadata;
    Taxi_ty arr4[4]; 
    SpecialTaxi_ty st; 
    Taxi_ty tmp_taxi2;
    
    Minibus_Minibus_Ctor((Minibus_ty * const)&m);
    print_info_MB(&m); 
    
    print_info_I(3, (PublicTransport_ty *)&ret_print_info);
    /*the upward func returns public transport (slicing)*/ 
    PublicTransport_display((PublicTransport_ty *const)&ret_print_info);
    Minibus_Minibus_Dtor((Minibus_ty * const)&ret_print_info); 
    
    array[0] = (PublicTransport_ty *)malloc(sizeof(Minibus_ty));
    Minibus_Minibus_Ctor((Minibus_ty *const)&array[0]);
    array[0] = (PublicTransport_ty *)array[0]; 
    
    array[1] = (PublicTransport_ty *)malloc(sizeof(Taxi_ty));
    Taxi_Taxi_Ctor((Taxi_ty *const)array[1]);
    array[1] = (PublicTransport_ty *)array[1]; 
    
    array[2] = (PublicTransport_ty *)malloc(sizeof(Minibus_ty));
    Minibus_Minibus_Ctor((Minibus_ty *const)&array[2]);
    array[2] = (PublicTransport_ty *)array[2];
    
    
    i=0;
    for(i=0 ; i<3 ; ++i) 
    {
        array[i]->Vtable->display(array[i]);
    }   
    
    i=0;
    for(i=0 ; i<3 ; ++i) 
    {
        array[i]->Vtable->Dtor(array[i]);
        free(array+i); 
    }
    
    Minibus_Minibus_Ctor(&tmp_mini);
    PublicTransport_PublicTransport_CCtor((const PublicTransport_ty *)arr2, 
                                          (PublicTransport_ty *const)&tmp_mini);
    Minibus_Minibus_Dtor((Minibus_ty *const)&tmp_mini);
    
    Taxi_Taxi_Ctor(&tmp_taxi);
    PublicTransport_PublicTransport_CCtor((const PublicTransport_ty *)arr2 + 1, 
                                          (PublicTransport_ty *const)&tmp_taxi);
    Taxi_Taxi_Dtor(&tmp_taxi);
    
    PublicTransport_PublicTransport_Ctor(arr2+2);
    
    i=0;
    for(i=0 ; i<3 ; ++i) 
    {
        (arr2+i)->Vtable->display(arr2+i);
    }
        
    PublicTransport_print_count();
    
    Minibus_Minibus_Ctor(&m2);
    
    PublicTransport_print_count();
    
    Minibus_Minibus_Ctor(arr3);
    Minibus_Minibus_Ctor(arr3+1);
    Minibus_Minibus_Ctor(arr3+2);
    Minibus_Minibus_Ctor(arr3+3);
    
    /*meta data of new and delete*/
    arr4_metadata = (size_t *)malloc(sizeof(Taxi_ty)*4 + sizeof(size_t));
    *arr4_metadata = 4;
    
    arr4 = (Taxi_ty)(arr4_metadata + 1); 
    
    i=0;
    for(i=0 ; i<4 ; ++i) 
    {
        Taxi_Taxi_Ctor((Taxi_ty)arr4 + i); 
    }
    
    i=0;
    for(i=*((size_t *)arr4 - 1)-1 ; i>=0 ; --i) 
    {
        (arr4+i)->Vtable->display(arr4+i);
    }
    
    printf("%d\n", max_func(1,2));
    printf("%d\n", max_func(1,2)); /*the user wanted int and the compiler can       
                                                             transform 2.0f->2*/
    
    SpecialTaxi_SpecialTaxi_Ctor(&st);
    Taxi_Taxi_CCtor(&st, tmp_taxi2);
    Taxi_display(&tmp_taxi2); 
    SpecialTaxi_SpecialTaxi_Dtor(tmp_taxi2); 
    
    Minibus_Minibus_Dtor(arr3+3);
    Minibus_Minibus_Dtor(arr3+2);
    Minibus_Minibus_Dtor(arr3+1);
    Minibus_Minibus_Dtor(arr3);
    Minibus_Minibus_Dtor(&m2);
    PublicTransport_PublicTransport_Dtor(arr2+2);
    PublicTransport_PublicTransport_Dtor(arr2+1);
    PublicTransport_PublicTransport_Dtor(arr2);
    Minibus_Minibus_Dtor(&m);
    
    return SUCCESS;
}
