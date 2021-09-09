
#include <string>
#include <fstream>
#include <vector>
#include "File_Management.h"
#include "Landmark.h"
#include <qmessagebox.h>
#include <qdebug.h>
#include <qdir.h>

std::string read_landmarks(std::ifstream& infile, std::string file_name)
{
	if (infile.is_open()) {

		std::string morph_landmarks;

		int index = 0; // to control line number
		int count = 0; // to control the number of landmarks
		int lm_number = 0; // to find the number of landmarks in the file
		int semi_number = 0; // number of semi-landmarks in the case of not using fixed landmarks
		int patch_count = 0; // check the count of semi-landmarks is correct
		std::string patch_value;
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

						check_decimal_char(&line, &file_name);

						std::vector<std::string> splitted = split(line, " ");
						double first = static_cast<double>(std::atof(splitted[0].c_str()));
						double second = static_cast<double>(std::atof(splitted[1].c_str()));
						landmarks.push_back(Landmark(first, second));

						//qDebug() << QString::fromStdString("First: " + std::to_string(first) + "Second: " + std::to_string(second));
					}
					if (line.rfind("SCALE=", 0) == 0) {
						scale = static_cast<double>(std::atof(split(line, "SCALE=")[0].c_str()));
					}
					count++;
				}
				else {
					semi_number = 7;
					if (count < semi_number) {
						if ((line.rfind("CURVES=", 0) != 0) && (line.rfind("POINTS=", 0) != 0)) {

							check_decimal_char(&line, &file_name);

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
			warning_message = "Scale is missing from file: " + file_name + ".tps";
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
		std::string lm_1_txt = std::to_string(LM1.x) + ' ' + std::to_string(LM1.y);
		LM2.ScaleCoords();
		std::string lm_2_txt = std::to_string(LM2.x) + ' ' + std::to_string(LM2.y);
		LM3.ScaleCoords();
		std::string lm_3_txt = std::to_string(LM3.x) + ' ' + std::to_string(LM3.y);
		LM4.ScaleCoords();
		std::string lm_4_txt = std::to_string(LM4.x) + ' ' + std::to_string(LM4.y);
		LM5.ScaleCoords();
		std::string lm_5_txt = std::to_string(LM5.x) + ' ' + std::to_string(LM5.y);
		LM6.ScaleCoords();
		std::string lm_6_txt = std::to_string(LM6.x) + ' ' + std::to_string(LM6.y);
		LM7.ScaleCoords();
		std::string lm_7_txt = std::to_string(LM7.x) + ' ' + std::to_string(LM7.y);

		morph_landmarks = lm_1_txt + '\n' + lm_2_txt + '\n' +
			lm_3_txt + '\n' + lm_4_txt + '\n' + lm_5_txt + '\n' + lm_6_txt + '\n' + lm_7_txt;

		return morph_landmarks;

	}
}

void morph_individual_string(std::ifstream& infile, std::string name,
	std::string* pt_landmark)
{
	name.resize(name.size() - 4);
	std::string lm_from_file = read_landmarks(infile, name);
	std::string lm_conj = "\u0027" + name + "\n" + lm_from_file + "\n\n";
	*pt_landmark = *pt_landmark + lm_conj;
}

void morph_sample_string(std::string sample, std::string* pt_sample)
{
	*pt_sample = *pt_sample + sample + "\n";
}

void morph_names_string(std::string name, std::string* pt_names)
{
	name.resize(name.size() - 4);
	*pt_names = *pt_names + name + "\n";
}