#include "SelectScene.h"
#include "cocostudio/CocoStudio.h"
#include "WonderlandScene.h"
#include "LevelOneScene.h"
#include "Global.h"
#include "LevelTwoScene.h"
#include <string>
using namespace std;
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;

Scene* SelectScene::createScene() {
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SelectScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;

}

bool SelectScene::init() {
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	auto bgsprite = Sprite::create("blue_grass.png");
	scale = visibleSize.height / bgsprite->getContentSize().height;
	bgsprite->setPosition(visibleSize / 2);
	bgsprite->setScale(visibleSize.width / bgsprite->getContentSize().width, visibleSize.height / bgsprite->getContentSize().height);
	this->addChild(bgsprite, 0);

	Vec2 vec2[9];

	double loc_scale = scale * 2;
	double size_scale = scale + 0.13;
	auto back1 = Sprite::create("HUD//boxCrate.png");
	back1->setScale(size_scale, size_scale);
	back1->setPosition(Vec2(visibleSize.width / 2 - back1->getContentSize().width * loc_scale, visibleSize.height / 2 + back1->getContentSize().width * loc_scale));
	this->addChild(back1, 1);

	vec2[0] = Vec2(visibleSize.width / 2 - back1->getContentSize().width * loc_scale, visibleSize.height / 2 + back1->getContentSize().width * loc_scale);
	vec2[1] = Vec2(visibleSize.width / 2, visibleSize.height / 2 + back1->getContentSize().width * loc_scale);
	vec2[2] = Vec2(visibleSize.width / 2 + back1->getContentSize().width * loc_scale, visibleSize.height / 2 + back1->getContentSize().width * loc_scale);
	vec2[3] = Vec2(visibleSize.width / 2 - back1->getContentSize().width * loc_scale, visibleSize.height / 2);
	vec2[4] = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	vec2[5] = Vec2(visibleSize.width / 2 + back1->getContentSize().width * loc_scale, visibleSize.height / 2);
	vec2[6] = Vec2(visibleSize.width / 2 - back1->getContentSize().width * loc_scale, visibleSize.height / 2 - back1->getContentSize().width * loc_scale);
	vec2[7] = Vec2(visibleSize.width / 2, visibleSize.height / 2 - back1->getContentSize().width * loc_scale);
	vec2[8] = Vec2(visibleSize.width / 2 + back1->getContentSize().width * loc_scale, visibleSize.height / 2 - back1->getContentSize().width * loc_scale);

	for (int i = 2; i <= Global::score; i++) {
		auto back2 = Sprite::create("HUD//boxCrate.png");
		back2->setScale(size_scale, size_scale);
		back2->setPosition(vec2[i - 1]);
		this->addChild(back2, 1);
	}

	for (int i = 1; i <= Global::score; i++) {
		auto dia = Sprite::create("HUD//hudJewel_blue.png");
		dia->setScale(size_scale * 0.6, size_scale * 0.6);
		dia->setPosition(Vec2(vec2[i - 1].x, vec2[i - 1].y + dia->getContentSize().width * 0.32));
		this->addChild(dia, 3);
		diamond.pushBack(dia);
	}

	for (int i = Global::score; i < 9; i++) {
		auto back = Sprite::create("HUD//boxCrate_double.png");
		back->setScale(size_scale, size_scale);
		back->setPosition(vec2[i]);
		this->addChild(back, 1);
	}

	Vector<MenuItem*> vec;
	MenuItemImage* GameItem;
	for (int i = 0; i < Global::score; i++) {
		char imageFileName[128];
		sprintf(imageFileName, "HUD//hud%d.png", i + 1);

		if (i == 0) GameItem = MenuItemImage::create(imageFileName, imageFileName, [&](Ref* pSender) {auto gamescene = LevelOne::createScene(i + 1); Director::getInstance()->replaceScene(gamescene); });
		else if (i == 1) GameItem = MenuItemImage::create(imageFileName, imageFileName, [&](Ref* pSender) {auto gamescene = LevelTwo::createScene(i + 1); Director::getInstance()->replaceScene(gamescene); });

		//auto GameItem = MenuItemImage::create(imageFileName, imageFileName, [&](Ref* pSender) {auto gamescene = Wonderland::createScene(i + 1); Director::getInstance()->replaceScene(gamescene); });
		GameItem->setPosition((vec2[i]));
		GameItem->setScale(size_scale, size_scale);
		vec.pushBack(GameItem);
	}

	auto menu = Menu::createWithArray(vec);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);

	for (Vector<Sprite*>::iterator it = diamond.begin(); it != diamond.end(); ++it)
	{
		auto rotate = RotateBy::create(0.15, -20);
		auto rotateRight = RotateBy::create(0.3, 40);
		auto rotateMed = RotateBy::create(0.15, -20);
		auto action = Sequence::create(rotate, rotateRight, rotateMed, NULL);
		(*(it))->runAction(RepeatForever::create(action));
	}

	return true;
}