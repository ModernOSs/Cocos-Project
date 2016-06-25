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

	auto back1 = Sprite::create("HUD//boxCrate.png");
	back1->setScale(scale, scale);
	back1->setPosition(Vec2(visibleSize.width / 2 - back1->getContentSize().width * scale * 1.5, visibleSize.height / 2 + back1->getContentSize().width * scale * 1.5));
	this->addChild(back1, 1);

	vec2[0] = Vec2(visibleSize.width / 2 - back1->getContentSize().width * scale * 1.5, visibleSize.height / 2 + back1->getContentSize().width * scale * 1.5);
	vec2[1] = Vec2(visibleSize.width / 2, visibleSize.height / 2 + back1->getContentSize().width * scale * 1.5);
	vec2[2] = Vec2(visibleSize.width / 2 + back1->getContentSize().width * 1.5 * scale, visibleSize.height / 2 + back1->getContentSize().width * 1.5 * scale);
	vec2[3] = Vec2(visibleSize.width / 2 - back1->getContentSize().width * 1.5 * scale, visibleSize.height / 2);
	vec2[4] = Vec2(visibleSize.width / 2, visibleSize.height / 2);
	vec2[5] = Vec2(visibleSize.width / 2 + back1->getContentSize().width * 1.5 * scale, visibleSize.height / 2);
	vec2[6] = Vec2(visibleSize.width / 2 - back1->getContentSize().width * 1.5 * scale, visibleSize.height / 2 - back1->getContentSize().width * 1.5 * scale);
	vec2[7] = Vec2(visibleSize.width / 2, visibleSize.height / 2 - back1->getContentSize().width * 1.5 * scale);
	vec2[8] = Vec2(visibleSize.width / 2 + back1->getContentSize().width * 1.5 * scale, visibleSize.height / 2 - back1->getContentSize().width * 1.5 * scale);

	for (int i = 2; i <= Global::score; i++) {
		auto back2 = Sprite::create("HUD//boxCrate.png");
		back2->setScale(scale, scale);
		back2->setPosition(vec2[i - 1]);
		this->addChild(back2, 1);
	}

	for (int i = Global::score; i < 9; i++) {
		auto back = Sprite::create("HUD//boxCrate_double.png");
		back->setScale(scale, scale);
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
		GameItem->setScale(scale, scale);
		vec.pushBack(GameItem);
	}

	auto menu = Menu::createWithArray(vec);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);
	return true;
}
