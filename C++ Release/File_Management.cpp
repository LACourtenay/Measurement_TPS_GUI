
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
}; // Función de Alberto Morcillo Sanz https://github.com/MorcilloSanz

double Landmark::scale = 0.0; // initiate static scale variable in class landmark

void lm_count_error(std::string* file_name) { // is the pointer the right size?
	std::string warning_message;
	warning_message = "Incorrect number of landmarks in file: " + *file_name;
	QMessageBox::warning(NULL, "Warning", QString::fromStdString(warning_message));
	exit(1);
}

void check_label(std::string* label) {
	std::string test = *label;

	if (test == "") {
		return;
	}

	char space = ' ';
	for (int i = 0; i < test.size(); i++) {
		if (test[i] == space) {
			QMessageBox::warning(NULL, "Invalid Character!", "Sample labels should not cointain spaces! Use underscores '_' instead.");
			*label = "INVALID_LABEL";
		}
	}
	
}

