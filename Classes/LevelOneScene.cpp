#include "LevelOneScene.h"
#include <string>
using namespace std;
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

void LevelOne::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* LevelOne::createScene(int game) {
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -2940));
	scene->getPhysicsWorld()->setAutoStep(false);

	auto layer = LevelOne::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());

	scene->addChild(layer);
	return scene;
}

bool LevelOne::init() {
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	preloadMusic();
	playBgm();

	addBackground();
	addPlayer();
	addEnemies();
	addCamera();

	addContactListener();
	addKeyboardListener();
	addMouseListener();

	this->schedule(schedule_selector(LevelOne::update), 0.01f);

	return true;
}

void LevelOne::preloadMusic() {
	// TODO
}

void LevelOne::playBgm() {
	// TODO
}

void LevelOne::addBackground() {
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

	// 设置地块
	for (unsigned int i = 0; i < 30; i++)
	{
		ground[i] = Sprite::create("grassMid.png");
		ground[i]->setScale(scale, scale);
		ground[i]->setPosition(i * ground[i]->getContentSize().width * scale + ground[i]->getContentSize().width * scale / 2,
			ground[i]->getContentSize().height * scale / 2);
		ground[i]->setPhysicsBody(PhysicsBody::createBox(Size(ground[i]->getContentSize().width,
			ground[i]->getContentSize().height),
			PhysicsMaterial(100.0f, 1.0f, 0.5f)));
		ground[i]->getPhysicsBody()->setDynamic(false);
		// 地块的Tag为3  不能被打破
		ground[i]->setTag(3);
		// 设置掩码
		ground[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		ground[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		ground[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		if (i != 2 && i != 3 && i != 4) this->addChild(ground[i], 0);
	}

	// 设置易碎的土块
	for (unsigned int j = 0; j < 4; j++)
	{
		if (j == 0) {
			for (unsigned int i = 0; i < 6; ++i) {
				// 第一层
				fragileGround[i] = Sprite::create("grassCenter.png");
				fragileGround[i]->setScale(scale, scale);
				fragileGround[i]->setPosition(ground[7 + i]->getPosition() + Vec2(0, fragileGround[i]->getContentSize().height * scale));
				fragileGround[i]->setPhysicsBody(PhysicsBody::createBox(Size(fragileGround[i]->getContentSize().width,
					fragileGround[i]->getContentSize().height),
					PhysicsMaterial(100.0f, 1.0f, 0.5f)));
				fragileGround[i]->getPhysicsBody()->setDynamic(false);
				// 土块块的Tag为1 可以被打破
				fragileGround[i]->setTag(1);
				// 设置掩码
				fragileGround[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
				fragileGround[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
				fragileGround[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
				this->addChild(fragileGround[i], 0);
			}
		}
		else if (j == 1) {
			for (unsigned int i = 0; i < 6; ++i) {
				//  第二层
				fragileGround[i + 6] = Sprite::create("grassCenter.png");
				fragileGround[i + 6]->setScale(scale, scale);
				fragileGround[i + 6]->setPosition(ground[7 + i]->getPosition() + Vec2(0, fragileGround[i + 6]->getContentSize().height * 2 * scale));
				fragileGround[i + 6]->setPhysicsBody(PhysicsBody::createBox(Size(fragileGround[i]->getContentSize().width,
					fragileGround[i + 6]->getContentSize().height),
					PhysicsMaterial(100.0f, 1.0f, 0.5f)));
				fragileGround[i + 6]->getPhysicsBody()->setDynamic(false);
				// 土块块的Tag为1 可以被打破
				fragileGround[i + 6]->setTag(1);
				// 设置掩码
				fragileGround[i + 6]->getPhysicsBody()->setCategoryBitmask(0xFF);
				fragileGround[i + 6]->getPhysicsBody()->setCollisionBitmask(0xFF);
				fragileGround[i + 6]->getPhysicsBody()->setContactTestBitmask(0xFF);
				this->addChild(fragileGround[i + 6], 0);
			}
		}
		else if (j == 2) {
			for (unsigned int i = 0; i < 2; ++i) {
				//  第三层
				fragileGround[i + 12] = Sprite::create("grassCenter.png");
				fragileGround[i + 12]->setScale(scale, scale);
				fragileGround[i + 12]->setPosition(ground[9 + i]->getPosition() + Vec2(0, fragileGround[i + 12]->getContentSize().height * 3 * scale));
				fragileGround[i + 12]->setPhysicsBody(PhysicsBody::createBox(Size(fragileGround[i]->getContentSize().width,
					fragileGround[i + 12]->getContentSize().height),
					PhysicsMaterial(100.0f, 1.0f, 0.5f)));
				fragileGround[i + 12]->getPhysicsBody()->setDynamic(false);
				// 土块块的Tag为1 可以被打破
				fragileGround[i + 12]->setTag(1);
				// 设置掩码
				fragileGround[i + 12]->getPhysicsBody()->setCategoryBitmask(0xFF);
				fragileGround[i + 12]->getPhysicsBody()->setCollisionBitmask(0xFF);
				fragileGround[i + 12]->getPhysicsBody()->setContactTestBitmask(0xFF);
				this->addChild(fragileGround[i + 12], 0);
			}
		}
		else if (j == 3) {
			for (unsigned int i = 0; i < 2; ++i) {
				//  第四层
				fragileGround[i + 14] = Sprite::create("grassCenter.png");
				fragileGround[i + 14]->setScale(scale, scale);
				fragileGround[i + 14]->setPosition(ground[9 + i]->getPosition() + Vec2(0, fragileGround[i + 14]->getContentSize().height * 4 * scale));
				fragileGround[i + 14]->setPhysicsBody(PhysicsBody::createBox(Size(fragileGround[i]->getContentSize().width,
					fragileGround[i + 14]->getContentSize().height),
					PhysicsMaterial(100.0f, 1.0f, 0.5f)));
				fragileGround[i + 14]->getPhysicsBody()->setDynamic(false);
				// 土块块的Tag为1 可以被打破
				fragileGround[i + 14]->setTag(1);
				// 设置掩码
				fragileGround[i + 14]->getPhysicsBody()->setCategoryBitmask(0xFF);
				fragileGround[i + 14]->getPhysicsBody()->setCollisionBitmask(0xFF);
				fragileGround[i + 14]->getPhysicsBody()->setContactTestBitmask(0xFF);
				this->addChild(fragileGround[i + 14], 0);
			}
		}
	}

	//  大石块
	bigStone = Sprite::create("bigStone.png");
	bigStone->setScale(scale, scale);
	bigStone->setPosition(ground[8]->getPosition() - Vec2(ground[8]->getContentSize().width / 3, 0) + Vec2(0, ground[11]->getContentSize().height * 3.5 * scale));
	bigStone->setPhysicsBody(PhysicsBody::createBox(Size(bigStone->getContentSize().width,
		bigStone->getContentSize().height),
		PhysicsMaterial(100.0f, 0.0f, 0.5f)));
	// 大石块的Tag为3  不能被打破
	bigStone->setTag(3);
	// 设置掩码
	bigStone->getPhysicsBody()->setCategoryBitmask(0xF0);
	bigStone->getPhysicsBody()->setCollisionBitmask(0xF0);
	bigStone->getPhysicsBody()->setContactTestBitmask(0xFF);
	this->addChild(bigStone, 0);

	  //墙
	for (unsigned int i = 0; i < 2; i++)
	{
		fragileWall[i] = Sprite::create("grassCenter.png");
		fragileWall[i]->setScale(scale, scale);
		fragileWall[i]->setPosition(ground[16]->getPosition() - Vec2(-ground[0]->getContentSize().width * scale * 0.35, ground[0]->getContentSize().height * scale * 0.5) + Vec2(0, fragileWall[i]->getContentSize().height * (i + 1) * scale));
		fragileWall[i]->setPhysicsBody(PhysicsBody::createBox(Size(fragileWall[i]->getContentSize().width,
			fragileWall[i]->getContentSize().height),
			PhysicsMaterial(100.0f, 1.0f, 0.3f)));
		fragileWall[i]->getPhysicsBody()->setDynamic(false);
		// 土块块的Tag为1 可以被打破
		fragileWall[i]->setTag(1);
		// 设置掩码
		fragileWall[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		fragileWall[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		fragileWall[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(fragileWall[i], 0);
	}
}

	//怪物
void LevelOne::addEnemies() {
	for (int i = 0; i < 3; ++i) {
		enemies[i] = Sprite::create("barnacle.png");
		enemies[i]->setScale(scale, scale);
		enemies[i]->setPosition(ground[13 + i]->getPosition() + Vec2(0, ground[0]->getContentSize().height * scale));
		enemies[i]->setPhysicsBody(PhysicsBody::createBox(Size(enemies[i]->getContentSize().width,
			enemies[i]->getContentSize().height),
			PhysicsMaterial(1.0f, 1.0f, 0.3f),
			Vec2(0, -enemies[i]->getContentSize().height * 0.3)));
		enemies[i]->getPhysicsBody()->setDynamic(false);
		//怪物tag为5
		enemies[i]->setTag(5);
		// 设置掩码
		enemies[i]->getPhysicsBody()->setCategoryBitmask(0x0F);
		enemies[i]->getPhysicsBody()->setCollisionBitmask(0x0F);
		enemies[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(enemies[i], 0);
	}
}


void LevelOne::addPlayer() {
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

void LevelOne::addCamera() {
	camera = Camera::create();
	this->addChild(camera);
	this->setCameraMask(1);
}

void LevelOne::addContactListener() {
	auto contactBeginListener = EventListenerPhysicsContact::create();
	contactBeginListener->onContactBegin = CC_CALLBACK_1(LevelOne::onContactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(contactBeginListener, 1);
}

void LevelOne::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(LevelOne::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(LevelOne::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void LevelOne::addMouseListener() {
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(LevelOne::mouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	auto clickListener = EventListenerMouse::create();
	clickListener->onMouseDown = CC_CALLBACK_1(LevelOne::mouseClick, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(clickListener, this);
}

void LevelOne::update(float f) {
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
	float offset = ground[0]->getContentSize().width * scale * 0.25;
	if (player->getPosition().x <= offset + player->getContentSize().width * scale / 2 && velocity < 0 ||
		player->getPosition().x >= 2 * visibleSize.width - offset - player->getContentSize().width * scale / 2 && velocity > 0)
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
	static bool initial = 1;
	if (initial) {
		//  杆和球
		auto upperBound = Sprite::create("stoneCenter.png");
		upperBound->setScale(scale, scale);
		upperBound->setPosition(ground[15]->getPosition() + Vec2(0, ground[0]->getContentSize().height * 4.5 * scale));
		upperBound->setPhysicsBody(PhysicsBody::createBox(upperBound->getContentSize()));
		upperBound->getPhysicsBody()->setDynamic(false);
		upperBound->getPhysicsBody()->setCategoryBitmask(0xF0);
		upperBound->getPhysicsBody()->setCollisionBitmask(0xFF);
		upperBound->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(upperBound, 0);

		for (unsigned int i = 0; i < 3; ++i) {
			chain[i] = Sprite::create("chain.png");
			chain[i]->setScale(scale, scale);
			chain[i]->setPhysicsBody(PhysicsBody::createBox(Size(10.0f, chain[i]->getContentSize().height),
				PhysicsMaterial(1.0f, 0.0f, 0.0f)));
			if (i == 0) chain[i]->setPosition(upperBound->getPosition() + Vec2(chain[0]->getContentSize().width / 2 * scale, 0));
			else {
				chain[i]->setPosition(Vec2(chain[i - 1]->getBoundingBox().getMaxX() + chain[0]->getContentSize().width / 2 * scale, chain[i - 1]->getPositionY()));
			}
			chain[i]->setRotation(90.0);
			//chain[i]->getPhysicsBody()->setDynamic(false);
			//  链tag为4
			chain[i]->setTag(4);
			// 设置掩码
			chain[i]->getPhysicsBody()->setCategoryBitmask(0x0F);
			chain[i]->getPhysicsBody()->setCollisionBitmask(0x0F);
			chain[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
			this->addChild(chain[i], 0);
		}

		saw = Sprite::create("saw.png");
		saw->setScale(scale, scale);
		saw->setPosition(Vec2(chain[2]->getBoundingBox().getMaxX(), chain[2]->getPositionY()));
		saw->setPhysicsBody(PhysicsBody::createCircle(saw->getContentSize().height / 2., PhysicsMaterial(100.0f, 0.0f, 50.0f)));
		//saw->getPhysicsBody()->setDynamic(false);
		// 摆锤tag为6
		saw->setTag(6);
		saw->getPhysicsBody()->setCategoryBitmask(0xFF);
		saw->getPhysicsBody()->setCollisionBitmask(0x0F);
		saw->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(saw);

		// 添加关节
		auto fixedpoint_1 = PhysicsJointPin::construct(upperBound->getPhysicsBody(), chain[0]->getPhysicsBody(), Vec2(upperBound->getPositionX(), chain[0]->getBoundingBox().getMidY()));
		fixedpoint_1->setCollisionEnable(false);
		fixedpoint_1->setEnable(false);
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->addJoint(fixedpoint_1);
		for (unsigned int i = 0; i < 2; ++i) {
			connect = PhysicsJointPin::construct(chain[i]->getPhysicsBody(), chain[i + 1]->getPhysicsBody(), Vec2(chain[i]->getBoundingBox().getMaxX(), chain[i + 1]->getBoundingBox().getMidY()));
			connect->setCollisionEnable(false);
			connect->setEnable(false);
			Director::getInstance()->getRunningScene()->getPhysicsWorld()->addJoint(connect);
		}
		auto fixedpoint_2 = PhysicsJointPin::construct(chain[2]->getPhysicsBody(), saw->getPhysicsBody(), Vec2(chain[2]->getBoundingBox().getMaxX(), chain[2]->getPositionY()));
		fixedpoint_2->setCollisionEnable(false);
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->addJoint(fixedpoint_2);

		initial = 0;
	}
}

bool LevelOne::onContactBegan(PhysicsContact& contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	auto sp1 = (Sprite*)bodyA->getNode();
	auto sp2 = (Sprite*)bodyB->getNode();

	// 摆锤tag为6
	// 怪物tag为5
	// 链tag为4
	// 大石块的Tag为3
	// 地块tag为3
	// 子弹的Tag为2
	// 土块的Tag为1
	// 玩家的tag为0
	try
	{
		// 子弹碰土块
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
		}

		//// 摆锤碰怪物
		//if (sp1 != NULL && sp2 != NULL)
		//{
		//	if ((sp1->getTag() == 5 && sp2->getTag() == 6) || (sp1->getTag() == 6 && sp2->getTag() == 5))
		//	{
		//		if (sp1 != NULL && sp1->getTag() == 5)
		//		{
		//			sp1->removeFromParentAndCleanup(true);
		//			sp1 = NULL;
		//		}
		//		if (sp2 != NULL && sp2->getTag() == 5)
		//		{
		//			sp2->removeFromParentAndCleanup(true);
		//			sp2 = NULL;
		//		}
		//	}
		//}

		//  子弹碰关节
		if (!isChainBroken && sp1 != NULL && sp2 != NULL) {
			if ((sp1->getTag() == 4 && sp2->getTag() == 2) || (sp1->getTag() == 2 && sp2->getTag() == 4)) {
				if (sp1->getTag() == 4) Director::getInstance()->getRunningScene()->getPhysicsWorld()->removeJoint(sp1->getPhysicsBody()->getJoints()[0], true);
				else if (sp2->getTag() == 4) Director::getInstance()->getRunningScene()->getPhysicsWorld()->removeJoint(sp2->getPhysicsBody()->getJoints()[0], true);
				
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

		////  player碰怪物
		//if (sp1 != NULL && sp2 != NULL)
		//{
		//	if ((sp1->getTag() == 0 && sp2->getTag() == 5) || (sp1->getTag() == 5 && sp2->getTag() == 0))
		//	{
		//		if (sp1 != NULL)
		//		{
		//			sp1->removeFromParentAndCleanup(true);
		//			sp1 = NULL;
		//		}
		//		if (sp2 != NULL)
		//		{
		//			sp2->removeFromParentAndCleanup(true);
		//			sp2 = NULL;
		//		}
		//	}
		//}


		////  摆锤碰player
		//if (sp1 != NULL && sp2 != NULL)
		//{
		//	if ((sp1->getTag() == 0 && sp2->getTag() == 6) || (sp1->getTag() == 6 && sp2->getTag() == 0))
		//	{
		//		if (sp1 != NULL && sp1->getTag() == 0)
		//		{
		//			sp1->removeFromParentAndCleanup(true);
		//			sp1 = NULL;
		//		}
		//		if (sp2 != NULL && sp2->getTag() == 0)
		//		{
		//			sp2->removeFromParentAndCleanup(true);
		//			sp2 = NULL;
		//		}
		//	}
		//}

	}
	catch (exception err)
	{
		// 解决C++的异步出错
	}
	return true;
}

void LevelOne::mouseMove(Event* event) {
	EventMouse* e = (EventMouse*)event;
	mousePosition = Vec2(camera->getPositionX() - visibleSize.width / 2 + e->getCursorX(), e->getCursorY());
}

void LevelOne::mouseClick(Event* event) {
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

void LevelOne::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
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
		CCLOG("%f", player->getPhysicsBody()->getVelocity().y);
		if ((int)player->getPhysicsBody()->getVelocity().y == 0) {
			player->getPhysicsBody()->setVelocity(Vec2(player->getPhysicsBody()->getVelocity().x, visibleSize.height * 0.9375f));
		}
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

void LevelOne::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
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

void LevelOne::initaction() {
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


