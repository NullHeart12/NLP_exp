#include "word_tokenization.h"

int main()
{
	std::string ci_path = "../exp_data/Ci.txt";
	std::string news_path = "../exp_data/news.txt";
	std::unordered_map<std::string, int> ci_map = read_and_cut_ci(ci_path, 1);
	std::unordered_map<std::string, int> news_map = read_news(news_path, 1);
	write(ci_map, "../exp_data/ci_count.txt");
	write(news_map, "../exp_data/news_count.txt");
	return 0;
}