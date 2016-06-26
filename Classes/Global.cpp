#include "Global.h"
int Global::score = 1;
int Global::level_one_score = 0;
int Global::level_two_score = 0;
int Global::level_three_score = 0;
#include "cocos2d.h"
USING_NS_CC;

Global::Global()
{
}


Global::~Global()
{
}

void Global::load() {
	if (UserDefault::getInstance()->getBoolForKey("isExit", false))
	{
		Global::score = UserDefault::getInstance()->getIntegerForKey("score");
		Global::level_one_score = UserDefault::getInstance()->getIntegerForKey("level_one_score");
		Global::level_two_score = UserDefault::getInstance()->getIntegerForKey("level_two_score");
		Global::level_three_score = UserDefault::getInstance()->getIntegerForKey("level_three_score");
	}
}

void Global::store() {
	if (!UserDefault::getInstance()->getBoolForKey("isExit", false))
	{
		UserDefault::getInstance()->setBoolForKey("isExit", true);
	}

	UserDefault::getInstance()->setIntegerForKey("score", Global::score);
	UserDefault::getInstance()->setIntegerForKey("level_one_score", Global::level_one_score);
	UserDefault::getInstance()->setIntegerForKey("level_two_score", Global::level_two_score);
	UserDefault::getInstance()->setIntegerForKey("level_three_score", Global::level_three_score);
	//第一次运行时，如果XML文件不存在就会新建
	UserDefault::getInstance()->flush();
}