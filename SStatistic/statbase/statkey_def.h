

#ifdef IKEY
#undef BEGIN_KEY
#undef KEY
#undef KEY2
#undef KEY3
#undef KEY4
#undef KEY5
#undef END_KEY

#define BEGIN_KEY(enum_base) \
public: \
enum {StatID=enum_base};\
int getId() const override {return enum_base;} \
enum {kFirst=(enum_base)-1,
#define KEY5(key,name,t,def,group,realKey,report)  key,
#define KEY4(key,name,t,def,group,report)  key,
#define KEY3(key,name,t,def,group,realKey)  key,
#define KEY2(key,name,t,def,group)  key,
#define KEY(key,name,t,def)  key,
#define END_KEY() };

#else
#undef BEGIN_KEY
#undef KEY
#undef KEY2
#undef KEY3
#undef KEY4
#undef KEY5
#undef END_KEY

#define BEGIN_KEY(enum_base) \
protected:\
const KeyInfo* getKeyInfoMap(int& nCount) const override{\
    static KeyInfo k[] = {

#define KEY5(key,name,t,def,group,realKey,report)  {key,name,t,def,group,realKey,report},
#define KEY4(key,name,t,def,group,report)  {key,name,t,def,group,0,report},
#define KEY3(key,name,t,def,group,realKey)  {key,name,t,def,group,realKey,true},
#define KEY2(key,name,t,def,group)  {key,name,t,def,group,0,true},
#define KEY(key,name,t,def)  {key,name,t,def,0,0,true},
#define END_KEY() }; \
    nCount = sizeof(k)/sizeof(k[0]);\
    return k;}


#endif


