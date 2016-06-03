#include "WonderlandScene.h"
#include <string>
using namespace std;
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

void Wonderland::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* Wonderland::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -2940));

    auto layer = Wonderland::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());

    scene->addChild(layer);
    return scene;
}

bool Wonderland::init() {
    if (!Layer::init()) {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();

    preloadMusic();
    playBgm();

    addBackground();
    addPlayer();

	addContactListener();
    addKeyboardListener();
	addMouseListener();

    this->schedule(schedule_selector(Wonderland::update), 0.01);

    return true;
}

void Wonderland::preloadMusic() {
    // TODO
}

void Wonderland::playBgm() {
    // TODO
}

void Wonderland::addBackground() {
	Sprite* background[3];
	for (unsigned int i = 0; i < 3; i++)
	{
		background[i] = Sprite::create("colored_desert.png");
		scale = visibleSize.height / background[i]->getContentSize().height;
		background[i]->setScale(scale, scale);
		background[i]->setPosition(i * visibleSize.height + visibleSize.height / 2, visibleSize.height / 2);
		this->addChild(background[i], 0);
	}

	for (unsigned int i = 0; i < 25; i++)
	{
		ground[i] = Sprite::create("grassCenter.png");
		ground[i]->setScale(scale, scale);
		ground[i]->setPosition(i * ground[i]->getContentSize().width * scale + ground[i]->getContentSize().width * scale / 2,
			                   ground[i]->getContentSize().height * scale / 2);
		ground[i]->setPhysicsBody(PhysicsBody::createBox(Size(ground[i]->getContentSize().width * scale * 0.67,
			                                                  ground[i]->getContentSize().height * scale * 0.67),
			                                             PhysicsMaterial(100.0f, 0.0f, 0.0f)));
		ground[i]->getPhysicsBody()->setDynamic(false);
		// 地块的Tag为1
		ground[i]->setTag(1);
		// 设置掩码
		ground[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		ground[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		ground[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(ground[i], 0);
	}

	isPlayerOnGround = Sprite::create("isPlayerOnGround.png");
	isPlayerOnGround->setScale(scale * 25, scale);
	isPlayerOnGround->setPosition(ground[12]->getPosition() + Vec2(0, isPlayerOnGround->getContentSize().height * scale / 2 +
		                                                              ground[12]->getContentSize().height * scale / 2));
	isPlayerOnGround->setOpacity(0);
	this->addChild(isPlayerOnGround, 0);
}

void Wonderland::addPlayer() {
	player = Sprite::create("alienGreen_walk0.png");
	circle = Sprite::create("circle.png");
	player->setScale(scale);
	circle->setScale(scale * 0.8);
	player->setAnchorPoint(Vec2(0.5, 0.5));
	circle->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPhysicsBody(PhysicsBody::createBox(player->getContentSize() * scale * 0.7, PhysicsMaterial(1.0f, 0.0f, 0.0f)));
	player->setPosition(visibleSize / 2);
	player->getPhysicsBody()->setAngularVelocityLimit(0);
	player->getPhysicsBody()->setRotationEnable(false);
	// 玩家的Tag为0
	player->setTag(0);
	// 设置掩码
	player->getPhysicsBody()->setCategoryBitmask(0xFF);
	player->getPhysicsBody()->setCollisionBitmask(0xFF);
	player->getPhysicsBody()->setContactTestBitmask(0xFF);
	this->addChild(circle);
	this->addChild(player);
}

void Wonderland::addContactListener() {
	auto contactBeginListener = EventListenerPhysicsContact::create();
	contactBeginListener->onContactBegin = CC_CALLBACK_1(Wonderland::onContactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(contactBeginListener, 1);
}

void Wonderland::addKeyboardListener() {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Wonderland::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Wonderland::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void Wonderland::addMouseListener() {
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(Wonderland::mouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Wonderland::update(float f) {
	// 边界控制
	if (player->getPosition().x <= 80 + player->getContentSize().width * 0.7 / 2 && velocity < 0 ||
		player->getPosition().x >= visibleSize.width - 80 - player->getContentSize().width * 0.7 / 2 && velocity > 0)
		player->getPhysicsBody()->setVelocity(Vec2(0, player->getPhysicsBody()->getVelocity().y));
	else
		player->getPhysicsBody()->setVelocity(Vec2(velocity, player->getPhysicsBody()->getVelocity().y));

	// 瞄准器控制
	circle->setPosition(player->getPosition());
	static int count = 0;
	count++;
	if (count == 20)
	{
		count = 0;
		circle->runAction(RotateTo::create(0.2f, -(player->getPosition() - mousePosition).getAngle() * 180 / 3.1415926 + 90));
	}

	// 纵向速度控制
	for (int i = 0; i < 25; i++)
	    if (player->getBoundingBox().intersectsRect(ground[i]->getBoundingBox()))
		    player->getPhysicsBody()->setVelocity(Vec2(player->getPhysicsBody()->getVelocity().x, 0));
}

bool Wonderland::onContactBegan(PhysicsContact& contact) {
	// 留空
	return true;
}

void Wonderland::mouseMove(Event* event) {
	EventMouse* e = (EventMouse*)event;
	mousePosition = Vec2(e->getCursorX(), e->getCursorY());
}

void Wonderland::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
		velocity -= 600;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
		velocity += 600;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
		if (player->getBoundingBox().intersectsRect(isPlayerOnGround->getBoundingBox()))
		    player->getPhysicsBody()->setVelocity(Vec2(player->getPhysicsBody()->getVelocity().x, 1440));
        break;
    default:
        break;
    }
}

void Wonderland::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
    switch (code)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
		velocity += 600;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
		velocity -= 600;
        break;
    default:
        break;
    }
}
