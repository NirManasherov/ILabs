#include <stdio.h>	/*for puts*/
#include <string.h> /*for strcmp*/
#include "slist.h"

#define UNUSED(x) (void)(x)

typedef struct person 
{
	char *name;
	double weight;  
	int age; 
}person_ty; 

static int IsPersonRacoon(const void *list_data, void *param);
static int HappyBirthday(void *list_data, void *param);

int main(void)
{
	size_t node_count = 0;	 
	const void *nir_ptr = NULL; 
	const void *sarit_ptr = NULL; 
	const void *lital_ptr = NULL; 
	const void *imposter_racoon_ptr = NULL; 
	int res = 0; 
	is_match_func_ty find_func = NULL;
	action_func_ty action_func = NULL;
	person_ty nir;
	person_ty sarit; 
	person_ty lital;
	person_ty imposter_racoon;
	slist_ty *my_list = NULL;
	iter_ty my_iter; 
	iter_ty my_second_iter; 
	
	find_func = IsPersonRacoon; 
	action_func = HappyBirthday;
	/*initializing nir*/ 
	nir.name = "Nir";
	nir.weight = 78.2; 
	nir.age = 26; 
	/*initializing sarit*/ 
	sarit.name = "Sarit";
	sarit.weight = 52.0; 
	sarit.age = 18; 
	/*initializing lital*/ 
	lital.name = "Lital";
	lital.weight = 55.1; 
	lital.age = 28; 
	/*initializing imposter racoon*/ 
	imposter_racoon.name = "Real Human - not a rAcO0N";
	imposter_racoon.weight = 4.2; 
	imposter_racoon.age = 5; 
	/*initializing structs pointer*/
	nir_ptr = (const void *)&nir;
	sarit_ptr = (const void *)&sarit;
	lital_ptr = (const void *)&lital;
	imposter_racoon_ptr = (const void *)&imposter_racoon; 
	
	my_list = SlistCreate(); 
	if(NULL == my_list)
	{
		puts("Error! fail to create list!");
	}
	
	node_count = SlistCount(my_list);
	
	if(0 != node_count)
	{
		puts("Error! counting empty list incorrectly!");
	}
	
	my_iter = SlistIterBegin(my_list);/*my iter in the first member*/
	/*adding members in reverse order (lital)->(sarit)->(nir)->(dummy) and check
	                                    /\
	                                    | 
	                                 my_iter
	every insert if it was inserted correctly by pointers*/
	my_iter = SlistInsert(my_iter, nir_ptr);
	if(nir_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in entering nir to the list"); 
	}
	
	
	my_iter = SlistInsert(my_iter, sarit_ptr);
	
	if(sarit_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in entering sarit to the list"); 
	}
	
	my_iter = SlistInsert(my_iter, lital_ptr);
	
	if(lital_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in entering lital to the list"); 
	}
	
	node_count = SlistCount(my_list);
	
	if(3 != node_count)
	{
		puts("Error! counting incorrectly!");
	}
	
	/*(lital)->(sarit)->(nir)->(dummy)
	             /\
	             | 
	          my_iter */
	my_iter = SlistIterNext(my_iter);
	
	if(sarit_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in moving to sarit in the list with iter-next"); 
	}
	
	/*(lital)->(nir)->(dummy)
	             /\
	             | 
	         my_iter */
	my_iter = SlistRemove(my_iter);
	
	if(nir_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in pointing  to nir after remove sarit"); 
	}
	
	/*(lital)->(nir)->(dummy)
	                    /\
	                    | 
	                 my_iter */
	my_iter = SlistIterEnd(my_list); 
	
	/*(lital)->(nir)->(sarit)->(dummy)
	                    /\
	                    | 
	                 my_iter */
	my_iter = SlistInsert(my_iter, sarit_ptr);
	
	if(sarit_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in entering sarit to the list second time"); 
	}
	
	/*(lital)->(nir)->(sarit)->(dummy)
	    /\
	    | 
	  my_iter */
	my_iter = SlistIterBegin(my_list); 
	
	/*(imposter_racoon)->(nir)->(sarit)->(dummy)
	         /\
	         |
	      my_iter  */
	SlistIterSetData(my_iter, imposter_racoon_ptr);
	
	if(imposter_racoon_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in replacing (murderring) lital with imposter racoon"); 
	}
	/*(imposter_racoon)->(nir)->(sarit)->(dummy)
	         /\                             /\
	         |                              | 
	      my_iter                     my_second_iter*/
	my_second_iter = SlistIterEnd(my_list);
	
	if(1 == (SlistIterIsEqual(my_iter, my_second_iter)))/*1=true*/
	{
		puts("Error! two different iterators are found as the same");
	}
	
		/*(imposter_racoon)->(nir)->(sarit)->(dummy)
	         /\                            
	         |                              
 my_second_iter,my_iter                     */
	my_second_iter = SlistIterBegin(my_list);
	
	if(0 == (SlistIterIsEqual(my_iter, my_second_iter)))/*0 = false*/
	{
		puts("Error! two same iterators are found as the different");
	}
	
	/*(imposter_racoon)->(nir)->(sarit)->(dummy)
	         /\           /\                
	         |            |                  
     my_second_iter     my_iter               */
	my_iter = SlistIterNext(my_iter);
	
	/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	         /\           /\                
	         |            |                  
     my_second_iter     my_iter               */
	my_iter = SlistInsert(my_iter, lital_ptr);
	
	if(lital_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error in entering lital to the list"); 
	}
	
		/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	         /\                /\                
	         |                 |                  
     my_second_iter        my_iter               */
	my_second_iter = SlistIterBegin(my_list);
	
	if(imposter_racoon_ptr != (person_ty *)SlistIterGetData(my_second_iter))
	{
		puts("Error finding racoon as start"); 
	}
	
	/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	                        /\                
	                        |                  
              my_second_iter, my_iter               */
	my_second_iter = SlistIterNext(my_second_iter);
	
	if(lital_ptr != (person_ty *)SlistIterGetData(my_iter))
	{
		puts("Error scanning lital"); 
	}
	
		/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	                           /\       /\             
	                           |        |          
                            my_iter   my_second_iter            */
	my_second_iter = SlistIterNext(my_second_iter);
	
	if(nir_ptr != (person_ty *)SlistIterGetData(my_second_iter))
	{
		puts("Error scanning nir"); 
	}
	
	/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	                       /\               /\             
	                       |                |          
                         my_iter     my_second_iter            */
	my_second_iter = SlistIterNext(my_second_iter);
	
	if(sarit_ptr != (person_ty *)SlistIterGetData(my_second_iter))
	{
		puts("Error scanning sarit"); 
	}
	
	/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	         /\             /\                
	         |              |                  
     my_second_iter      my_iter               */
	my_second_iter = SlistFind(SlistIterBegin(my_list), SlistIterEnd(my_list), find_func, NULL);
	
	if(imposter_racoon_ptr != (person_ty *)SlistIterGetData(my_second_iter))
	{
		puts("Error detecting racoon!"); 
	}
	
	res = SlistForEach(SlistIterBegin(my_list), SlistIterEnd(my_list), action_func, NULL);
	
	if(0 != res)
	{
		puts("fail aging!");
	}
	
	if(6 != (((person_ty *)(SlistIterGetData(my_second_iter)))->age))
	{
		puts("Error aging racoon!"); 
	}
	
	
	/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	                        /\                
	                        |                  
                my_second_iter, my_iter               */
	my_second_iter = SlistIterNext(my_second_iter); 
	
	if(29 != (((person_ty *)(SlistIterGetData(my_second_iter)))->age))
	{
		puts("Error aging lital!"); 
	}
	
	/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	                        /\      /\              
	                        |       |                 
                         my_iter my_second_iter            */
	my_second_iter = SlistIterNext(my_second_iter); 
	
	if(27 != (((person_ty *)(SlistIterGetData(my_second_iter)))->age))
	{
		puts("Error aging nir!"); 
	}
	
	/*(imposter_racoon)->(lital)->(nir)->(sarit)->(dummy)
	                        /\             /\              
	                        |              |                 
                         my_iter      my_second_iter            */
	my_second_iter = SlistIterNext(my_second_iter); 
	
	if(19 != (((person_ty *)(SlistIterGetData(my_second_iter)))->age))
	{
		puts("Error aging sarit!"); 
	}
	
	SlistDestroy(my_list);
	return 0; 
	
}

static int IsPersonRacoon(const void *list_data, void *param)
{
	person_ty *data_ptr = NULL; 
	UNUSED(param); 
	data_ptr = (person_ty *)list_data; 
	if(0 == strcmp(data_ptr->name, "Real Human - not a rAcO0N")) 
	{
		return 1; /*1=true*/
	}
	return 0; /*0=false*/
}

static int HappyBirthday(void *list_data, void *param)
{
	UNUSED(param); 
	(((person_ty *)list_data)->age) += 1;
	return 0;
}

