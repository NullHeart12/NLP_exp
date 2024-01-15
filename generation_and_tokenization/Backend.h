#pragma once
#ifndef BACKEND_H
#define BACKEND_H

#include <fstream>
#include <vector>
#include <unordered_set>
#include <random>
#include "word_tokenization/word_tokenization.h"

enum type {ci, news, wrong};

class Backend
{
public:
	Backend();
	~Backend() {}

	bool tokenize();
	std::string show_tokens(std::string path);
	bool set_frag_num(int num);
	std::string generate(std::string prefix);
	std::string FMM(std::string sentence);
	std::string BMM(std::string sentence);
private:
	int frag_num;
	std::unordered_set<std::string> ci_set;
	std::unordered_set<std::string> news_set;
	std::unordered_map<std::string, int> ci_map;
	std::unordered_map<std::string, int> news_map;
	int max_news_len;

	std::string generate_sentence(std::string prefix);
	std::string suffix(std::string sentence, type t);
	std::string cut_prefix(std::string sub_prefix, type t);
	std::string generate_token(std::string sub_prefix, type t);
};

#endif