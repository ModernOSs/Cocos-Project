#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class Wonderland :public Layer {
public:
	void setPhysicsWorld(PhysicsWorld * world);
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(Wonderland);

private:
    //“Ù¿÷º”‘ÿ”Î≤•∑≈
    Sprite* player;
	Sprite* circle;
	int velocity = 0;
	Sprite* ground[25];
	Sprite* isPlayerOnGround;
	PhysicsWorld* m_world;
	Size visibleSize;
	Vec2 mousePosition = Vec2::ZERO;
	double scale;

    void preloadMusic();
    void playBgm();

    void addBackground();
	void addPlayer();

	void addContactListener();
	void addKeyboardListener();
	void addMouseListener();

	void update(float f);

	bool onContactBegan(PhysicsContact& contact);
	void mouseMove(Event* event);
	void mouseClick(Event* event);
    void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
};