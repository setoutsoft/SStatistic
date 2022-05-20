#pragma once
#include <string>
#include <sstream>
#include <stdint.h>

#ifndef STATSTREAM
#define STATSTREAM std::stringstream
#endif

namespace SOUI
{
    enum ValueType{
        None=0,
        I32,
        UI32,
        I64,
        Float,
        String,
        Bool,
    };

class Variant
{
public:
    Variant();
    Variant(int value);
    bool Value(int& value) const;
    Variant(int64_t value) ;
    bool Value(int64_t& value) const;
    Variant(uint32_t value) ;
    bool Value(uint32_t& value) const;
    Variant(float value) ;
    bool Value(float& value) const ;
    Variant(bool value) ;
    bool Value(bool& value) const ;

    Variant(const char* value) ;
    Variant(const std::string & value);
    bool Value(std::string & value) const;

    Variant(const Variant &src);
    ~Variant();
    void operator = (const Variant & src);
    void Clear();
    bool isEmpty() const;

    ValueType m_type;
    union {
        int v_i32;
        int64_t v_i64;
        uint32_t v_ui32;
        float   v_float;
        bool    v_bool;
    };
    std::string v_str;
};

class StatBase;
struct IStatQuery{
    virtual StatBase * getStatById(int id) = 0;
};


struct IStat2Stream;
class StatBase
{
    friend class StatPool;
protected:
    /*
    * 统计字段的属性表，在具体统计模块中，使用statkey_def.h定义的KEY系统宏来配置。
    */
    struct KeyInfo
    {
        int key;        //标识统计量的KEY，命名时注意带上类型
        const char* name;//统计量统计上报的name，为nullptr时，该统计量为为中间变量，不上报
        ValueType type;    //统计量类型
        Variant defValue;//统计量默认值
        int group;        //统计量的分组，有时候不同组的统计量相关性比较大，可能需要放到一个相同的统计模块中，但上报时机不相同，用group可以分开
        int realKey;      //应对特定情况下，一个相同的统计量在不同的group上报中name不相同的情况。当前key负责提供name,而realKey则负责提供数据
        bool reportAlways;//特定情况下有些统计量在没有被主动赋值（是默认值）的时候不上报，则将该属性设置为false
    };
    virtual const KeyInfo* getKeyInfoMap(int& nCount) const = 0;
    virtual void onKeyValueChanged(int key){}

public:
    StatBase(const char *name="");
    virtual ~StatBase();
public:
    StatBase * queryStat(int statId);
    const  StatBase * queryStat(int statId) const;
	virtual std::string getName() const {return m_name;}
	void init(IStatQuery *pStatQuery);

    bool verifyKeymap() const;
    virtual int getId() const = 0;
	virtual void getDesc(STATSTREAM &desc, uint32_t group = -1, uint32_t param = 0);

    bool setValue(int key, const Variant & value);
    bool updateValue(int key,const Variant & delta);
    bool getValue(int key, Variant & value,uint32_t param) const;

    virtual bool setValue(int key, int value);
    virtual bool setValue(int key, int64_t value);
    virtual bool setValue(int key, uint32_t value);
    virtual bool setValue(int key, float value);
    virtual bool setValue(int key, const std::string& value);
    virtual bool setValue(int key, bool value);

    virtual bool getValue(int key, int &value, uint32_t param) const;
    virtual bool getValue(int key, int64_t &value, uint32_t param) const;
    virtual bool getValue(int key, uint32_t &value, uint32_t param) const;
    virtual bool getValue(int key, float &value, uint32_t param) const;
    virtual bool getValue(int key, std::string &value, uint32_t param) const;
    virtual bool getValue(int key, bool &value, uint32_t param) const;

    virtual void clear(uint32_t group=-1);

protected:
    const KeyInfo* getKeyInfo(int key) const;
    const char* getKeyName(int key) const;
    void getKeyRange(int& first, int& last) const;
    int   getKeyIndex(int key) const;
protected:
    Variant * m_pValues;
    IStatQuery * m_pStatQuery;
	std::string m_name;

	static IStat2Stream * s_stat2Stream;
};


struct IStat2Stream{
	virtual void OnStatBegin(STATSTREAM & stm) = 0;
	virtual void OnStatEnd(STATSTREAM & stm) = 0;
	virtual void OnStatKeyValue(STATSTREAM &stm, const std::string &key, const Variant & value,int iKey) =0;
};

}//end of soui

