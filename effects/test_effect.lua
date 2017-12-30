
EFFECT_NAME = "TestEffect"
INIT_PARAM = "int,int,int"
INIT_PARAM_META = "R,G,B"

segment = 2

function render(current_time)
	local time_dif =  current_time - start_time
	print("Run time " .. tostring(time_dif))
	
end

