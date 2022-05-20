#include "StatMgr.h"
#include "Stat1.h"

namespace SOUI{

StatMgr::StatMgr(void)
{
	m_stats.push_back(new Stat1);
	m_stats.push_back(new Stat2);
	for(StatList::iterator it = m_stats.begin();it!= m_stats.end();it++)
	{
		(*it)->init(this);
	}
}

StatMgr::~StatMgr(void)
{
	for(StatList::iterator it = m_stats.begin();it!= m_stats.end();it++)
	{
		delete *it;
	}
	m_stats.clear();
}

StatBase * StatMgr::getStatById(int statId)
{
	for(StatList::iterator it = m_stats.begin();it!= m_stats.end();it++)
	{
		if((*it)->getId() == statId)
			return *it;
	}
	return NULL;
}

bool StatMgr::setValue(int key, const Variant & value)
{
	for(StatList::iterator it = m_stats.begin();it!= m_stats.end();it++)
	{
		if((*it)->setValue(key,value))
			return true;
	}
	return false;
}

bool StatMgr::updateValue(int key, const Variant & value)
{
	for(StatList::iterator it = m_stats.begin();it!= m_stats.end();it++)
	{
		if((*it)->updateValue(key,value))
			return true;
	}
	return false;

}

bool StatMgr::getValue(int statId, int key, Variant &value)
{
	StatBase *pStatBase = getStatById(statId);
	if (!pStatBase)
		return false;
	return pStatBase->getValue(key, value, 0);
}

void StatMgr::getDesc(STATSTREAM &desc, uint32_t group, uint32_t param)
{
	desc << "{";

	for(StatList::iterator it = m_stats.begin();it!= m_stats.end();)
	{
		desc<<"\""<<(*it)->getName()<<"\""<<":";
		(*it)->getDesc(desc,group,param);
		it++;
		if(it != m_stats.end())
		{
			desc<<",\n";
		}
	}
	desc << "}";
}

}
