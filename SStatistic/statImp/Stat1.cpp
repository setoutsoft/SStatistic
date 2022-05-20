#include "Stat1.h"

namespace SOUI{

	Stat1::Stat1(void):StatBase("stat1")
{
}

Stat1::~Stat1(void)
{
}

//you can update Test_AB by set keys of Test_A and Test_B seperately
bool Stat1::setValue(int key, uint32_t value)
{
	bool bRet = StatBase::setValue(key,value);
	if(!bRet) return false;
	if(key == Test_A || key == Test_B)
	{
		uint32_t a,b = 0;
		getValue(Test_A,a,0);
		getValue(Test_B,b,0);
		uint32_t ab = a*b;
		StatBase::setValue(Test_AB,ab);
	}
	return true;
}

}
