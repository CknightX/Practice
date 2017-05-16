#ifndef _ENV_H
#define _ENV_H
#include<map>
#include<string>
#include "type.h"

/*环境*/
class Type;

typedef std::map<std::string, Type*> name_type;

class Env
{
public:
	Env():outer(nullptr){}
	Env(parms_name_list& parms_name, parms_value_list& parms_value, Env* outer)
		:outer(outer)
	{
		auto iter1 = parms_name.begin();
		auto iter2 = parms_value.begin();
		for (; iter1 != parms_name.end() && iter2 != parms_value.end(); ++iter1, ++iter2)
			env[*iter1] = *iter2;
	}
	~Env(){}
public:
	Type* find();

	name_type env;
	Env* outer; //外部环境
};
#endif