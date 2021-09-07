#pragma once

std::vector<std::string> split(const std::string str, const std::string& delim);

void check_label(std::string* label);

void lm_count_error(std::string* file_name);