#pragma once

#include <StatBase.h>

namespace SOUI
{
	class Stat1 : public StatBase
	{
	public:
		Stat1(void);
		~Stat1(void);

#define IKEY
#include <statkey_def.h>
		BEGIN_KEY(1000)
			KEY(DnsTime_UI32, "dns", UI32, 0)    //DNS parse time
			KEY(Test_A, NULL, UI32, 0)    //test a
			KEY(Test_B, NULL, UI32, 0)    //test b
			KEY(Test_AB, "amb", UI32, 0)    //test ab = a*b
		END_KEY()

#undef IKEY
#include <statkey_def.h>
		BEGIN_KEY(1000)
		KEY(DnsTime_UI32, "dns", UI32, 0)    //DNS parse time
		KEY(Test_A, NULL, UI32, 0)    //test a
		KEY(Test_B, NULL, UI32, 0)    //test b
		KEY(Test_AB, "amb", UI32, 0)    //test ab = a*b
		END_KEY()

	protected:
		
		virtual bool setValue(int key, uint32_t value);
	};

}
