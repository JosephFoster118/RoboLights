#pragma once
#include <string.h>
#include <lua.hpp>
#include "utility.h"
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include "LightEffect.h"

class Segment
{
private:
	
	uint8_t layer_count;
	uint8_t* light_data;
	lua_State** effects;
	uint8_t background_color[3];
	uint16_t segment_length;
	
public:
	static const uint8_t DEFUALT_LAYER_COUNT = 4;
	static const uint8_t R = 0;
	static const uint8_t G = 1;
	static const uint8_t B = 2;
	Segment(uint8_t * data, uint16_t length);
	void clearSegment();
	void setLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
	void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
	void renderLights();
	bool insertEffect(uint8_t lay, const char* name, const char* param);
	~Segment();
};

