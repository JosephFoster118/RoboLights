#pragma once
#include <string.h>
#include <string>
#include <lua.hpp>
#include <map>
#include <string>
#include <stdint.h>
#include "utility.h"

class LightEffect
{
private:
	char effect_directory[512];
	char* effect_param_type;
	char* effect_param_meta;
	static std::map<std::string, LightEffect*>* effect_bank;
	char* script_data;
	uint32_t lua_buffer_size;

public:
	LightEffect(const char* d, const char* type, const char* meta);
	~LightEffect();
	static LightEffect* create(const char* d);
	static LightEffect* getEffect(const char* name);
	lua_State* loadEffect();
	void getParamType(char* cp_string);
	uint16_t getParamTypeLength();
};


