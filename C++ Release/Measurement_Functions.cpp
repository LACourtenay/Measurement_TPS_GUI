
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

// linear transformation of OA variable

double circ_lin_transform(double OA) {
	double theta = OA * PI / 180;
	double cos_theta = cos(theta);
	double sin_theta = sin(theta);
	return cos_theta + sin_theta;
}