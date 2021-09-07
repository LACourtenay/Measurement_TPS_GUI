#pragma once

std::vector<std::string> split(const std::string str, const std::string& delim);

std::string read_measurements(std::ifstream& infile, std::string file_name);