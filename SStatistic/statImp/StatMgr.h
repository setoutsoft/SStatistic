#pragma once
#include <list>
#include "Stat1.h"
#include "Stat2.h"

namespace SOUI{

class StatMgr :public IStatQuery
{
public:
	StatMgr(void);
	~StatMgr(void);

public:
	void getDesc(STATSTREAM &desc, uint32_t group, uint32_t param);

	bool setValue(int key, const Variant & value);

	bool getValue(int statId, int key, Variant &value);

	bool updateValue(int key, const Variant & value);

private:
	StatBase * getStatById(int statId) override;

private:
	typedef std::list<StatBase *> StatList;
	StatList m_stats;
};

}
