#pragma once

struct Landmark {

	static double scale;
	double x, y;

	Landmark(double X, double Y) {
		x = X;
		y = Y;
	}

	void ScaleCoords() {
		x = x * scale;
		y = y * scale;
	}

};
