#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class LevelOne :public Layer {
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(int game);
	static PhysicsWorld* world;

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(LevelOne);

private:
	Camera* camera;
	Sprite* player;
	Sprite* circle;
	int velocity = 0;
	Sprite* ground[25];
	Sprite* fragileGround[100];
	Sprite* fragileWall[10];
	Sprite* bigStone;
	Sprite* saw;
	Sprite* chain[5];
	Sprite* isPlayerOnGround[16];
	Sprite* enemies[6];
	PhysicsWorld* m_world;
	Size visibleSize;
	Vec2 mousePosition = Vec2::ZERO;
	double scale;
	PhysicsJointPin* connect;
	bool isChainBroken = 0;
	Sprite* diamond[3];
	Sprite* exit;
	MenuItemImage* restartMenu;
	Menu* restart;
	bool initial;

	// ¶¯»­
	int keyCount = 0;
	Animation *animation_stand;
	Animate *action_stand;
	Animation *animation_walk;
	Animate *action_walk;
	Animation *animation_jump;
	Animate *action_jump;
	Animation *animation_hit;
	Animate *action_hit;

	Sequence *jumpSequence;
	CallFunc *callback;

	void preloadMusic();
	void playBgm();

	void addBackground();
	void addPlayer();
	void initaction();

	void addCamera();
	void goBack(Ref* pSender);

	void addContactListener();
	void addKeyboardListener();
	void addMouseListener();

	void update(float f);

	bool onContactBegan(PhysicsContact& contact);
	void mouseMove(Event* event);
	void mouseClick(Event* event);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	
	void addEnemies();
};