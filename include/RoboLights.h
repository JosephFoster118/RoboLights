#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <lua.hpp>
#include "utility.h"
#include "LightEffect.h"
#include "Segment.h"
#include <dirent.h> 

#define EFFECT_DIRECTORY "./effects/"


class RoboLights
{
private:
	uint16_t segment_count;
	uint16_t* segment_sizes;
	uint32_t led_count;
	uint8_t* led_data;
	bool class_failed;
	Segment** segment;
	uint32_t frame_count;
public:
	RoboLights(uint16_t seg_cnt, ...);
	LightEffect* loadEffect(const char* d);
	void clearLEDs();
	void printLEDs();
	void renderLights();
	bool insertEffect(uint8_t seg, uint8_t lay, const char* name, const char* param);
	~RoboLights();
	
};

