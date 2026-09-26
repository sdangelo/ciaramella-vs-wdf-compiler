struct Params {
	static constexpr float R1_value = 1.0e+03;
	static constexpr float C1_value = 1.0e-06;
};

struct Impedances {
	float S1_pr;
};

struct State {
	float C1_z;
};

void calc_impedances(Impedances& impedances, float fs, Params params) {
	// Computing impedance for: R1;
	const auto R1_R = params.R1_value;
	// Computing impedance for: C1;
	const auto C1_G = 2 * params.C1_value * fs;
	const auto C1_R = 1 / C1_G;
	// Computing impedance for: S1;
	const auto S1_R = R1_R + C1_R;
	const auto S1_G = 1 / S1_R;
	impedances.S1_pr = R1_R * S1_G;
}

float process(State& state, Impedances imp, float Vin) {
	auto C1_b = state.C1_z; // C1 reflected
	auto R1_b = 0; // R1 reflected
	auto S1_b = -(R1_b + C1_b); // S1 reflected
	auto Vin_a = S1_b; // Vin incident
	auto Vin_b = 2 * Vin - Vin_a;// Vin reflected
	auto S1_a = Vin_b; // S1 incident
	auto R1_a = R1_b - imp.S1_pr * (S1_a - S1_b); // R1 incident
	auto C1_a = -S1_a - R1_b + imp.S1_pr * (S1_a - S1_b); // C1 incident
	state.C1_z = C1_a; // C1 state update
	// C1 voltage
	auto v_C1 = (C1_a + C1_b) * 0.5;
	return v_C1;
}
