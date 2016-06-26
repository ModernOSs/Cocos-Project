#pragma once
#include "cocos2d.h"
#include <vector>
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;

class SelectScene :public Layer {
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SelectScene);

private:
	Vector<Sprite*> diamond;
	Size visibleSize;
	double scale;
};