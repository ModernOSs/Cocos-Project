#include "WonderlandScene.h"
#include "SelectScene.h"
#include <string>
using namespace std;
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

Scene* Wonderland::createScene(int game) {
    auto scene = Scene::createWithPhysics();
    // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -2940));
	scene->getPhysicsWorld()->setAutoStep(false);

    auto layer = Wonderland::create();
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

	addCamera();

	addContactListener();
    addKeyboardListener();
	addMouseListener();

    this->schedule(schedule_selector(Wonderland::update), 0.01f);

    return true;
}

void Wonderland::preloadMusic() {
    // TODO
}

void Wonderland::playBgm() {
    // TODO
}

void Wonderland::addBackground() {
	// 设置背景
	Sprite* background[3];
	for (unsigned int i = 0; i < 3; i++)
	{
		background[i] = Sprite::create("colored_desert.png");
		scale = visibleSize.height / background[i]->getContentSize().height;
		background[i]->setScale(scale, scale);
		background[i]->setPosition(i * visibleSize.height + visibleSize.height / 2, visibleSize.height / 2);
		this->addChild(background[i], 0);
	}

	// 设置土块
	for (unsigned int i = 0; i < 25; i++)
	{
		ground[i] = Sprite::create("grassCenter.png");
		ground[i]->setScale(scale, scale);
		ground[i]->setPosition(i * ground[i]->getContentSize().width * scale + ground[i]->getContentSize().width * scale / 2,
			                   ground[i]->getContentSize().height * scale / 2);
		ground[i]->setPhysicsBody(PhysicsBody::createBox(Size(ground[i]->getContentSize().width,
			                                                  ground[i]->getContentSize().height),
			                                             PhysicsMaterial(100.0f, 1.0f, 0.5f)));
		ground[i]->getPhysicsBody()->setDynamic(false);
		// 地块的Tag为1
		ground[i]->setTag(1);
		// 设置掩码
		ground[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		ground[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		ground[i]->getPhysicsBody()->setContactTestBitmask(0xFF);

		if (i > 8 && i < 18)
			continue;
		this->addChild(ground[i], 0);
	}

	//添加钻石
	for (int i = 0; i < 3; i++) {
		auto dia = Sprite::create("HUD//hudJewel_blue.png");
		dia->setPhysicsBody(PhysicsBody::createBox(Size(dia->getContentSize().width*0.8,
			dia->getContentSize().height*0.8),
			PhysicsMaterial(0.0f, 0.0f, 0.0f),
			Vec2(0, -dia->getContentSize().height * 0.1)));
		dia->getPhysicsBody()->setDynamic(false);
		dia->setTag(7);
		dia->getPhysicsBody()->setCategoryBitmask(0xFF);
		dia->getPhysicsBody()->setCollisionBitmask(0xFF);
		dia->getPhysicsBody()->setContactTestBitmask(0xFF);
		dia->setScale(scale, scale);
		diamond.pushBack(dia);
	}
	diamond.at(0)->setPosition(3 * ground[0]->getContentSize().width * scale + ground[0]->getContentSize().width * scale / 2,
		ground[0]->getContentSize().height * scale * 2);
	this->addChild(diamond.at(0), 2);
	diamond.at(1)->setPosition(20 * ground[0]->getContentSize().width * scale + ground[0]->getContentSize().width * scale / 2,
		ground[0]->getContentSize().height * scale * 6);
	this->addChild(diamond.at(1), 2);
	diamond.at(2)->setPosition(13 * ground[0]->getContentSize().width * scale + ground[0]->getContentSize().width * scale / 2,
		ground[0]->getContentSize().height * scale * 2);
	this->addChild(diamond.at(2), 2);

	//添加重启按钮
	restartMenu = MenuItemImage::create("restart.png", "restart.png", [&](Ref* pSender) { auto gamescene = SelectScene::createScene(); Director::getInstance()->replaceScene(gamescene); });
	restartMenu->setScale(scale - 0.2, scale - 0.2);
	restartMenu->setPosition(restartMenu->getContentSize().width / 2, visibleSize.height - restartMenu->getContentSize().height / 4.3);
	restart = Menu::createWithItem(restartMenu);
	restart->setPosition(Vec2::ZERO);
	this->addChild(restart, 4);

	isPlayerOnGround[0] = Sprite::create("isPlayerOnGround.png");
	isPlayerOnGround[0]->setScale(scale * 24, scale);
	isPlayerOnGround[0]->setPosition(ground[12]->getPosition() + Vec2(0, isPlayerOnGround[0]->getContentSize().height * scale / 2 +
		                             ground[12]->getContentSize().height * scale / 2));

	isPlayerOnGround[0]->setOpacity(0);
	this->addChild(isPlayerOnGround[0], 0);

	//添加出口
	exit = Sprite::create("signExit.png");
	exit->setScale(scale, scale);
	exit->setPosition(22 * ground[0]->getContentSize().width * scale + ground[0]->getContentSize().width * scale / 2,
		ground[0]->getContentSize().height * scale * 1.5);

	this->addChild(exit, 3);

	// 添加提示语
	hint[0] = Sprite::create("hint1.png");
	hint[0]->setScale(scale * 0.5, scale * 0.5);
	hint[0]->setPosition(3 * ground[0]->getContentSize().width * scale + ground[0]->getContentSize().width * scale / 2,
		ground[0]->getContentSize().height * scale * 3.3);
	hint[0]->setOpacity(180);
	this->addChild(hint[0], 4);
	hint[1] = Sprite::create("hint2.png");
	hint[1]->setScale(scale * 0.4, scale * 0.4);
	hint[1]->setPosition(visibleSize.width / 2 + ground[0]->getContentSize().width * scale * 2,
		                 visibleSize.height - ground[0]->getContentSize().width * scale * 1.5);
	hint[1]->setOpacity(180);
	this->addChild(hint[1], 4);
	hint[2] = Sprite::create("hint3.png");
	hint[2]->setScale(scale * 0.4, scale * 0.4);
	hint[2]->setPosition(13 * ground[0]->getContentSize().width * scale + ground[0]->getContentSize().width * scale / 2,
						 ground[0]->getContentSize().height * scale * 5);
	hint[2]->setOpacity(180);
	this->addChild(hint[2], 4);

	// 设置石块
	for (unsigned int i = 0; i < 11; i++)
	{
		stone[i] = Sprite::create("stoneCenter.png");
		stone[i]->setScale(scale, scale);
		stone[i]->setPosition(ground[8 + i]->getPosition() + Vec2(0, stone[i]->getContentSize().height * 2.5 * scale));
		stone[i]->setPhysicsBody(PhysicsBody::createBox(Size(stone[i]->getContentSize().width,
			                                                 stone[i]->getContentSize().height),
			                                            PhysicsMaterial(100.0f, 1.0f, 0.3f)));
		stone[i]->getPhysicsBody()->setDynamic(false);
		// 石块的Tag为3
		stone[i]->setTag(3);
		// 设置掩码
		stone[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		stone[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		stone[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(stone[i], 0);
	}

	isPlayerOnGround[1] = Sprite::create("isPlayerOnGround.png");
	isPlayerOnGround[1]->setScale(scale * 10, scale);
	isPlayerOnGround[1]->setPosition(stone[5]->getPosition() + Vec2(0, isPlayerOnGround[1]->getContentSize().height * scale / 2 +
		                             stone[5]->getContentSize().height * scale / 2));

	isPlayerOnGround[1]->setOpacity(0);
	this->addChild(isPlayerOnGround[1], 0);
}

void Wonderland::addPlayer() {
	player = Sprite::create("alienGreen_walk0.png");
	circle = Sprite::create("circle.png");
	player->setScale(scale);
	circle->setScale(scale * 0.8);
	player->setAnchorPoint(Vec2(0.5, 0.3));
	circle->setAnchorPoint(Vec2(0.5, 0.5));
	player->setPhysicsBody(PhysicsBody::createBox(Size(player->getContentSize().width * 0.8f,
		                                               player->getContentSize().height * 0.65f),
	                                              PhysicsMaterial(1.0f, 0.0f, 0.0f),
		                                          Vec2(0, -player->getContentSize().height * 0.2)));
	player->setPosition(Vec2(visibleSize.width * 0.15, visibleSize.height / 2));
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

void Wonderland::addCamera() {
	camera = Camera::create();
	this->addChild(camera);
	this->setCameraMask(1);
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
	auto clickListener = EventListenerMouse::create();
	clickListener->onMouseDown = CC_CALLBACK_1(Wonderland::mouseClick, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(clickListener, this);
}

void Wonderland::update(float f) {
	// 摄像头位置控制
	if (player->getPositionX() >= visibleSize.width / 2 && player->getPositionX() <= 3 * visibleSize.width / 2)
		camera->setPositionX(player->getPositionX());

	// 物理世界控制
	for (int i = 0; i < 3; ++i)
	{
		getScene()->getPhysicsWorld()->step(1 / 300.0f);
	}
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setGravity(Vec2(0, -visibleSize.height * 1.9140625f));
	// 玩家速度控制
	float offset = stone[0]->getContentSize().width * scale * 0.25;
	if (player->getPosition().x <= offset + player->getContentSize().width * scale / 2 && velocity < 0 ||
		player->getPosition().x >= 2 * visibleSize.width - offset - player->getContentSize().width * scale / 2 && velocity > 0)
		player->getPhysicsBody()->setVelocity(Vec2(0, player->getPhysicsBody()->getVelocity().y));
	else
		player->getPhysicsBody()->setVelocity(Vec2(velocity, player->getPhysicsBody()->getVelocity().y));

	// 瞄准器控制
	circle->setPosition(player->getPosition());
	static int count = 0;
	count++;
	if (count % 20 == 0)
	{
		circle->runAction(RotateTo::create(0.2f, -(player->getPosition() - mousePosition).getAngle() * 180 / 3.1415926 + 90));
	}

	// 判断hint
	if (count == 300)
	{
		count = 0;
		if (player->getPositionX() > hint[0]->getPositionX())
			hint[0]->runAction(FadeOut::create(1.0f));
		if (player->getPositionX() > hint[1]->getPositionX())
			hint[1]->runAction(FadeOut::create(1.0f));
		if (player->getPositionX() > hint[2]->getPositionX())
			hint[2]->runAction(FadeOut::create(1.0f));
	}

	// 添加铰链
	static bool initial = 1;
	if (initial)
	{
		auto upperBound = Sprite::create("stoneCenter.png");
		upperBound->setScale(scale, scale);
		upperBound->setPosition(visibleSize.width / 2, visibleSize.height);
		upperBound->setPhysicsBody(PhysicsBody::createBox(upperBound->getContentSize()));
		upperBound->getPhysicsBody()->setDynamic(false);
		upperBound->getPhysicsBody()->setCategoryBitmask(0xF0);
		upperBound->getPhysicsBody()->setCollisionBitmask(0xFF);
		upperBound->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(upperBound);

		Sprite* chain = Sprite::create("chain.png");
		chain->setScale(scale, scale);
		chain->setPhysicsBody(PhysicsBody::createBox(Size(10.0f, chain->getContentSize().height),
			                                         PhysicsMaterial(1.0f, 0.8f, 0.5f)));
		chain->setPosition(visibleSize.width / 2, upperBound->getBoundingBox().getMinY()- chain->getContentSize().height * scale / 2);
		// 设置掩码
		chain->getPhysicsBody()->setCategoryBitmask(0x0F);
		chain->getPhysicsBody()->setCollisionBitmask(0x0F);
		chain->getPhysicsBody()->setContactTestBitmask(0xFF);
		chain->setTag(4);
		this->addChild(chain);

		Sprite* chain_2 = Sprite::create("chain.png");
		chain_2->setScale(scale, scale);
		chain_2->setPhysicsBody(PhysicsBody::createBox(Size(10.0f, chain->getContentSize().height),
			                    PhysicsMaterial(1.0f, 0.2f, 0.5f)));
		chain_2->setPosition(visibleSize.width / 2, chain->getBoundingBox().getMinY() - chain_2->getContentSize().height * scale / 2);
		// 设置掩码
		chain_2->getPhysicsBody()->setCategoryBitmask(0xF0);
		chain_2->getPhysicsBody()->setCollisionBitmask(0xF0);
		chain_2->getPhysicsBody()->setContactTestBitmask(0xFF);
		chain_2->setTag(4);
		this->addChild(chain_2);

		box = Sprite::create("boxCrate_double.png");
		box->setScale(scale, scale);
		box->setPosition(visibleSize.width / 2, chain_2->getBoundingBox().getMinY() - box->getContentSize().height * scale / 2);
		box->setPhysicsBody(PhysicsBody::createBox(box->getContentSize(), PhysicsMaterial(0.3f, 0.2f, 0.8f)));
		box->getPhysicsBody()->setCategoryBitmask(0xFF);
		box->getPhysicsBody()->setCollisionBitmask(0x0F);
		box->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(box);

		isPlayerOnGround[2] = Sprite::create("isPlayerOnBox.png");
		float temp = scale * 1.15;
		isPlayerOnGround[2]->setScale(temp, temp);
		isPlayerOnGround[2]->setPosition(box->getPosition());
		isPlayerOnGround[2]->setOpacity(0);
		this->addChild(isPlayerOnGround[2]);

		auto fixedpoint_1 = PhysicsJointPin::construct(upperBound->getPhysicsBody(), chain->getPhysicsBody(),Vec2(upperBound->getPosition().x, upperBound->getBoundingBox().getMinY()));
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->addJoint(fixedpoint_1);
		connect	 = PhysicsJointPin::construct(chain->getPhysicsBody(), chain_2->getPhysicsBody(), Vec2(chain->getPosition().x, chain->getBoundingBox().getMinY()));
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->addJoint(connect);
		auto fixedpoint_2 = PhysicsJointPin::construct(chain_2->getPhysicsBody(), box->getPhysicsBody(), Vec2(box->getPosition().x, box->getBoundingBox().getMaxY()));
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->addJoint(fixedpoint_2);
	}
	initial = 0;

	if (box->getBoundingBox().intersectsRect(isPlayerOnGround[0]->getBoundingBox()) ||
		box->getBoundingBox().intersectsRect(isPlayerOnGround[1]->getBoundingBox()))
		isPlayerOnGround[2]->setPosition(box->getPosition());

	restartMenu->setPositionX(camera->getPosition().x - restartMenu->getContentSize().width * 1.9);

	if (player->getBoundingBox().intersectsRect(exit->getBoundingBox())) {
		Sprite* board;
		this->unschedule(schedule_selector(Wonderland::update));
		int score = 3 - diamond.size();
		if (score == 0) board = Sprite::create("greyBoard_win0.png");
		else if (score == 1) board = Sprite::create("greyBoard_win1.png");
		else if (score == 2) board = Sprite::create("greyBoard_win2.png");
		else board = Sprite::create("greyBoard_win3.png");
		board->setScale(scale + 0.5);
		board->setPosition(Vec2(camera->getPosition().x + board->getContentSize().width * scale * 0.04, camera->getPosition().y + 130 * scale));
		this->addChild(board, 3);
		restartMenu->setScale(scale, scale);
		restartMenu->setPosition(board->getPositionX(), board->getPositionY() - restartMenu->getContentSize().width / 3);
	}
}

bool Wonderland::onContactBegan(PhysicsContact& contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	auto sp1 = (Sprite*)bodyA->getNode();
	auto sp2 = (Sprite*)bodyB->getNode();

	// 子弹碰土块
	try
	{
		if (sp1 != NULL && sp2 != NULL)
		{
			if ((sp1->getTag() == 1 && sp2->getTag() == 2) || (sp1->getTag() == 2 && sp2->getTag() == 1))
			{
				if (sp1 != NULL)
				{
					sp1->removeFromParentAndCleanup(true);
					sp1 = NULL;
				}
				if (sp2 != NULL)
				{
					sp2->removeFromParentAndCleanup(true);
					sp2 = NULL;
				}
			}

			if (!isChainBroken && sp1 != NULL && sp2 != NULL) {
				if ((sp1->getTag() == 4 && sp2->getTag() == 2) || (sp1->getTag() == 2 && sp2->getTag() == 4)) {
					Director::getInstance()->getRunningScene()->getPhysicsWorld()->removeJoint(connect, true);
					if (sp1 != NULL)
						if (sp1->getTag() == 2) {
							sp1->removeFromParentAndCleanup(true);
							sp1 = NULL;
						}
					if (sp2 != NULL)
						if (sp2->getTag() == 2) {
							sp2->removeFromParentAndCleanup(true);
							sp2 = NULL;
						}
					isChainBroken = 1;
				}
			}
		}

		// player碰钻石
		if (sp1 != NULL && sp2 != NULL)
		{
			if ((sp1->getTag() == 0 && sp2->getTag() == 7) || (sp1->getTag() == 7 && sp2->getTag() == 0))
			{
				if (sp1 != NULL && sp1->getTag() == 7)
				{
					sp1->removeFromParentAndCleanup(true);
					diamond.erase(diamond.find(sp1));
					sp1 = NULL;
				}
				if (sp2 != NULL && sp2->getTag() == 7)
				{
					sp2->removeFromParentAndCleanup(true);
					diamond.erase(diamond.find(sp2));
					sp2 = NULL;
				}
			}
		}

		// 子弹碰除了关节、土块的别的物体
		if (sp1 != NULL && sp2 != NULL)
		{
			if ((sp1->getTag() == 2 && sp2->getTag() != 1 && sp2->getTag() != 4) ||
				(sp2->getTag() == 2 && sp1->getTag() != 1 && sp1->getTag() != 4))
			{
				if (sp1 != NULL && sp1->getTag() == 2)
				{
					sp1->removeFromParentAndCleanup(true);
					sp1 = NULL;
				}
				if (sp2 != NULL && sp2->getTag() == 2)
				{
					sp2->removeFromParentAndCleanup(true);
					sp2 = NULL;
				}
			}
		}
	}
	catch (exception err)
	{
		// 解决C++的异步出错
	}
	return true;
}

void Wonderland::mouseMove(Event* event) {
	EventMouse* e = (EventMouse*)event;
	mousePosition = Vec2(camera->getPositionX() - visibleSize.width / 2 + e->getCursorX(), e->getCursorY());
}

void Wonderland::mouseClick(Event* event) {
	// 解决摄像头导致的触发两次事件的问题
	static bool doubleClick = false;
	doubleClick = !doubleClick;
	if (doubleClick)
	{
		Sprite* bullet = Sprite::create("bullet.png");
		bullet->setScale(scale * 0.335);
		Vec2 temp = mousePosition - player->getPosition();
		temp = 0.15 * visibleSize.height * temp / temp.getLength();
		bullet->setPosition(player->getPosition() + temp);
		bullet->setPhysicsBody(PhysicsBody::createCircle(bullet->getContentSize().width * 0.35, PhysicsMaterial(1.0f, 1.0f, 0.0f)));
		bullet->getPhysicsBody()->setGravityEnable(false);
		bullet->getPhysicsBody()->setVelocity(temp * 8);
		// 子弹的Tag为2
		bullet->setTag(2);
		// 设置掩码
		bullet->getPhysicsBody()->setCategoryBitmask(0xFF);
		bullet->getPhysicsBody()->setCollisionBitmask(0xFF);
		bullet->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(bullet);
	}
}

void Wonderland::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	initaction();
	static char lastcid = 'D';
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		if (lastcid != 'A') {
			player->setFlipX(true);
		}
		lastcid = 'A';
		player->stopAllActions();
		player->runAction(RepeatForever::create(action_walk));
		velocity -= 0.390625 * visibleSize.height;
		keyCount++;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		if (lastcid != 'D') {
			player->setFlipX(false);
		}
		lastcid = 'D';
		player->stopAllActions();
		player->runAction(RepeatForever::create(action_walk));
		velocity += 0.390625 * visibleSize.height;
		keyCount++;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		if (player->getBoundingBox().intersectsRect(isPlayerOnGround[0]->getBoundingBox()) ||
			player->getBoundingBox().intersectsRect(isPlayerOnGround[1]->getBoundingBox()) ||
			player->getBoundingBox().intersectsRect(isPlayerOnGround[2]->getBoundingBox()))
			player->getPhysicsBody()->setVelocity(Vec2(player->getPhysicsBody()->getVelocity().x, visibleSize.height * 0.9375f));
		player->stopAllActions();
		player->runAction(action_jump);
		if (keyCount >= 0)
			player->runAction(Sequence::create(DelayTime::create(1.0f), action_walk, NULL));
		else
			player->runAction(Sequence::create(DelayTime::create(1.0f), action_stand, NULL));
		break;
	default:
		break;
	}
}

void Wonderland::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	initaction();
	switch (code)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		keyCount--;
		if (keyCount == 0)
		{
			player->stopAllActions();
			player->runAction(RepeatForever::create(action_stand));
		}
		velocity += 0.390625 * visibleSize.height;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_D:
		keyCount--;
		if (keyCount == 0)
		{
			player->stopAllActions();
			player->runAction(RepeatForever::create(action_stand));
		}
		velocity -= 0.390625 * visibleSize.height;
		break;
	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:
	case cocos2d::EventKeyboard::KeyCode::KEY_W:
		player->stopAllActions();
		if (velocity == 0)
			player->runAction(RepeatForever::create(action_stand));
		else
			player->runAction(RepeatForever::create(action_walk));
		break;
	default:
		break;
	}
}

void Wonderland::initaction() {
	//walk
	animation_walk = Animation::create();
	for (int i = 1; i < 3; i++)
	{
		char imageFileName[128];
		sprintf(imageFileName, "alienGreen_walk%d.png", i);
		animation_walk->addSpriteFrameWithFileName(imageFileName);
	}
	animation_walk->setDelayPerUnit(0.1f);
	animation_walk->setRestoreOriginalFrame(true);
	animation_walk->setLoops(1024);
	action_walk = Animate::create(animation_walk);

	// jump
	animation_jump = Animation::create();
	animation_jump->addSpriteFrameWithFileName("alienGreen_jump.png");
	animation_jump->setDelayPerUnit(1.0f);
	animation_jump->setRestoreOriginalFrame(true);
	action_jump = Animate::create(animation_jump);

	// stand
	animation_stand = Animation::create();
	animation_stand->addSpriteFrameWithFileName("alienGreen_walk0.png");
	animation_stand->setDelayPerUnit(10.0f);
	animation_stand->setRestoreOriginalFrame(true);
	action_stand = Animate::create(animation_stand);

	// hit
	animation_hit = Animation::create();
	animation_hit->addSpriteFrameWithFileName("alienGreen_hit.png");
	animation_hit->setDelayPerUnit(10.0f);
	animation_hit->setRestoreOriginalFrame(false);
	action_hit = Animate::create(animation_hit);
}

void Wonderland::win(bool isWin) {
	this->unschedule(schedule_selector(Wonderland::update));
	auto board = Sprite::create("greyBoard.png");
	board->setScale(scale + 0.2);
	board->setPosition(Vec2(camera->getPosition().x + 100 * scale, camera->getPosition().y + 130 * scale));
	this->addChild(board, 2);

	auto label = Label::createWithTTF("You Win!", "fonts\\Marker Felt.ttf", 48);
	label->setPosition(Vec2(camera->getPosition().x,
		camera->getPosition().y + 200 * scale));
	label->setScale(scale + 0.2);
	this->addChild(label, 3);

	auto goBack = MenuItemFont::create("Return",  CC_CALLBACK_1(Wonderland::goBack, this));
	goBack->setFontSize(20);
	goBack->setFontName("Marker Felt");
	goBack->setPosition(Vec2(camera->getPosition().x - 200 * scale,
		camera->getPosition().y + 100 * scale));
	auto goBackMenu = Menu::create(goBack, NULL);
	goBackMenu->setPosition(Vec2::ZERO);
	this->addChild(goBackMenu, 3);
}

void Wonderland::goBack(Ref* pSender) {
	auto gamescene = SelectScene::createScene();
	Director::getInstance()->replaceScene(TransitionSplitCols::create(0.5f, gamescene));
}
