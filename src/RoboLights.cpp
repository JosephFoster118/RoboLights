#include "RoboLights.h"

RoboLights::RoboLights(uint16_t seg_cnt, ...)
{
	frame_count = 0;
	class_failed = false;
	segment_count = seg_cnt;
	segment_sizes = new uint16_t[segment_count];
	for(int i = 0; i < segment_count; i++)
	{
		segment_sizes[i] = 0;
	}
	va_list vl;
	va_start(vl,seg_cnt);
	uint32_t total_size = 0;
	for(int i = 0; i < segment_count; i++)
	{
		segment_sizes[i] = (uint16_t)va_arg(vl,int);
		total_size += segment_sizes[i];
	}
	led_count = total_size;
	va_end(vl);
	printf("%d segments initialized with the following sizes\n", segment_count);
	
	for(int i = 0 ; i < segment_count; i++)
	{
		printf("Segment %3d: %d\n",i,segment_sizes[i]);
	}
	led_data = new uint8_t[total_size*3];
	for(int i = 0; i < led_count*3; i++)
	{
		led_data[i] = 0;
	}
	printf("A total of %d LEDs initialized\n", led_count);
	/////Load Effects
	DIR           *d;
	struct dirent *dir;
	d = opendir(EFFECT_DIRECTORY);
	if (d)
	{
		int effect_count = 0;
		while ((dir = readdir(d)) != NULL)
		{
			if(strstr(dir->d_name, ".lua") != NULL)
			{
				printf("Loading %s\n",dir->d_name);
				char full_dir[512];
				strcpy(full_dir,EFFECT_DIRECTORY);
				strcat(full_dir, dir->d_name);
				LightEffect* le = loadEffect(full_dir);
				if(le != NULL)
				{
					effect_count++;
				}
			}
		}
		printf("Loaded %d effects\n",effect_count);
		closedir(d);
	}
	else
	{
		printf("ERROR: Failed to find \"effects\" directory\n");
		class_failed = true;
	}
	//Set up segments
	segment = new Segment*[segment_count];
	uint16_t led_counter = 0;
	for(int i = 0; i < segment_count; i++)
	{
		segment[i] = new Segment(led_data + led_counter*3, segment_sizes[i]);
		led_counter+= segment_sizes[i];
	}
	
}

LightEffect* RoboLights::loadEffect(const char* d)
{
	LightEffect* le = LightEffect::create(d);
	return le;
}

RoboLights::~RoboLights()
{
	for(int i = 0; i < segment_count; i++)
	{
		SAFE_DELETE(segment[i]);
	}
	SAFE_DELETE_ARRAY(led_data);
	SAFE_DELETE_ARRAY(segment_sizes);
}


void RoboLights::clearLEDs()
{
	for(int i = 0; i < led_count*3; i++)
	{
		led_data[i] = 0;
	}
}

void RoboLights::printLEDs()
{
	int led_counter = 0;
	for(int i = 0; i < segment_count; i++)
	{
		printf("Segment %d\n", i);
		for(int j = 0; j < segment_sizes[i]; j++)
		{
			printf("{%3d} [%3d] [%3d] [%3d]\n", j,
				   led_data[led_counter*3 + Segment::R],
				   led_data[led_counter*3 + Segment::G],
				   led_data[led_counter*3 + Segment::B]);
			led_counter++;
		}
	}
}

void RoboLights::renderLights()
{
	for(int i = 0; i < segment_count; i++)
	{
		segment[i]->clearSegment();
		segment[i]->renderLights();
	}
}

bool RoboLights::insertEffect(uint8_t seg, uint8_t lay, const char* name, const char* param)
{
	return segment[seg]->insertEffect(lay, name, param);
}












