
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Landmark.h"
#include "Measurement_Functions.h"
#include "File_Management.h"
#include <qmessagebox.h>
#include <qdebug.h>
#include <qdir.h>

std::string read_measurements(std::ifstream& infile, std::string file_name, std::string delim, std::string lb_sample) {
	if (infile.is_open()) {

		int index = 0; // to control line number
		int count = 0; // to control the number of landmarks
		int lm_number = 0; // to find the number of landmarks in the file
		int semi_number = 0; // number of semi-landmarks in the case of not using fixed landmarks
		int patch_count = 0; // check the count of semi-landmarks is correct
		std::string patch_value;
		std::string image_name;
		std::vector<Landmark> landmarks;

		double scale = 0;

		std::string line;
		while (std::getline(infile, line)) {
			if (index == 0) {

				//qDebug() << QString::fromStdString(line);

				lm_number = std::atoi(split(line, "LM=")[0].c_str());
				if ((lm_number != 7) && (lm_number != 0)) {

					lm_count_error(&file_name);

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
						if (line.rfind("SCALE=", 0) == 0) {
							scale = static_cast<double>(std::atof(split(line, "SCALE=")[0].c_str()));
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
						else if (line.rfind("POINTS=", 0) == 0) {
							patch_value = split(line, "POINTS=")[0];
							patch_count = patch_count + std::stoi(patch_value);
							if (patch_count > semi_number) {

								lm_count_error(&file_name);

							}
						}
					}
					if (line.rfind("IMAGE=", 0) == 0) {

						image_name = split(line, "IMAGE=")[0];

						//qDebug() << QString::fromStdString(image_name);
					}
					if (line.rfind("SCALE=", 0) == 0) {
						scale = static_cast<double>(std::atof(split(line, "SCALE=")[0].c_str()));
					}
				}
			}
			index++;
		}
		if (patch_count < semi_number) {

			lm_count_error(&file_name);

		}
		if (scale == 0) {
			std::string warning_message;
			warning_message = "Scale is missing from file: " + file_name;
			QMessageBox::warning(NULL, "Warning", QString::fromStdString(warning_message));
			exit(1);
		}

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

		double WIS, WIM, WIB, LDC, RDC, OA, D, OA_lin;

		WIS = euclid_distance(LM1.x, LM1.y, LM7.x, LM7.y);
		WIM = euclid_distance(LM2.x, LM2.y, LM6.x, LM6.y);
		WIB = euclid_distance(LM3.x, LM3.y, LM5.x, LM5.y);
		LDC = euclid_distance(LM1.x, LM1.y, LM4.x, LM4.y);
		RDC = euclid_distance(LM4.x, LM4.y, LM7.x, LM7.y);
		OA = angle(LDC, RDC, WIS);
		D = height(WIS, heron(LDC, RDC, WIS));
		OA_lin = circ_lin_transform(OA);

		std::string output;

		if (lb_sample == "") {
			output = image_name + delim + std::to_string(WIS) + delim + std::to_string(WIM) + delim + std::to_string(WIB) +
				delim + std::to_string(LDC) + delim +
				std::to_string(RDC) + delim +
				std::to_string(OA) + delim +
				std::to_string(D) + delim +
				std::to_string(OA_lin);
		}
		else {
			output = image_name + delim +
				lb_sample + delim +
				std::to_string(WIS) + delim + std::to_string(WIM) + delim + std::to_string(WIB) +
				delim + std::to_string(LDC) + delim +
				std::to_string(RDC) + delim +
				std::to_string(OA) + delim + std::to_string(D) + delim +
				std::to_string(OA_lin);
		}



		return output;
	}
};