#pragma once

std::string read_landmarks(std::ifstream& infile, std::string file_name);

void morph_individual_string(std::ifstream& infile, std::string* file_name,
	std::string* pt_landmark);

void morph_sample_string(std::string sample, std::string* pt_sample);

void morph_names_string(std::string name, std::string* pt_names);