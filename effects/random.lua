
EFFECT_NAME = "random"
INIT_PARAM = "float"
INIT_PARAM_META = "speed"

SPEED = 0
values = {}
next_time = 0

function initialize(speed)
	SPEED = speed
	for i = 1, length do
			values[i] = {}
		for j = 1, 3 do
			values[i][j] = math.random(0,255)
		end
	end
	next_time = SPEED
end

function render(current_time)
	local time_dif =  current_time - start_time
	--print(tostring(position % 10))
	if time_dif > next_time then
		next_time = next_time + SPEED
		for i = 1, length do
			for j = 1, 3 do
				values[i][j] = math.random(0,255)
			end
		end
	end
	
	for i = 1, length do
		--print("Boop" .. tostring(position))
		setLED(i - 1,values[i][1],values[i][2],values[i][3])
	end
	
	
	return false
end




