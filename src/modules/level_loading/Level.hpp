#pragma once
#include <iostream>
#include <sstream>
#include <locale>
#include <iomanip>
#include <ctime>
#include <string>

class Level {
public:
	Level(std::string name, std::string file, std::string bestTime, std::string gold, std::string silver, std::string bronze, bool completed, int difficulty) {
		this->name = name;
		this->file = file;
		this->bestTime = bestTime;
		this->completed = completed;
		this->gold = gold;
		this->silver = silver;
		this->bronze = bronze;
		this->difficulty = difficulty;

		int bestTotal = GetSeconds(bestTime);
		this->goldTotal = GetSeconds(gold);
		this->silverTotal = GetSeconds(silver);
		this->bronzeTotal = GetSeconds(bronze);

		rank = bestTotal == 0 ? 3 : (bestTotal <= goldTotal ? 0 : (bestTotal <= silverTotal ? 1 : (bestTotal <= bronzeTotal ? 2 : 3)));
	}

	static int GetSeconds(std::string timeText) {
		std::tm time;
		std::istringstream ss(timeText);
		ss >> std::get_time(&time, "%H:%M:%S");
		return time.tm_hour * 3600 + time.tm_min * 60 + time.tm_sec;
	}

	static std::string GetString(int time) {
		time = time % (24 * 3600);
		int hour = time / 3600;
		time %= 3600;
		int minutes = time / 60;
		time %= 60;
		int seconds = time;

		std::string h = std::to_string(hour);
		if (h.length() == 1) h = "0" + h;
		std::string m = std::to_string(minutes);
		if (m.length() == 1) m = "0" + m;
		std::string s = std::to_string(seconds);
		if (s.length() == 1) s = "0" + s;

		return h + ":" + m + ":" + s;
	}

	std::string name;
	std::string file;
	std::string bestTime;
	std::string gold;
	std::string silver;
	std::string bronze;
	int goldTotal;
	int silverTotal;
	int bronzeTotal;
	int difficulty;
	int rank;
	bool completed;
};