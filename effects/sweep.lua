
EFFECT_NAME = "sweep"
INIT_PARAM = "int,int,int,int,float,float"
INIT_PARAM_META = "R,G,B,Size,Speed,Diration"

R = 0
G = 0
B = 0
SIZE = 0
SPEED = 0
DURATION = 0.0
position = 0

function initialize(r,g,b,size,speed,duration)
	R = r
	G = g
	B = b
	SPEED = speed
	SIZE = size
	DURATION = duration
end

function render(current_time)
	local time_dif =  current_time - start_time
	if time_dif > DURATION and DURATION > 0 then
		return true
	end
	position = (time_dif*SPEED)
	--print(tostring(position % 10))
	
	for i = 0, SIZE - 1 do
		--print("Boop" .. tostring(position))
		setLED(math.floor(position + i)%length,R,G,B)
	end
	
	
	return false
end




