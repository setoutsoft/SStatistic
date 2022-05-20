#include "StatBase.h"

namespace SOUI
{
	class Stat2Json : public IStat2Stream
	{
	public:
		bool isStreamEmpty(const STATSTREAM & stm) const;

		virtual void OnStatBegin(STATSTREAM & stm);

		virtual void OnStatEnd(STATSTREAM & stm);

		virtual void OnStatKeyValue(STATSTREAM &desc, const std::string &key, const Variant & value,int iKey);

	};
}