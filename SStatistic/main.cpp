#include "statImp/StatMgr.h"
using namespace SOUI;
void main()
{
	StatMgr statMgr;

	statMgr.setValue(Stat1::DnsTime_UI32,5000U);
	statMgr.setValue(Stat1::Test_A,10U);
	statMgr.setValue(Stat1::Test_B,20U);

	statMgr.setValue(Stat2::Url_Str,"www.yy.com");
	statMgr.setValue(Stat2::Test_Bool,false);
	statMgr.setValue(Stat2::Test_Float,5.5f);

	STATSTREAM stm;
	statMgr.getDesc(stm,-1,0);

	std::string str = stm.str();
	printf("desc=%s\n",str.c_str());
}