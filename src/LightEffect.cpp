#include "LightEffect.h"
std::map<std::string, LightEffect*>* LightEffect::effect_bank = new std::map<std::string, LightEffect*>();

LightEffect::LightEffect(const char* d, const char* type, const char* meta)
{
	strcpy(effect_directory, d);
	effect_param_type = new char[strlen(type) + 1];
	strcpy(effect_param_type, type);
	effect_param_meta = new char[strlen(meta) + 1];
	strcpy(effect_param_meta, meta);
	//load file into buffer for fatser loading in run time
	FILE* lua_file = fopen(d,"r");
	if(lua_file == NULL)//failed to load
	{
		script_data = NULL;
		lua_buffer_size = 0;
	}
	else
	{
		fseek(lua_file, 0L, SEEK_END);
		lua_buffer_size = ftell(lua_file) + 1;
		script_data = new char[lua_buffer_size];
		memset(script_data, 0, lua_buffer_size);
		rewind(lua_file);
		fread(script_data, lua_buffer_size, 1, lua_file);
		fclose(lua_file);
	}
								 
							 
}

LightEffect::~LightEffect()
{
	SAFE_DELETE_ARRAY(effect_param_type);
	SAFE_DELETE_ARRAY(effect_param_meta);
	SAFE_DELETE_ARRAY(script_data);
}


LightEffect* LightEffect::create(const char* d)
{
	printf("--------------------------------\n");
	lua_State* L = luaL_newstate();
    luaL_openlibs(L);
	if (luaL_loadfile(L, d) || lua_pcall(L, 0, 0, 0))
    {
        printf("error: %s", lua_tostring(L, -1));
		
		
        return NULL;
    }
	lua_getglobal(L, "EFFECT_NAME");
	int stat = lua_isstring(L,-1);
	if(!stat)
	{
		fprintf(stderr,"ERROR: Failed to load effect name\n");
		printf("--------------------------------\n");
		lua_close(L);
		return NULL;
	}
	std::string ef_name = lua_tostring(L,-1);
	lua_getglobal(L, "INIT_PARAM");
	stat = lua_isstring(L,-1);
	if(!stat)
	{
		fprintf(stderr,"ERROR: Failed to load effect types\n");
		printf("--------------------------------\n");
		lua_close(L);
		return NULL;
	}
	std::string ef_type = lua_tostring(L,-1);
	lua_getglobal(L, "INIT_PARAM_META");
	stat = lua_isstring(L,-1);
	if(!stat)
	{
		fprintf(stderr,"ERROR: Failed to load effect types\n");
		printf("--------------------------------\n");
		lua_close(L);
		return NULL;
	}
	std::string ef_meta = lua_tostring(L,-1);
	printf("Loading effect \"%s\"\n", ef_name.c_str());
	printf("Effect's types [%s]\n", ef_type.c_str());
	printf("Effect's meta [%s]\n", ef_meta.c_str());
	printf("--------------------------------\n");
	lua_close(L);
	
	LightEffect* effect = new LightEffect(d, ef_type.c_str(), ef_meta.c_str());
	effect_bank->insert( std::pair<std::string, LightEffect*>(ef_name, effect));
	return effect;
}

LightEffect* LightEffect::getEffect(const char* name)
{
	if (effect_bank->find(name) == effect_bank->end())
	{
		printf("Could not find effect \"%s\"\n", name);
		return NULL;
	}
	else
	{
		return effect_bank->at(name);
	}
}

lua_State* LightEffect::loadEffect()
{
	lua_State* L = luaL_newstate();
    luaL_openlibs(L);
	if (luaL_loadbuffer(L, script_data, lua_buffer_size - 1, "effect") || lua_pcall(L, 0, 0, 0))
    {
        printf("error: %s", lua_tostring(L, -1));
		
		
        return NULL;
    }
	return L;
}



void LightEffect::getParamType(char* cp_string)
{
	strcpy(cp_string,effect_param_type);
}

uint16_t LightEffect::getParamTypeLength()
{
	return strlen(effect_param_type);
}






