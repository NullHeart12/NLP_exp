#include "Backend.h"

Backend::Backend()
{
	frag_num = 1;

	std::string ci_path = "../exp_data/Ci.txt";
	std::string news_path = "../exp_data/news.txt";
	std::unordered_map<std::string, int> temp_ci_map = read_and_cut_ci(ci_path, frag_num);
	std::unordered_map<std::string, int> temp_news_map = read_news(news_path, frag_num);
	for(auto it = temp_ci_map.begin(); it != temp_ci_map.end(); ++it)
		ci_set.insert(it->first);
	max_news_len = 0;
	for (auto it = temp_news_map.begin(); it != temp_news_map.end(); ++it)
	{
		news_set.insert(it->first);
		if (it->first.size() > max_news_len)
			max_news_len = it->first.size();
	}
	ci_set.insert("E_");
	ci_set.insert("_B");
}

bool Backend::tokenize()
{
	std::string ci_path = "../exp_data/Ci.txt";
	std::string news_path = "../exp_data/news.txt";
	std::unordered_map<std::string, int> temp_ci_map = read_and_cut_ci(ci_path, frag_num);
	std::unordered_map<std::string, int> temp_news_map = read_news(news_path, frag_num);
	write(temp_ci_map, "../exp_data/ci_count.txt");
	write(temp_news_map, "../exp_data/news_count.txt");
	
	if (!ci_map.empty())
		ci_map.clear();
	if (!news_map.empty())
		news_map.clear();
	ci_map = temp_ci_map;
	news_map = temp_news_map;
	
	return !temp_ci_map.empty() && !temp_news_map.empty();
}

std::string Backend::show_tokens(std::string path)
{
	std::string ret = "";
	std::ifstream file_in(path);
	if (!file_in.is_open())
	{
		//std::cerr << "Error: file not found" << std::endl;
		return ret;
	}
	ret = std::string(std::istreambuf_iterator<char>(file_in), std::istreambuf_iterator<char>());
	return ret;
}

bool Backend::set_frag_num(int num)
{
	if (num <= 0)
		return false;
	frag_num = num;
	return true;
}

std::string Backend::generate(std::string prefix)
{
	std::string generation = "";
	if (ci_map.empty() && news_map.empty())
		return generation;
	generation = generate_sentence(prefix);
	return generation;
}

std::string Backend::generate_sentence(std::string prefix)
{
	std::string generation = "";
	type t = prefix[0] == 'A' ? ci : prefix[0] == 'B' ? news: wrong;
	if (frag_num <= 1 or t == wrong)
		return generation;
	prefix = prefix.substr(1);
	
	if (t == ci)
		prefix = "_B" + prefix;
	else if (t == news)
		prefix = "<BOS>" + prefix;
	
	std::string suffixes = suffix(prefix, t);
	if (suffixes.empty())
		return generation;

	std::string cur_token = "";
	std::string sub_prefix = "";
	if (t == ci)
	{
		sub_prefix = cut_prefix(suffixes, t);
		while (cur_token != "E_")
		{
			cur_token = generate_token(sub_prefix, t);
			if (cur_token == "")
			{
				generation += "E_";
				break;
			}
			generation += cur_token;
			sub_prefix += cur_token;
			sub_prefix = cut_prefix(sub_prefix, t);
		}
	}
	else if (t == news)
	{
		sub_prefix = cut_prefix(suffixes, t);
		while (cur_token != "<EOS>")
		{
			cur_token = generate_token(sub_prefix, t);
			if (cur_token == "")
			{
				generation += "<EOS>";
				break;
			}
			generation += cur_token;
			sub_prefix += cur_token;
			sub_prefix = cut_prefix(sub_prefix, t);
		}
	}
	return generation;
}

std::string Backend::suffix(std::string prefix, type t)
{
	std::string ret;
	if (prefix.empty())
		return ret;
	int prefix_size = prefix.size();
	std::string token = "";
	if (t == ci)
	{
		for (int i = prefix_size - 2; i >= 0; i = i - 2)
		{
			token = prefix.substr(i);
			if (ci_map.find(token) != ci_map.end())
			{
				ret = token;
				prefix = prefix.substr(0,i);
				break;
			}
		}
	}
	else if (t == news)
	{
		for (int i = prefix_size - 2; i >= 0; i--)
		{
			token = prefix.substr(i);
			if (news_map.find(token) != news_map.end())
			{
				ret = token;
				prefix = prefix.substr(0, i);
				break;
			}
		}
	}
	return ret;
}

std::string Backend::cut_prefix(std::string sub_prefix, type t)
{
	if (t == ci)
	{
		for (int i = sub_prefix.size(); i >= 0; i--)
		{
			if (ci_set.find(sub_prefix.substr(0, i)) != ci_set.end())
			{
				sub_prefix = sub_prefix.substr(i);
				break;
			}
		}
	}
	else if (t == news)
	{
		for (int i = sub_prefix.size(); i >= 0; i--)
		{
			if (news_set.find(sub_prefix.substr(0, i)) != news_set.end())
			{
				sub_prefix = sub_prefix.substr(i);
				break;
			}
		}
	}
	return sub_prefix;
}

std::string Backend::generate_token(std::string sub_prefix, type t)
{
	std::string ret = "";
	std::vector<std::pair<std::string, int>> tokens;
	if (sub_prefix.empty())
		return ret;
	if (t == ci)
	{
		for (auto it = ci_map.begin(); it != ci_map.end(); it++)
		{
			if (sub_prefix == (it->first).substr(0, sub_prefix.size()) && ci_set.find((it->first).substr(sub_prefix.size())) != ci_set.end())
			{
				std::string temp = (it->first).substr(sub_prefix.size());
				tokens.push_back({ temp , it->second });
			}
		}
	}
	else if (t == news)
	{
		for (auto it = news_map.begin(); it != news_map.end(); it++)
		{
			if (sub_prefix == (it->first).substr(0, sub_prefix.size()) && news_set.find((it->first).substr(sub_prefix.size())) != news_set.end())
			{
				std::string temp = (it->first).substr(sub_prefix.size());
				tokens.push_back({ temp, it->second });
			}
		}
	}
	int token_size = tokens.size();
	if (token_size <= 0)
		return t == ci ? "E_" : t == news ? "<EOS>" : "";

	//After I finished the following code, I found that I can use std::discrete_distribution to generate token instead of top-k;
	//If I free, I will change the following code.
	//Of course, you can change it by yourself.
	//But now I think it's  not necessary to change it maybe.Because top-k has a better randomness.
	if (token_size <= 10)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, token_size - 1);
		int rand = dis(gen);
		ret = tokens[rand].first;
	}
	else
	{
		std::sort(tokens.begin(), tokens.end(), [](std::pair<std::string, int> a, std::pair<std::string, int> b) {return a.second > b.second; });
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 10);
		int rand = dis(gen);
		ret = tokens[rand].first;
	}
	return ret;
}

std::string Backend::FMM(std::string sentence)
{
	std::string ret = "";
	if (sentence.empty())
		return ret;
	int sentence_size = sentence.size();
	for (int i = 0; i < sentence_size; i++)//i++ is aim at jumping the tokens that don't exist in the dictionary
	{
		int rest_len = sentence_size - i;
		int cur_len = rest_len > max_news_len ? max_news_len : rest_len;
		for (int j = cur_len - 1; j >= 0; j--)
		{
			if (news_set.find(sentence.substr(i, j + 1)) != news_set.end())
			{
				ret += sentence.substr(i, j + 1);
				ret += "/";
				i += j;
				break;
			}
		}
	}
	return ret;
}

std::string Backend::BMM(std::string sentence)
{
	std::string ret = "";
	if (sentence.empty())
		return ret;
	int sentence_size = sentence.size();
	for (int i = sentence_size - 1; i >= 0; i--)
	{
		int rest_len = i + 1;
		int cur_len = rest_len > max_news_len ? max_news_len : rest_len;
		for (int j = 0; j < cur_len; j++)
		{
			if (news_set.find(sentence.substr(i - cur_len + j + 1, cur_len - j)) != news_set.end())
			{
				ret.insert(0, "/");
				ret.insert(0, sentence.substr(i - cur_len + j + 1, cur_len - j));
				i -= cur_len - j;
				i++;
				break;
			}
		}
	}
	return ret;
}