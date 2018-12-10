EFFECT_NAME = "block build"
INIT_PARAM = "int,int,int,int,float"
INIT_PARAM_META = "R,G,B,Size,Speed"

R = 0
G = 0
B = 0
SIZE = 0
SPEED = 0

PHASE_BUILD = 0
PHASE_RESET = 1

block_count = -1
block_pos = 0
phase = PHASE_BUILD
last_move = 0

function getBuildLength()
	return block_count*SIZE
end

function isFull()
	return getBuildLength() > SIZE
end


function moveBlock()
	
end


function initialize(r,g,b,size,speed,duration)
	R = r
	G = g
	B = b
	SPEED = speed
	SIZE = size
end

function render(current_time)
	local time_dif =  current_time - start_time
	setLED(0,255,255,255);
	return false
end