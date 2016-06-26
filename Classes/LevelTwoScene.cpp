#include "LevelTwoScene.h"
#include <string>
using namespace std;
#include "SelectScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

void LevelTwo::setPhysicsWorld(PhysicsWorld* world) { m_world = world; }

Scene* LevelTwo::createScene(int game) {
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -2940));
	scene->getPhysicsWorld()->setAutoStep(false);
	auto layer = LevelTwo::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool LevelTwo::init() {
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	preloadMusic();
	playBgm();

	addCamera();
	addBackground();
	addPlayer();
	addEnemies();

	addContactListener();
	addKeyboardListener();
	addMouseListener();

	this->schedule(schedule_selector(LevelTwo::update), 0.01f);
	return true;
}

void LevelTwo::preloadMusic() {
	auto autio = SimpleAudioEngine::getInstance();
	autio->preloadEffect("music/win.wav");
	autio->preloadEffect("music/diamond.wav");
	autio->preloadEffect("music/bullet.wav");
	autio->preloadEffect("music/jump.mp3");
	autio->preloadBackgroundMusic("music/bgm.mp3");
}

void LevelTwo::playBgm() {
	auto autio = SimpleAudioEngine::getInstance();
	autio->playBackgroundMusic("music/bgm.mp3", true);
}

void LevelTwo::addBackground() {
	// 设置背景
	Sprite* background[4];
	for (unsigned int i = 0; i < 4; i++)
	{
		background[i] = Sprite::create("colored_desert.png");
		scale = visibleSize.height / background[i]->getContentSize().height;
		background[i]->setScale(scale, scale);
		background[i]->setPosition(i * visibleSize.height + visibleSize.height / 2, visibleSize.height / 2);
		this->addChild(background[i], 0);
	}

	// 设置地块
	for (unsigned int i = 0; i < 50; i++)
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
		if (!(i >= 2 && i <= 6) && !(i >= 9 && i <= 24))
		this->addChild(ground[i], 0);
	}

	// 上下移动的平台
	for (unsigned int i = 0; i < 2; ++i) {
		platform[i] = Sprite::create("bridge.png");
		platform[i]->setScale(scale);
		if (i == 0) {
			platform[i]->setPosition(ground[3 + i]->getPosition() + Vec2(ground[0]->getContentSize().width * scale, ground[0]->getContentSize().height * 0.5 * scale));
		}
		else if (i == 1) {
			platform[i]->setPosition(ground[19 + i]->getPosition() + Vec2(ground[0]->getContentSize().width * scale, ground[0]->getContentSize().height * 4 * scale));
		}
		platform[i]->setPhysicsBody(PhysicsBody::createBox(Size(platform[i]->getContentSize().width,
			platform[i]->getContentSize().height * 50.0 / 128.0),
			PhysicsMaterial(100.0f, 1.0f, 0.5f),
			Vec2(0, platform[i]->getContentSize().height * 0.3)));
		platform[i]->getPhysicsBody()->setDynamic(false);
		//  平台tag为3
		platform[i]->setTag(3);
		// 设置掩码
		platform[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		platform[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		platform[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(platform[i], 0);

		isPlayerOnGround[i] = Sprite::create("isPlayerOnBox.png");
		isPlayerOnGround[i]->setScale(scale * 2, scale);
		isPlayerOnGround[i]->setPosition(platform[i]->getPosition() + Vec2(0, platform[i]->getContentSize().height * scale * 0.2));

		  isPlayerOnGround[i]->setOpacity(0);
		this->addChild(isPlayerOnGround[i], 0);
	}

	//  左右移动的平台
	for (unsigned int i = 0; i < 4; ++i) {
		platform2[i] = Sprite::create("bridgeA.png");
		platform2[i]->setScale(scale);
		if (i >= 0 && i < 2) {
			platform2[i]->setPosition(ground[10 + i]->getPosition() + Vec2(ground[0]->getContentSize().width * scale * 0.5, ground[0]->getContentSize().height * scale * 2));
		}
		else if (i >= 2 && i < 4) {
			platform2[i]->setPosition(ground[11 + i]->getPosition() + Vec2(ground[0]->getContentSize().width * scale * 0.5, ground[0]->getContentSize().height * scale * 2));
		}
		platform2[i]->setPhysicsBody(PhysicsBody::createBox(Size(platform2[i]->getContentSize().width,
			platform2[i]->getContentSize().height * 50.0 / 128.0),
			PhysicsMaterial(100.0f, 1.0f, 0.5f),
			Vec2(0, platform2[i]->getContentSize().height * 0.3)));
		platform2[i]->getPhysicsBody()->setDynamic(false);
		// 平台tag为3
		platform2[i]->setTag(3);
		// 设置掩码
		platform2[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		platform2[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		platform2[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(platform2[i], 0);
	}

	//  洞穴
	for (unsigned int i = 0; i < 5; i++)
	{
		fragileWall[i] = Sprite::create("grassCenter.png");
		fragileWall[i]->setScale(scale, scale);
		fragileWall[i]->setPosition(ground[28]->getPosition() + Vec2(0, fragileWall[i]->getContentSize().height * (i + 1) * scale));
		fragileWall[i]->setPhysicsBody(PhysicsBody::createBox(Size(fragileWall[i]->getContentSize().width,
			fragileWall[i]->getContentSize().height),
			PhysicsMaterial(100.0f, 0.0f, 0.3f)));
		fragileWall[i]->getPhysicsBody()->setDynamic(false);
		// 土块块的Tag为1 可以被打破
		fragileWall[i]->setTag(1);
		// 设置掩码
		fragileWall[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		fragileWall[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		fragileWall[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(fragileWall[i], 0);
	}
	for (unsigned int i = 0; i < 2; ++i) {
		fragileGround[i] = Sprite::create("grassCenter.png");
		fragileGround[i]->setScale(scale, scale);
		fragileGround[i]->setPosition(ground[26 + i]->getPosition() + Vec2(0, fragileGround[i]->getContentSize().height * 4 * scale));
		fragileGround[i]->setPhysicsBody(PhysicsBody::createBox(Size(fragileGround[i]->getContentSize().width,
			fragileGround[i]->getContentSize().height),
			PhysicsMaterial(100.0f, 0.0f, 0.5f)));
		fragileGround[i]->getPhysicsBody()->setDynamic(false);
		// 土块块的Tag为1 可以被打破
		fragileGround[i]->setTag(1);
		// 设置掩码
		fragileGround[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		fragileGround[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		fragileGround[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(fragileGround[i], 0);
	}
	//  大石块
	bigStone = Sprite::create("bigStone.png");
	bigStone->setScale(scale);
	bigStone->setPosition(Vec2((ground[26]->getPositionX() + ground[27]->getPositionX()) / 2,
		(fragileWall[4]->getBoundingBox().getMaxY())));
	bigStone->setPhysicsBody(PhysicsBody::createBox(Size(bigStone->getContentSize().width,
		bigStone->getContentSize().height),
		PhysicsMaterial(100.0f, 0.0f, 0.5f)));
	// 大石块的Tag为7  不能被打破
	bigStone->setTag(9);
	// 设置掩码
	bigStone->getPhysicsBody()->setCategoryBitmask(0xFF);
	bigStone->getPhysicsBody()->setCollisionBitmask(0xFF);
	bigStone->getPhysicsBody()->setContactTestBitmask(0xFF);
	this->addChild(bigStone, 0);

	//添加钻石
	for (int i = 0; i < 3; i++) {
		auto dia = Sprite::create("HUD//hudJewel_blue.png");
		dia->setPhysicsBody(PhysicsBody::createBox(Size(dia->getContentSize().width * 0.8,
			dia->getContentSize().height * 0.8),
			PhysicsMaterial(0.0f, 0.0f, 0.0f),
			Vec2(0, -dia->getContentSize().height * 0.1)));
		dia->getPhysicsBody()->setDynamic(false);
		dia->setTag(7);
		dia->getPhysicsBody()->setCategoryBitmask(0xF0);
		dia->getPhysicsBody()->setCollisionBitmask(0xF0);
		dia->getPhysicsBody()->setContactTestBitmask(0xFF);
		dia->setScale(scale, scale);
		diamond[i] = dia;
	}
	diamond[0]->setPosition(ground[5]->getPositionX() + ground[0]->getContentSize().width * 0.5, ground[0]->getContentSize().height * scale * 7.5);
	this->addChild(diamond[0], 2);
	diamond[1]->setPosition((ground[12]->getPositionX() + ground[13]->getPositionX()) / 2, ground[0]->getContentSize().height * scale * 6.5);
	this->addChild(diamond[1], 2);
	diamond[2]->setPosition(ground[19]->getPositionX(), ground[0]->getContentSize().height * scale * 3);
	this->addChild(diamond[2], 2);

	//添加出口
	exit = Sprite::create("signExit.png");
	exit->setScale(scale, scale);
	exit->setPosition(29 * ground[0]->getContentSize().width * scale + ground[0]->getContentSize().width * scale / 2,
		ground[0]->getContentSize().height * scale * 1.5);

	this->addChild(exit, 3);

	//添加重启按钮
	restartMenu = MenuItemImage::create("restart.png", "restart.png", [&](Ref* pSender) { auto gamescene = SelectScene::createScene(); Director::getInstance()->replaceScene(gamescene); });
	restartMenu->setScale(scale - 0.2, scale - 0.2);
	restartMenu->setPosition(camera->getPosition().x - restartMenu->getContentSize().width * 1.9, visibleSize.height - restartMenu->getContentSize().height / 4.3);
	restart = Menu::createWithItem(restartMenu);
	restart->setPosition(Vec2::ZERO);
	this->addChild(restart, 4);
}

void LevelTwo::addEnemies() {
	for (int i = 0; i < 2; ++i) {
		enemies[i] = Sprite::create("barnacle0.png");
		enemies[i]->setScale(scale, scale);
		enemies[i]->setPosition(Vec2(fragileGround[i]->getPositionX(), fragileWall[0]->getPositionY()));
		enemies[i]->setPhysicsBody(PhysicsBody::createBox(Size(enemies[i]->getContentSize().width,
			enemies[i]->getContentSize().height),
			PhysicsMaterial(1.0f, 1.0f, 0.3f),
			Vec2(0, -enemies[i]->getContentSize().height * 0.3)));
		enemies[i]->getPhysicsBody()->setDynamic(false);
		//怪物tag为5
		enemies[i]->setTag(5);
		// 设置掩码
		enemies[i]->getPhysicsBody()->setCategoryBitmask(0xFF);
		enemies[i]->getPhysicsBody()->setCollisionBitmask(0xFF);
		enemies[i]->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(enemies[i], 0);
	}

	// 怪物1动画
	Animation *animation_monster_move;
	Animate *monster_move;
	animation_monster_move = Animation::create();
	for (int i = 0; i < 2; i++)
	{
		char imageFile[128];
		sprintf(imageFile, "barnacle%d.png", i);
		animation_monster_move->addSpriteFrameWithFileName(imageFile);
	}
	animation_monster_move->setDelayPerUnit(0.5f);
	animation_monster_move->setLoops(1024);
	monster_move = Animate::create(animation_monster_move);

	enemies[0]->runAction(monster_move);
	enemies[1]->runAction(monster_move->clone());


	//  怪物2
	enemies2[0] = Sprite::create("slimeBlock.png");
	enemies2[0]->setScale(scale);
	enemies2[0]->setPosition(ground[7]->getPosition() + Vec2(ground[0]->getContentSize().width * scale * 0.5, ground[0]->getContentSize().height * scale));
	enemies2[0]->setTag(5);
	enemies2[0]->setPhysicsBody(PhysicsBody::createBox(Size(enemies2[0]->getContentSize().width * 0.8,
		enemies2[0]->getContentSize().height * 0.78),
		PhysicsMaterial(1.0f, 1.0f, 0.3f),
		Vec2(0, -enemies2[0]->getContentSize().height * 0.13)));
	enemies2[0]->getPhysicsBody()->setDynamic(false);
	enemies2[0]->setTag(8);
	// 设置掩码
	enemies2[0]->getPhysicsBody()->setCategoryBitmask(0xFF);
	enemies2[0]->getPhysicsBody()->setCollisionBitmask(0xFF);
	enemies2[0]->getPhysicsBody()->setContactTestBitmask(0xFF);
	this->addChild(enemies2[0], 0);
	
	//怪物2动画
	Animation *animation_monster_move2;
	Animate *monster_move2;
	animation_monster_move2 = Animation::create();
	for (int i = 0; i < 2; i++)
	{
		char imageFile[128];
		sprintf(imageFile, "slimeBlock%d.png", i);
		animation_monster_move2->addSpriteFrameWithFileName(imageFile);
	}
	animation_monster_move2->setDelayPerUnit(1.8f);
	animation_monster_move2->setLoops(1024);
	monster_move2 = Animate::create(animation_monster_move2);

	ActionInterval *jump = JumpBy::create(1.8f, Vec2(0, 0), ground[0]->getContentSize().height * scale * 1.7, 1);
	enemies2[0]->runAction(RepeatForever::create(jump));

	enemies2[0]->runAction(monster_move2);
}

void LevelTwo::bulletRemovement() {
	if (bullets.size() != 0) {
		(bullets.at(0))->removeFromParentAndCleanup(true);
		bullets.erase(0);
	}
}

void LevelTwo::addPlayer() {
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
	player->getPhysicsBody()->setCategoryBitmask(0x0F);
	player->getPhysicsBody()->setCollisionBitmask(0x0F);
	player->getPhysicsBody()->setContactTestBitmask(0xFF);
	this->addChild(circle);
	this->addChild(player);
}

void LevelTwo::addCamera() {
	camera = Camera::create();
	this->addChild(camera);
	this->setCameraMask(1);
}

void LevelTwo::addContactListener() {
	auto contactBeginListener = EventListenerPhysicsContact::create();
	contactBeginListener->onContactBegin = CC_CALLBACK_1(LevelTwo::onContactBegan, this);
	_eventDispatcher->addEventListenerWithFixedPriority(contactBeginListener, 1);
}

void LevelTwo::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(LevelTwo::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(LevelTwo::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void LevelTwo::addMouseListener() {
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(LevelTwo::mouseMove, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	auto clickListener = EventListenerMouse::create();
	clickListener->onMouseDown = CC_CALLBACK_1(LevelTwo::mouseClick, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(clickListener, this);
}

void LevelTwo::update(float f) {
	// 摄像头位置控制
	if (player->getPositionX() >= visibleSize.width / 2 && player->getPositionX() <= 4 * visibleSize.width / 2)
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
		player->getPosition().x >= 2.5 * visibleSize.width - offset - player->getContentSize().width * scale / 2 && velocity > 0)
		player->getPhysicsBody()->setVelocity(Vec2(0, player->getPhysicsBody()->getVelocity().y));
	else
		player->getPhysicsBody()->setVelocity(Vec2(velocity, player->getPhysicsBody()->getVelocity().y));

	// 判断出界
	if (player->getPositionY() < 0)
		Director::getInstance()->replaceScene(LevelTwo::createScene(0));

	// 瞄准器控制
	circle->setPosition(player->getPosition());
	static int count = 0;
	count++;
	if (count == 20)
	{
		count = 0;
		circle->runAction(RotateTo::create(0.2f, -(player->getPosition() - mousePosition).getAngle() * 180 / 3.1415926 + 90));
	}

	//平台移动
	/*static double cnt = 0;
	static double dir = 0.8;*/
	cnt += f;
	if (cnt > 2.5) {
		cnt = 0.0;
		dir = -dir;
	}

	platform[0]->setPosition(platform[0]->getPosition() + Vec2(0, dir));
	platform[1]->setPosition(platform[1]->getPosition() + Vec2(0, -dir));
	isPlayerOnGround[0]->setPosition(isPlayerOnGround[0]->getPosition() + Vec2(0, dir));
	isPlayerOnGround[1]->setPosition(isPlayerOnGround[1]->getPosition() + Vec2(0, -dir));

	for (int i = 0; i < 2; ++i) {
		platform2[i]->setPosition(platform2[i]->getPosition() + Vec2(-dir, 0));
		platform2[i + 2]->setPosition(platform2[i + 2]->getPosition() + Vec2(dir, 0));
	}

	restartMenu->setPositionX(camera->getPosition().x - restartMenu->getContentSize().width * 1.9);

	if (player->getBoundingBox().intersectsRect(exit->getBoundingBox())) {
		auto autio = SimpleAudioEngine::getInstance();
		autio->playEffect("music/win.wav", false, 1.0f, 1.0f, 1.0f);
		Sprite* board;
		int score = 0;
		for (int i = 0; i < 3; i++)
			if (diamond[i] == NULL)
				score++;
		if (score == 0) board = Sprite::create("greyBoard_win0.png");
		else if (score == 1) board = Sprite::create("greyBoard_win1.png");
		else if (score == 2) board = Sprite::create("greyBoard_win2.png");
		else board = Sprite::create("greyBoard_win3.png");
		board->setScale(scale + 0.5);
		board->setPosition(Vec2(camera->getPosition().x + board->getContentSize().width * scale * 0.04, camera->getPosition().y + 130 * scale));
		this->addChild(board, 3);
		restartMenu->setScale(scale, scale);
		restartMenu->setPosition(board->getPositionX(), board->getPositionY() - restartMenu->getContentSize().width / 3);
		this->unschedule(schedule_selector(LevelTwo::update));
	}
}

bool LevelTwo::onContactBegan(PhysicsContact& contact) {
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();
	auto sp1 = (Sprite*)bodyA->getNode();
	auto sp2 = (Sprite*)bodyB->getNode();
	
	// 大石块的Tag为9
	// 怪物2tag为8
	// 钻石tag为7
	// 怪物1tag为5
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

		// 石块碰怪物1
		if (sp1 != NULL && sp2 != NULL)
		{
			if ((sp1->getTag() == 5 && sp2->getTag() == 9) || (sp1->getTag() == 9 && sp2->getTag() == 5))
			{
				if (sp1 != NULL && sp1->getTag() == 5)
				{
					sp1->removeFromParentAndCleanup(true);
					sp1 = NULL;
				}
				if (sp2 != NULL && sp2->getTag() == 5)
				{
					sp2->removeFromParentAndCleanup(true);
					sp2 = NULL;
				}
			}
		}
		// player碰怪物2
		if (sp1 != NULL && sp2 != NULL)
		{
			if ((sp1->getTag() == 8 && sp2->getTag() == 0) || (sp1->getTag() == 0 && sp2->getTag() == 8))
			{
				Director::getInstance()->replaceScene(LevelTwo::createScene(0));
			}
		}

		// player碰怪物1
		if (sp1 != NULL && sp2 != NULL)
		{
			if ((sp1->getTag() ==5 && sp2->getTag() == 0) || (sp1->getTag() == 0 && sp2->getTag() == 5))
			{
				Director::getInstance()->replaceScene(LevelTwo::createScene(0));
			}
		}

		//  player碰钻石
		if (sp1 != NULL && sp2 != NULL)
		{
			if ((sp1->getTag() == 0 && sp2->getTag() == 7) || (sp1->getTag() == 7 && sp2->getTag() == 0))
			{
				auto autio = SimpleAudioEngine::getInstance();
				autio->playEffect("music/diamond.wav", false, 1.0f, 1.0f, 1.0f);
				if (sp1 != NULL && sp1->getTag() == 7)
				{
					sp1->removeFromParentAndCleanup(true);
					sp1 = NULL;
				}
				if (sp2 != NULL && sp2->getTag() == 7)
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

void LevelTwo::mouseMove(Event* event) {
	EventMouse* e = (EventMouse*)event;
	mousePosition = Vec2(camera->getPositionX() - visibleSize.width / 2 + e->getCursorX(), e->getCursorY());
}

void LevelTwo::mouseClick(Event* event) {
	// 解决摄像头导致的触发两次事件的问题
	//static bool doubleClick = false;
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
		auto autio = SimpleAudioEngine::getInstance();
		autio->playEffect("music/bullet.wav", false, 1.0f, 1.0f, 1.0f);
		bullet->getPhysicsBody()->setVelocity(temp * 8);
		// 子弹的Tag为2
		bullet->setTag(2);
		// 设置掩码
		bullet->getPhysicsBody()->setCategoryBitmask(0xFF);
		bullet->getPhysicsBody()->setCollisionBitmask(0xFF);
		bullet->getPhysicsBody()->setContactTestBitmask(0xFF);
		this->addChild(bullet);
		bullets.pushBack(bullet);
		auto bulletFadeOut = FadeOut::create(0.5f);
		auto callback = CallFunc::create(this, callfunc_selector(LevelTwo::bulletRemovement));
		auto sequence = Sequence::create(DelayTime::create(1.0f), bulletFadeOut, callback, NULL);
		bullet->runAction(sequence);
	}
}

void LevelTwo::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	initaction();
	//static char lastcid = 'D';
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
		CCLOG("%lf", player->getPositionX());
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
		SimpleAudioEngine::getInstance()->playEffect("music/jump.mp3", false, 1.0f, 1.0f, 1.0f);
		if ((int)player->getPhysicsBody()->getVelocity().y == 0 ||
			player->getBoundingBox().intersectsRect(isPlayerOnGround[0]->getBoundingBox()) ||
			player->getBoundingBox().intersectsRect(isPlayerOnGround[1]->getBoundingBox())) {
			player->getPhysicsBody()->setVelocity(Vec2(player->getPhysicsBody()->getVelocity().x, visibleSize.height * 0.98f));//  0.9375f
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

void LevelTwo::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
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

void LevelTwo::initaction() {
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


