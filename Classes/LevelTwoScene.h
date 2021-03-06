#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class LevelTwo :public Layer {
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(int game);
	static PhysicsWorld* world;

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(LevelTwo);

private:
	Camera* camera;
	Sprite* player;
	Sprite* circle;
	int velocity = 0;
	Sprite* ground[60];
	Sprite* isPlayerOnGround[16];
	Sprite* fragileGround[100];
	Sprite* fragileWall[10];
	Sprite* platform[2];
	Sprite* platform2[4];
	Sprite* enemies[6];
	Sprite* enemies2[5];
	Sprite* bigStone;
	Vector<Sprite *> bullets;
	Sprite* diamond[3];
	PhysicsWorld* m_world;
	Size visibleSize;
	Vec2 mousePosition = Vec2::ZERO;
	double scale;
	int count = 0;
	double cnt = 0, dir = 0.8;
	bool doubleClick = false;
	char lastcid = 'D';
	Sprite* exit;
	MenuItemImage* restartMenu;
	Menu* restart;

	// ����
	int keyCount = 0;
	Animation *animation_stand;
	Animate *action_stand;
	Animation *animation_walk;
	Animate *action_walk;
	Animation *animation_jump;
	Animate *action_jump;
	Animation *animation_hit;
	Animate *action_hit;

	void preloadMusic();
	void playBgm();

	void addBackground();
	void addPlayer();
	void initaction();

	void addCamera();

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
	void bulletRemovement();
};