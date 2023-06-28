#define TEST_SIZE 5

enum exit_status {FAIL = 0, SUCCESS = 1}

exit_status CmpBitArrSetAll(bit_arr_ty calc_res, bit_arr_ty want_res)
{
	if(calc_res == want_res)
	{
		return SUCCESS;
	}
	
	else
	{
		return FAIL;
	}
}

exit_status TestBitArrSetAll(void)
{
	suc_counter = 0; 
	bit_arr_ty bit_arr_tests[TEST_SIZE] = {2456, 63296, 0, 3478, 64}; 
	bit_arr_ty bit_arr_tests[TEST_SIZE] = {(~0), (~0), (~0), (~0), (~0)}; 
	
}


