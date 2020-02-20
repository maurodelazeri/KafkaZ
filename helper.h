#pragma once

#include <chrono>
#include <chrono>
#include <string>
#include <vector>

int getpid_wrapper();

std::string gethostname_wrapper();

std::vector<char> readFileIntoVector(std::string const &FileName);

std::chrono::duration<long long int, std::milli> getCurrentTimeStampMS();
