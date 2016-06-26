#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
using std::string;
using std::vector;
class Global
{
public:
	Global();
	~Global();
	static void load();
	static void store();
	static int score;
	static int level_one_score;
	static int level_two_score;
	static int level_three_score;
};

#endif
