
#include <math.h>
#define PI 3.14159265359

// euclidean distance

double euclid_distance(double x1, double y1, double x2, double y2) {
	double x_dif = pow((x1 - x2), 2);
	double y_dif = pow((y1 - y2), 2);
	return sqrt(x_dif + y_dif);
}

// angle

double angle(double side_1, double side_2, double side_3) {
	double numerator = side_1 * side_1 + side_2 * side_2 - side_3 * side_3;
	double denominator = 2.0 * side_1 * side_2;
	return acos(numerator / denominator) * 180.0 / PI;
}

// perimiter

double perimiter(double side_1, double side_2, double side_3) {
	return side_1 + side_2 + side_3;
}

// heron

double heron(double side_1, double side_2, double side_3) {
	float s = perimiter(side_1, side_2, side_3) / 2.0;
	return sqrt(s * (s - side_1) * (s - side_2) * (s - side_3));
}

// height

double height(double base, double area) {
	return (2.0 * area) / base;
}

/*

	float WIS, WIM, WIB, LDC, RDC, OA, D;

	landmark::scale = 0.000919f; // set scale across all instances of the class

	landmark LM1(228.0f, 179.0f);
	LM1.ScaleCoords();

	landmark LM2(306.0f, 135.0f);
	LM2.ScaleCoords();

	landmark LM3(373.0f, 91.0f);
	LM3.ScaleCoords();

	landmark LM4(512.0f, 51.0f);
	LM4.ScaleCoords();

	landmark LM5(568.0f, 71.0f);
	LM5.ScaleCoords();

	landmark LM6(664.0f, 104.0f);
	LM6.ScaleCoords();

	landmark LM7(818.0f, 186.0f);
	LM7.ScaleCoords();

	WIS = euclid_distance(LM1.x, LM1.y, LM7.x, LM7.y);
	WIM = euclid_distance(LM2.x, LM2.y, LM6.x, LM6.y);
	WIB = euclid_distance(LM3.x, LM3.y, LM5.x, LM5.y);
	LDC = euclid_distance(LM1.x, LM1.y, LM4.x, LM4.y);
	RDC = euclid_distance(LM4.x, LM4.y, LM7.x, LM7.y);
	OA = angle(LDC, RDC, WIS);
	D = height(WIS, heron(LDC, RDC, WIS));

	std::ofstream file ("measurements.txt");
	file << WIS << "," << WIM << "," << WIB << "," << LDC << "," << RDC << "," << OA << "," << D << std::endl;
	file.close();

*/