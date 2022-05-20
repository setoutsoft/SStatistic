#include "Stat2Json.h"
#include <assert.h>

namespace SOUI{
	static Stat2Json s_defStat2Stm;
	IStat2Stream * StatBase::s_stat2Stream = &s_defStat2Stm;

void Stat2Json::OnStatKeyValue(STATSTREAM &desc, const std::string &key, const Variant & value,int iKey)
{
	if(iKey!=0)
		desc <<",\n";
	desc << "\"" << key << "\":";
	switch (value.m_type)
	{
	case I32:
		desc << value.v_i32;
		break;
	case UI32:
		desc << value.v_ui32;
		break;
	case I64:
		desc << value.v_i64;
		break;
	case Float:
		desc << value.v_float;
		break;
	case String:
		desc << "\""<<(value.v_str) << "\"";
		break;
	case Bool:
		desc << (value.v_bool?"true":"false");
		break;
	case None:
		assert(false);
		break;
	}
}

void Stat2Json::OnStatBegin(STATSTREAM & stm)
{
	stm<<"{";
}

void Stat2Json::OnStatEnd(STATSTREAM & stm)
{
	stm<<"}";
}


bool Stat2Json::isStreamEmpty(const STATSTREAM & stm) const
{
	return stm.str().empty();
}

}