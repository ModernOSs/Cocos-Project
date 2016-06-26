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
	static long score;
	static int level_one_score;
	static int level_two_score;
};

#endif
