#include "word_tokenization.h"

std::unordered_map<std::string, int> read_and_cut_ci(std::string path, int ci_size)
{
	std::ifstream file_in(path);
	std::unordered_map<std::string, int> ci_map;

	if (!file_in.is_open())
	{
		std::cerr << "Error: file not found" << std::endl;
		return ci_map;
	}

	std::string line;
	int line_size;

	while (std::getline(file_in, line))
	{
		if (line.size() <= 10)
			continue;
		line.insert(0, "_B");
		line.insert(line.size(), "E_");
		line_size = line.size();
		for (int i = 0; i <= line_size - 2 * ci_size; i = i + 2)
		{
			std::string cur = line.substr(i, ci_size * 2);
			if (0 <= cur[0] && cur[0] <= 127)
			{
				i--;
				continue;
			}
			if (ci_map.find(cur) != ci_map.end())
				ci_map[cur]++;
			else
				ci_map[cur] = 1;
		}
	}
	return ci_map;
}

std::unordered_map<std::string, int> read_news(std::string path, int word_size)
{
	std::ifstream file_in(path);
	std::unordered_map<std::string, int> news_map;

	if (!file_in.is_open())
	{
		std::cerr << "Error: file not found" << std::endl;
		return news_map;
	}

	std::string line;
	int line_size;

	while (std::getline(file_in, line))
	{
		if (line.size() == 0)
			continue;
		std::istringstream iss(line);
		std::string word;
		std::vector<std::string> words;
		while (iss >> word)
		{
			int pos = word.find("/");
			word = word.substr(0, pos);
			words.push_back(word);
		}

		words[0] = "<BOS>";
		words.push_back("<EOS>");

		line_size = words.size();
		for (int i = 0; i <= line_size - word_size; i++)
		{
			std::string cur = words[i];
			for (int j = 1; j < word_size; j++)
			{
				cur += words[i + j];
			}
			if (news_map.find(cur) != news_map.end())
				news_map[cur]++;
			else
				news_map[cur] = 1;
		}
	}
	return news_map;
}

void write(std::unordered_map<std::string, int> in_map, std::string path)
{
	std::ofstream file_out(path);
	for (auto it = in_map.begin(); it != in_map.end(); it++)
		file_out << it->first << ":" << it->second << '\n';
	return;
}