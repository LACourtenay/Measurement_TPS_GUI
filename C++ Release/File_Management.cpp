
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Landmark.h"
#include "Measurement_Functions.h"
#include <qmessagebox.h>
#include <qdebug.h>
#include <qdir.h>

std::vector<std::string> split(const std::string str, const std::string& delim) {
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do {
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
};

double Landmark::scale = 0.0; // initiate static scale variable in class landmark

std::string read_measurements(std::ifstream& infile, std::string file_name) {
	if (infile.is_open()) {

		int index = 0;
		int count = 0;
		int lm_number = 0;
		int semi_number = 0;
		std::string image_name;
		std::vector<Landmark> landmarks;

		std::string line;
		while (std::getline(infile, line)) {
			if (index == 0) {
				//qDebug() << QString::fromStdString(line);
				lm_number = std::atoi(split(line, "LM=")[0].c_str());
				if ((lm_number != 7) && (lm_number != 0)) {
					//throw std::exception("Incorrect number of landmarks"); // for debugging
					std::string warning_message;
					warning_message = "Incorrect number of landmarks in file: " + file_name;
					QMessageBox::warning(NULL, "Warning", QString::fromStdString(warning_message));
					exit(1);
				}
			}
			else {
				if (lm_number == 7) {
					if (count < lm_number) {
						std::vector<std::string> splitted = split(line, " ");
						double first = static_cast<double>(std::atof(splitted[0].c_str()));
						double second = static_cast<double>(std::atof(splitted[1].c_str()));
						landmarks.push_back(Landmark(first, second));
						//qDebug() << QString::fromStdString("First: " + std::to_string(first) + "Second: " + std::to_string(second));
					}
					else {
						if (line.rfind("IMAGE=", 0) == 0) {
							image_name = split(line, "IMAGE=")[0];
							//qDebug() << QString::fromStdString(image_name);
						}
					}
					count++;
				}
				else {
					semi_number = 7;
					if (count < semi_number) {
						if ((line.rfind("CURVES=", 0) != 0) && (line.rfind("POINTS=", 0) != 0)) {

							std::vector<std::string> splitted = split(line, " ");
							double first = static_cast<double>(std::atof(splitted[0].c_str()));
							double second = static_cast<double>(std::atof(splitted[1].c_str()));
							landmarks.push_back(Landmark(first, second));
							//qDebug() << QString::fromStdString("First: " + std::to_string(first) + "Second: " + std::to_string(second));

							count++;
						}
					}
					if (line.rfind("IMAGE=", 0) == 0) {
						image_name = split(line, "IMAGE=")[0];
						//qDebug() << QString::fromStdString(image_name);
					}
				}
			}
			index++;
		}
		
		if (line.rfind("SCALE=", 0) != 0) {
			std::string warning_message;
			warning_message = "Scale is missing from file: " + file_name;
			QMessageBox::warning(NULL, "Warning", QString::fromStdString(warning_message));
			exit(1);
		}

		double scale = static_cast<double>(std::atof(split(line, "SCALE=")[0].c_str()));
		//qDebug() << QString::fromStdString(std::to_string(scale));
		Landmark::scale = scale;

		Landmark LM1 = landmarks[0];
		Landmark LM2 = landmarks[1];
		Landmark LM3 = landmarks[2];
		Landmark LM4 = landmarks[3];
		Landmark LM5 = landmarks[4];
		Landmark LM6 = landmarks[5];
		Landmark LM7 = landmarks[6];

		LM1.ScaleCoords();
		LM2.ScaleCoords();
		LM3.ScaleCoords();
		LM4.ScaleCoords();
		LM5.ScaleCoords();
		LM6.ScaleCoords();
		LM7.ScaleCoords();

		double WIS, WIM, WIB, LDC, RDC, OA, D;

		WIS = euclid_distance(LM1.x, LM1.y, LM7.x, LM7.y);
		WIM = euclid_distance(LM2.x, LM2.y, LM6.x, LM6.y);
		WIB = euclid_distance(LM3.x, LM3.y, LM5.x, LM5.y);
		LDC = euclid_distance(LM1.x, LM1.y, LM4.x, LM4.y);
		RDC = euclid_distance(LM4.x, LM4.y, LM7.x, LM7.y);
		OA = angle(LDC, RDC, WIS);
		D = height(WIS, heron(LDC, RDC, WIS));

		std::string output;
		output = image_name + "," + std::to_string(WIS) + "," + std::to_string(WIM) + "," + std::to_string(WIB) + "," + std::to_string(LDC) + "," +
			std::to_string(RDC) + "," +
			std::to_string(OA) + "," + std::to_string(D);

		return output;
	}
};
