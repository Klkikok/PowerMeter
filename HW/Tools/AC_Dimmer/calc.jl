#!/usr/bin/env julia

function calc(; V_ac = 0, R2 = 220e3+10e3)
	println()
	@show V_ac
	
	R1 = 22e3
	R3 = 1e3

	V_peak = V_ac*sqrt(2)
	V_cap = V_peak/(R1+2*R2)*R1

	@show V_cap

	I_f_max = 20e-3
	I_f_min = 5e-3
	
	I_f = V_cap/R3
	@show I_f*1e3
	if I_f > I_f_max
		println("too high")
	elseif I_f < I_f_min
		println("too low")
	end
end

calc(V_ac = 100)
calc(V_ac = 240)

calc(V_ac = 9, R2 = 10e3)
