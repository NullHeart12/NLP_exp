#pragma once
#ifndef WORD_TOKENIZATION_H
#define WORD_TOKENIZATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

std::unordered_map<std::string, int> read_and_cut_ci(std::string path, int ci_size);

std::unordered_map<std::string, int> read_news(std::string path, int word_size);

void write(std::unordered_map<std::string, int> in_map, std::string path);

#endif // !WORD_TOKENIZATION_H
