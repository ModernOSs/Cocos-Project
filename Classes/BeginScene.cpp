#include "BeginScene.h"
#include "SelectScene.h"
#include "cocostudio/CocoStudio.h"
#include "WonderlandScene.h"
#include <string>
using namespace std;
#include "SimpleAudioEngine.h"
USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;

void BeginScene::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* BeginScene::createScene() {
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Point(0, 0));
	scene->getPhysicsWorld()->setAutoStep(false);
	auto layer = BeginScene::create(scene->getPhysicsWorld());

	scene->addChild(layer);
	return scene;

}

bool BeginScene::init(PhysicsWorld* world) {
	if (!Layer::init()) {
		return false;
	}

	left = true;
	visibleSize = Director::getInstance()->getVisibleSize();

	this->setPhysicsWorld(world);
	addBackground();
	addPlayer();
	addMouseListener();
	this->schedule(schedule_selector(BeginScene::update), 0.01f);

	return true;
}

BeginScene * BeginScene::create(PhysicsWorld * world) {
	BeginScene* pRet = new(std::nothrow) BeginScene();
	if (pRet && pRet->init(world)) {
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

void BeginScene::addBackground() {
	auto background = Sprite::create("blue_grass.png");
	scale = visibleSize.height / background->getContentSize().height;
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);

	this->addChild(background, 0);

	Sprite* ground[12];
	for (unsigned int i = 0; i < 12; i++)
	{
		ground[i] = Sprite::create("grassMid.png");
		ground[i]->setScale(scale, scale);
		ground[i]->setPosition(i * ground[i]->getContentSize().width * scale + ground[i]->getContentSize().width * scale / 2,
			ground[i]->getContentSize().height * scale / 2);
		ground[i]->setPhysicsBody(PhysicsBody::createBox(Size(ground[i]->getContentSize().width,
			ground[i]->getContentSize().height),
			PhysicsMaterial(100.0f, 1.0f, 0.5f)));
		ground[i]->getPhysicsBody()->setDynamic(false);
		this->addChild(ground[i], 0);
	}

	auto label = Label::createWithTTF("Wonderland", "fonts\\Marker Felt.ttf", 150 * scale);
	label->setPosition(Vec2(visibleSize.width / 2,
		visibleSize.height - label->getContentSize().height));
	label->setTextColor(Color4B(128, 0, 0, 255));
	this->addChild(label, 1);

	sign = Sprite::create("sign.png");
	sign->setScale(scale + 0.2);
	sign->setPosition(visibleSize.width - sign->getContentSize().width * scale * 2, ground[0]->getContentSize().width * scale + sign->getContentSize().height * scale / 1.4);
	this->addChild(sign, 1);

	auto Begin = Label::createWithTTF("Begin", "fonts\\Marker Felt.ttf", 75 * scale);
	Begin->setPosition(Size(visibleSize.width - sign->getContentSize().width * scale * 2, ground[0]->getContentSize().width * scale + sign->getContentSize().height * scale / 1.2));
	this->addChild(Begin, 1);
}

void BeginScene::addPlayer() {
	player = Sprite::create("alienGreen_walk0.png");
	player->setScale(scale);
	player->setAnchorPoint(Vec2(0.5, 0.3));
	player->setPhysicsBody(PhysicsBody::createBox(Size(player->getContentSize().width * 0.8f,
		player->getContentSize().height * 0.65f),
		PhysicsMaterial(1.0f, 0.0f, 0.0f),
		Vec2(0, -player->getContentSize().height * 0.2)));
	player->setPosition(Vec2(visibleSize.width * 0.15, visibleSize.height / 2));
	player->getPhysicsBody()->setAngularVelocityLimit(0);
	player->getPhysicsBody()->setRotationEnable(false);
	this->addChild(player);

	auto beeFly = Animation::create();
	beeFly->addSpriteFrameWithFileName("bee.png");
	beeFly->addSpriteFrameWithFileName("bee_move.png");
	beeFly->setDelayPerUnit(0.2f);
	auto beeAnimate = Animate::create(beeFly);
	bee = Sprite::create("bee.png");
	bee->setScale(scale);
	bee->setPosition(visibleSize.width / 2, visibleSize.height - 300*scale);
	bee->runAction(RepeatForever::create(beeAnimate));
	this->addChild(bee, 1);
}

void BeginScene::addMouseListener() {
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(BeginScene::mouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	auto clickListener = EventListenerMouse::create();
	clickListener->onMouseDown = CC_CALLBACK_1(BeginScene::mouseClick, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(clickListener, this);
}

void BeginScene::mouseMove(Event* event) {
	EventMouse* e = (EventMouse*)event;
	mousePosition = Vec2(e->getCursorX(), e->getCursorY());
}

void BeginScene::mouseClick(Event* event) {
	player->stopAllActions();
	walk = Animation::create();
	for (int i = 1; i < 3; i++)
	{
		char imageFileName[128];
		sprintf(imageFileName, "alienGreen_walk%d.png", i);
		walk->addSpriteFrameWithFileName(imageFileName);
	}
	walk->setDelayPerUnit(0.1f);
	walk->setRestoreOriginalFrame(true);
	float m = mousePosition.x - player->getPosition().x;
	if (m < 0) {
		player->setFlippedX(true);
		m = -m;
	}
	else {
		player->setFlippedX(false);
	}
	walk->setLoops(m/(60 * scale));
	action_walk = Animate::create(walk);
	auto move = MoveTo::create(m / (300 * scale), Vec2(mousePosition.x, player->getPosition().y));
	auto spawn = Spawn::createWithTwoActions(Repeat::create(action_walk, 1), move);
	player->runAction(spawn);
}

void BeginScene::update(float f) {
	if (player->getBoundingBox().intersectsRect(sign->getBoundingBox())) {
		begin();
		return;
	}

	float moveTo = bee->getPositionX();
	if (left && moveTo <= 100 * scale) { left = false; bee->setFlippedX(true); }
	else if (!left && moveTo >= visibleSize.width - 100 * scale) { left = true; bee->setFlippedX(false);}
	moveTo += left ? -2 * scale : 2 * scale;
	bee->setPositionX(moveTo);

	for (int i = 0; i < 3; ++i)
	{
		getScene()->getPhysicsWorld()->step(1 / 300.0f);
	}
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setGravity(Vec2(0, -visibleSize.height * 1.9140625f));
}

void BeginScene::begin() {
	this->unschedule(schedule_selector(BeginScene::update));
	auto gamescene = SelectScene::createScene();
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5f, gamescene));
}

void BeginScene::signIn(Ref *pSender, TouchEventType type) { }
void BeginScene::signUp(Ref *pSender, TouchEventType type) { }
