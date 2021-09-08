#pragma once
#include "Global.h"

namespace dbg
{
	/*
	/// <summary>
	/// Debug function for measuring the execution time of a function
	/// </summary>
	/// <param name="function"> The function wichs time should be measured </param>
	/// <param name="paramCount"> The number of parameters the function takes </param>
	/// <param name=""> The parameters that are supposed to be passed to the function </param>
	template<typename ... T>void GetExecutionTime(void* function, T ... parms)
	{

		auto t1 = std::chrono::high_resolution_clock::now();


		function(parms...);


		auto t2 = std::chrono::high_resolution_clock::now();

		auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

		std::chrono::duration<double, std::milli> ms_double = t2 - t1;
	}

	void TestDD(int ind, std::string strr)
	{
		std::cout << ind << "\n" << strr << "\n";

	}*/
}