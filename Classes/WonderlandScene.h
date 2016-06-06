#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class Wonderland :public Layer {
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int game);
	static PhysicsWorld* world;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Wonderland);

private:
	Camera* camera;
    Sprite* player;
	Sprite* circle;
	int velocity = 0;
	Sprite* ground[25];
	Sprite* stone[11];
	Sprite* box;
	Sprite* isPlayerOnGround[16];
	PhysicsWorld* m_world;
	Size visibleSize;
	Vec2 mousePosition = Vec2::ZERO;
	double scale;
	PhysicsJointPin* connect;
	bool isChainBroken = 0;

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
	void win(bool isWin);
	void goBack(Ref* pSender);

	bool onContactBegan(PhysicsContact& contact);
	void mouseMove(Event* event);
	void mouseClick(Event* event);
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
};