#include "Segment.h"

using namespace std;


//uint8_t Segment::DEFUALT_LAYER_COUNT = 4;

Segment::Segment(uint8_t * data, uint16_t length)
{
	light_data = data;
	segment_length = length;
	layer_count = DEFUALT_LAYER_COUNT;
	//set up layer lua states (effects0
	effects = new lua_State*[layer_count];
	for(int i = 0; i < layer_count; i++)
	{
		effects[i] = NULL;
	}
	//Set background
	background_color[R] = 0;
	background_color[G] = 0;
	background_color[B] = 0;
}

Segment::~Segment()
{
	SAFE_DELETE_ARRAY(effects);
}


void Segment::clearSegment()
{
	for(int i = 0; i < segment_length; i++)
	{
		setLED(i, background_color[R], background_color[G], background_color[B]);
	}
}

void Segment::setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
	
	if(index > segment_length)
	{
		printf("Out of segment bounds\n");
		return;
	}
	light_data[index*3 + R] = r;
	light_data[index*3 + G] = g;
	light_data[index*3 + B] = b;
}

void Segment::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
	background_color[R] = r;
	background_color[G] = g;
	background_color[B] = b;
}

void Segment::renderLights()
{
	struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
	for(int i = 0; i < layer_count; i++)
	{
		if(effects[i] != NULL)
		{
			int stack_size = lua_gettop(effects[i]);
			lua_pop(effects[i], stack_size);
			lua_getglobal(effects[i], "render");
			lua_pushnumber(effects[i], (spec.tv_nsec/1.0e9)+spec.tv_sec);
			int top = lua_gettop(effects[i]);
			if (lua_pcall(effects[i], 1, 1, 0) != 0)
			{
				printf("error running render function!\n");
				removeEffect(i);
				return;
			}
			////////////////////Check if effect is done////////////////////

			bool result = lua_toboolean(effects[i], -1);
			if(result == true)
			{
				removeEffect(i);
			}
		}
	}
}

void Segment::removeEffect(uint16_t index)
{
	printf("Removing effect %d\n", index);
	lua_close(effects[index]);
	effects[index] = NULL;
}

bool Segment::insertEffect(uint8_t lay, const char* name, const char* param)
{
	
	LightEffect* ef = LightEffect::getEffect(name);
	if(ef == NULL)
	{
		return false;
	}
	char* param_type = new char[ef->getParamTypeLength()];
	ef->getParamType(param_type);
	uint16_t effect_param_count = 0;
	for(int i = 0; param_type[i]!=0; i++)
	{
		if(param_type[i] == ',')
		{
			effect_param_count++;
		}	
	}
	if(ef->getParamTypeLength() != 0)
	{
		effect_param_count++;
	}
	printf("Effect param count is %d\n", effect_param_count);
	uint16_t given_param_count = 0;
	for(int i = 0; param[i]!=0; i++)
	{
		if(param[i] == ',')
		{
			given_param_count++;
		}	
	}
	if(ef->getParamTypeLength() != 0)
	{
		given_param_count++;
	}
	printf("Given param count is %d\n", given_param_count);
	
	if(given_param_count != effect_param_count)
	{
		return false;
	}
	
	
	
	
	lua_State* l = ef->loadEffect();
	printf("POINT %d\n",1);
	if(effects[lay] != NULL)
	{
		lua_close(effects[lay]);
		return false;
	}
	lua_pushinteger(l,segment_length);
	lua_setglobal(l, "length");
	struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
	lua_pushnumber(l, (spec.tv_nsec/1.0e9)+spec.tv_sec);
	lua_setglobal(l, "start_time");
	Segment* seg = this;
	//Segment **ud = static_cast<Segment **>(lua_newuserdata(l, sizeof *ud));
	//*ud = object;
	lua_pushlightuserdata(l, seg);
	lua_setglobal(l, "___self___");
	////////////////////Push functions to state////////////////////
	lua_register(l,"setLED",Segment::setLEDProxy);
	////////////////////Initialize effect (Make sure you have the correct params!)////////////////////
	char* given_param = new char[strlen(param)];
	strcpy(given_param, param);
	for(int i = 0; given_param[i]!=0; i++)
	{
		if(given_param[i] == ',')
		{
			given_param[i] = ' ';
		}	
	}
	
	for(int i = 0; param_type[i]!=0; i++)
	{
		if(param_type[i] == ',')
		{
			param_type[i] = ' ';
		}	
	}
	
	////////////////////Give lua the params////////////////////
	stringstream given_param_stream;
	given_param_stream << given_param;
	stringstream param_type_stream;
	param_type_stream << param_type;
	//printf("%s\n", given_param);
	lua_getglobal(l, "initialize");
	for(int i = 0; i < given_param_count; i++)
	{
		string the_type;
		param_type_stream >> the_type;
		if(the_type.compare("int") == 0)
		{
			//printf("Got int!\n");
			int recv_int = 0;
			given_param_stream >> recv_int;
			//sprintf("Got %d!\n", recv_int);
			lua_pushinteger(l, recv_int);
		}
		else if(the_type.compare("float") == 0)
		{
			//printf("Got int!\n");
			double recv_double= 0;
			given_param_stream >> recv_double;
			//sprintf("Got %d!\n", recv_int);
			lua_pushnumber(l, recv_double);
		}
		else
		{
			printf("error parsing effect param types\n");
			lua_close(l);
			return false;
		}
	}
	if (lua_pcall(l, given_param_count, 0, 0) != 0)
	{
		printf("error initialize render function!\n");
		lua_close(l);
		return false;
	}
	
	
	
	effects[lay] = l;
	
	delete [] param_type;
	delete [] given_param;
	return true;
}

int Segment::setLEDProxy(lua_State *L)
{
	
	if(lua_gettop(L) == 4)
	{
		lua_getglobal(L, "___self___");
		Segment* seg = static_cast<Segment*>(lua_touserdata(L, -1));
		uint8_t index = lua_tointeger(L, 1);
		uint8_t r = lua_tointeger(L, 2);
		uint8_t g = lua_tointeger(L, 3);
		uint8_t b = lua_tointeger(L, 4);
    	//printf("setting non-alpha %3d %3d %3d at %3d\n", r, g, b, index);
		int stack_size = lua_gettop(L);
		lua_pop(L, stack_size);
		lua_getglobal(L, "LED_START_AT_1");
		int stat = lua_isboolean(L,-1);
		if(!stat)
		{
			seg->setLED(index, r, g, b);
		}
		else
		{
			seg->setLED(index - 1, r, g, b);
		}
	}
	else if(lua_gettop(L) == 5)
	{
		lua_getglobal(L, "___self___");
		Segment* seg = static_cast<Segment*>(lua_touserdata(L, -1));
		//printf("setting with alpha\n");
	}
	else
	{
		return luaL_error(L, "expecting exactly 4 or 5 arguments");
	}
	return 0;
}
























