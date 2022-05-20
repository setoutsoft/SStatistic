#include "StatBase.h"
#include <sstream>
#include <assert.h>
#include <stdint.h>

namespace SOUI{

    Variant::Variant():m_type(None){}
    Variant::Variant(int value) : v_i32(value),m_type(I32) {}
    bool Variant::Value(int& value) const {
        if(m_type!=I32)
            return false;
        value = v_i32;
        return true;
    }
    Variant::Variant(int64_t value) : v_i64(value),m_type(I64) {}
    bool Variant::Value(int64_t& value) const {
        if(m_type!=I64)
            return false;
        value = v_i64;
        return true;
    }
    Variant::Variant(uint32_t value) : v_ui32(value) ,m_type(UI32){}
    bool Variant::Value(uint32_t& value) const {
        if(m_type!=UI32)
            return false;
        value = v_ui32;
        return true;
    }
    Variant::Variant(float value) : v_float(value),m_type(Float) {}
    bool Variant::Value(float& value) const {
        if(m_type!=Float)
            return false;
        value = v_float;
        return true;
    }
    Variant::Variant(bool value) : v_bool(value),m_type(Bool) {}
    bool Variant::Value(bool& value) const {
        if(m_type!=Bool)
            return false;
        value = v_bool;
        return true;
    }

    Variant::Variant(const std::string & value) : m_type(String) ,v_str(value) {
    }

    Variant::Variant(const char* value) : m_type(String) ,v_str(value){
    }

    bool Variant::Value(std::string & value) const {
        if(m_type!=String)
            return false;
        value=v_str;;
        return true;
    }

    void Variant::operator=(const Variant &src) {

        m_type = src.m_type;
        v_str.clear();
        switch(src.m_type)
        {
            case I32: v_i32 = src.v_i32;break;
            case I64: v_i64 = src.v_i64;break;
            case UI32:v_ui32 = src.v_ui32;break;
            case Float:v_float = src.v_float;break;
            case Bool:v_bool = src.v_bool;break;
            case String:v_str = src.v_str;break;
            default:break;
        }
    }

    Variant::Variant(const Variant &src) {
        *this = src;
    }

    Variant::~Variant() {
        Clear();
    }

    void Variant::Clear() {
        m_type = None;
        v_str.clear();
    }

    bool Variant::isEmpty() const {
        return m_type==None;
    }


    //----------------------------------------------------------------------------------------
    StatBase::StatBase(const char *name):m_pStatQuery(NULL),m_pValues(NULL),m_name(name) {
    }

    StatBase::~StatBase() {
        if(m_pValues)
        {
            delete []m_pValues;
            m_pValues = NULL;
        }
    }

    void StatBase::init(IStatQuery *pStatQuery) {
        m_pStatQuery = pStatQuery;
        int nCount = 0;
        getKeyInfoMap(nCount);
        assert(nCount>0);
        m_pValues = new Variant[nCount];
#ifdef _DEBUG
        verifyKeymap();
#endif
    }

    bool StatBase::verifyKeymap() const
    {
        int nCount = 0;
        const KeyInfo* keyInfoMap = getKeyInfoMap(nCount);
        for (int i = 0; i < nCount; i++)
        {
            if(keyInfoMap[i].key != getId()+i)
            {
                //loge("key info map has wrong order! key: %d, order:%d, base:%d",keyInfoMap[i].key,i, keyInfoMap[0].key);
                return false;
            }
            if (keyInfoMap[i].realKey)
            {
                if (keyInfoMap[i].name == NULL)
                {
                    //loge("A real key has been set to %d without a name for key %d", keyInfoMap[i].realKey, keyInfoMap[i].key);
                    return false;
                }
                int key = keyInfoMap[i].realKey;
                int idx = key - keyInfoMap[0].key;
                if (keyInfoMap[idx].type != keyInfoMap[i].type)
                {
                    //loge("A real key has been set to %d but the type value is not same to origin key %d", keyInfoMap[i].realKey, keyInfoMap[i].key);
                    return false;
                }
            }
        }
        return true;
    }

    const StatBase::KeyInfo* StatBase::getKeyInfo(int key) const
    {
        int nCount = 0;
        const KeyInfo* keyInfoMap = getKeyInfoMap(nCount);
        int first, last;
        getKeyRange(first, last);
        if (key >= first && key < last)
        {
            return keyInfoMap + (key - first);
        }
        return NULL;
    }

const char* StatBase::getKeyName(int key) const {
    const KeyInfo* keyInfo = getKeyInfo(key);
    return keyInfo->name;
}

void StatBase::getKeyRange(int& first, int& last) const {
    int nCount = 0;
    const KeyInfo* kKeyMap = getKeyInfoMap(nCount);
    first = kKeyMap[0].key;
    last = kKeyMap[nCount - 1].key + 1;
}

int StatBase::getKeyIndex(int key) const
{
    int first,last;
    getKeyRange(first,last);
    if(key>=first && key < last)
        return key-first;
    return -1;
}

void StatBase::getDesc(STATSTREAM &desc, uint32_t group, uint32_t param) {
    int keyFirst = 0, keyLast = 0;
    getKeyRange(keyFirst, keyLast);

	s_stat2Stream->OnStatBegin(desc);
	int iKey = 0;
    for (int key = keyFirst; key < keyLast; key++)
    {
        const KeyInfo* info = getKeyInfo(key);
        assert(info);
        if(info->group!=0 && !(info->group & group))
            continue;
        if(info->name == NULL)
            continue;
        Variant value;
        bool bValid = getValue(key,value,param);
        if(info->reportAlways || bValid)
        {
			s_stat2Stream->OnStatKeyValue(desc,getKeyName(key),value,iKey++);
        }
    }
	s_stat2Stream->OnStatEnd(desc);
}

#define GETREALKEY(key) \
    const KeyInfo* keyInfo = getKeyInfo(key);\
    assert(keyInfo);\
    if(keyInfo->realKey!=0) \
    {\
        key = keyInfo->realKey;\
    }\

    bool StatBase::setValue(int key, const Variant & value)
    {
        const KeyInfo *keyInfo = getKeyInfo(key);
        if(!keyInfo)
            return false;
        if (keyInfo->type != value.m_type)
            return false;
        if(keyInfo->realKey!=0)
            key = keyInfo->realKey;
        bool bRet = false;
        switch(value.m_type)
        {
            case I32: bRet= setValue(key,value.v_i32);break;
            case I64:bRet= setValue(key,value.v_i64);break;
            case UI32:bRet= setValue(key,value.v_ui32);break;
            case Float:bRet= setValue(key,value.v_float);break;
            case Bool: bRet= setValue(key,value.v_bool);break;
            case String:bRet= setValue(key,value.v_str);break;
        }
        if(bRet)
            onKeyValueChanged(key);
        return bRet;
    }

    bool StatBase::setValue(int key, int64_t value) {
        int idx = getKeyIndex(key);
        if(idx==-1)
            return false;
        m_pValues[idx] = value;
        return true;
    }

    bool StatBase::setValue(int key, int value) {
        int idx = getKeyIndex(key);
        if(idx==-1)
            return false;
        m_pValues[idx] = value;
        return true;
    }

    bool StatBase::setValue(int key, uint32_t value) {
        int idx = getKeyIndex(key);
        if(idx==-1)
            return false;
        m_pValues[idx] = value;
        return true;
    }

    bool StatBase::setValue(int key, float value) {
        int idx = getKeyIndex(key);
        if(idx==-1)
            return false;
        m_pValues[idx] = value;
        return true;
    }

    bool StatBase::setValue(int key, const std::string &value) {
        int idx = getKeyIndex(key);
        if(idx==-1)
            return false;
        m_pValues[idx] = value;
        return true;
    }

    bool StatBase::setValue(int key, bool value) {
        int idx = getKeyIndex(key);
        if(idx==-1)
            return false;
        m_pValues[idx] = value;
        return true;
    }

    void StatBase::clear(uint32_t group)
    {
        int nCount=0;
        const KeyInfo * keyInfo = getKeyInfoMap(nCount);
        for (int i=0;i<nCount;i++)
        {
            if(group != -1 && keyInfo[i].group!=0 && !(keyInfo[i].group & group))
                continue;
            m_pValues[i].Clear();
        }
    }

    StatBase *StatBase::queryStat(int statId) {
        if(!m_pStatQuery)
            return NULL;
        return m_pStatQuery->getStatById(statId);
    }

    const StatBase *StatBase::queryStat(int statId) const {
        if(!m_pStatQuery)
            return NULL;
        return m_pStatQuery->getStatById(statId);
    }

    bool StatBase::updateValue(int key, const Variant &delta) {
        if(delta.m_type == String)//type of string can't be updated.
            return false;
        const StatBase::KeyInfo*  keyInfo = getKeyInfo(key);
        if (!keyInfo)
            return false;
        if (keyInfo->type != delta.m_type)
        {
            //logw("warning!!! update key type of %d with delta type of %d",keyInfo->type,delta.m_type);
            return false;
        }

        Variant value;
        getValue(key, value, 0);
        switch(value.m_type)
        {
            case I32:value.v_i32+=delta.v_i32;break;
            case I64:value.v_i64+=delta.v_i64;break;
            case UI32:value.v_ui32 += delta.v_ui32;break;
            case Float:value.v_float += delta.v_float;break;
            default:
                break;
        }
        setValue(key,value);
        return true;
    }


    bool StatBase::getValue(int key, Variant & value,uint32_t param) const
    {
        GETREALKEY(key);
        int idx=getKeyIndex(key);
        if(idx ==-1)
            return false;
        if(m_pValues[idx].isEmpty())
        {
            const KeyInfo* keyInfo = getKeyInfo(key);
            value = keyInfo->defValue;
            return false;
        }
        value = m_pValues[idx];
        return true;
    }

    bool StatBase::getValue(int key, int &value, uint32_t param) const {
        Variant value2;
        bool bRet = getValue(key,value2,param);
        value2.Value(value);
        return bRet;
    }

    bool StatBase::getValue(int key, int64_t &value, uint32_t param) const {
        Variant value2;
        bool bRet = getValue(key,value2,param);
        value2.Value(value);
        return bRet;
    }

    bool StatBase::getValue(int key, uint32_t &value, uint32_t param) const {
        Variant value2;
        bool bRet = getValue(key,value2,param);
        value2.Value(value);
        return bRet;
    }

    bool StatBase::getValue(int key, float &value, uint32_t param) const {
        Variant value2;
        bool bRet = getValue(key,value2,param);
        value2.Value(value);
        return bRet;
    }

    bool StatBase::getValue(int key, std::string &value, uint32_t param) const {
        Variant value2;
        bool bRet = getValue(key,value2,param);
        value2.Value(value);
        return bRet;
    }

    bool StatBase::getValue(int key, bool &value, uint32_t param) const {
        Variant value2;
        bool bRet = getValue(key,value2,param);
        value2.Value(value);
        return bRet;
    }

}//end of soui