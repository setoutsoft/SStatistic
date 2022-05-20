#pragma once

#include <StatBase.h>

namespace SOUI
{
	class Stat2 : public StatBase
	{
	public:
		Stat2(void);
		~Stat2(void);

#define IKEY
#include <statkey_def.h>
		BEGIN_KEY(2000)
			KEY(Url_Str, "url", String, "soime.cn")    
			KEY(Test_I32, "test_i32", I32, 0)    
			KEY(Test_Float, "test_float", Float, 0.0f)
			KEY(Test_Bool, "test_bool", Bool, true) 
		END_KEY()

#undef IKEY
#include <statkey_def.h>
		BEGIN_KEY(2000)
		KEY(Url_Str, "url", String, "soime.cn")    
		KEY(Test_I32, "test_i32", I32, 0)    
		KEY(Test_Float, "test_float", Float, 0.0f)
		KEY(Test_Bool, "test_bool", Bool, true) 
		END_KEY()
	};

}
