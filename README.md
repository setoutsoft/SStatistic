# SStatistic
一个统计数据收集模块
使用方法：
```
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
```

Stat1, Stat2为两个统计模块。
