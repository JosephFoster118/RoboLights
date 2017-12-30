#include "Segment.h"


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
			lua_getglobal(effects[i], "render");
			lua_pushnumber(effects[i], (spec.tv_nsec/1.0e9)+spec.tv_sec);
			if (lua_pcall(effects[i], 1, 1, 0) != 0)
			{
				printf("error running render function!\n");
				lua_close(effects[i]);
				effects[i] = NULL;
			}
		}
	}
}

bool Segment::insertEffect(uint8_t lay, const char* name, const char* param)
{
	
	LightEffect* ef = LightEffect::getEffect(name);
	lua_State* l = ef->loadEffect();
	printf("POINT %d\n",1);
	if(effects[lay] != NULL)
	{
		lua_close(effects[lay]);
	}
	lua_pushinteger(l,segment_length);
	lua_setglobal(l, "segment.length");
	struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
	lua_pushnumber(l, (spec.tv_nsec/1.0e9)+spec.tv_sec);
	lua_setglobal(l, "start_time");
	effects[lay] = l;
	lua_getglobal(l, "segment.length");
	printf( "%d\n", (int)lua_tointeger(l, -1));
	
	return true;
}






