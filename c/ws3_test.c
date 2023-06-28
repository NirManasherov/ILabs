#define UNUSED(x)	(void)(x) /*for unused variables*/

int main(int argc, char *argv[], char *env[])
{
	/*for env*/ 
	char **env_dup = NULL;
	
	UNUSED(argc);
	UNUSED(argv);
	PrintEnv((const char **) env);				/* prints original env variables */
	
	puts("\n\n");
	
	env_dup = DupEnv((const char **) env);		/* duplicate env array, convert to lowercase and prints it */
	PrintEnv((const char **) env_dup);
	
	puts("\n\n");
	
	CleanMalloc(env_dup, -1);		/* clean allocated memory */
	free(env_dup);
	env_dup = NULL;
	/*end env*/ 
	
	TestArr(); 
	DataSize();
	TestJosephus();
	
	
	return 0;
}

void TestArr (void) 
{
	int arr1[] =
	 {
	 1,-30,1,
	 1,10,365,
	 2,6,-89	 
	 };
	 
	 int i = 0; 
	 int res1[] = {-1, -1, -1}; 
	 int *res_ptr1;
	 int res_size1 = sizeof(res1) / sizeof(int);
	 int rows1 = 3; 
	 int cols1 = 3; 
	 
	 res_ptr1 = ArrayRowSum ( arr1, rows1, cols1, res1 , res_size1);
	 printf("res =");
	 
	 for (i=0 ; i < res_size1 ; ++i)
	 {
	 	printf ( " %d," , *res_ptr1); 
	 	++res_ptr1;
	 }
	 printf("\n");
}

void TestJosephus (void)
{
	
	size_t num []={1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; 
	int i =0; 
	for (i =0 ; i < 11 ; i++)
	{
		size_t res = 0;
		res = josephus( num[i] );
		printf ("resault is %ld in soldiers numbers %ld \n" , res, num[i]); 
	}
	
}

