#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
USING_NS_CC;

class BeginScene :public Layer {
public:
	void setPhysicsWorld(PhysicsWorld * world);
	static cocos2d::Scene* createScene();
	
	virtual bool init(PhysicsWorld* world);

	// implement the "static create()" method manually
	static BeginScene* create(PhysicsWorld* world);

private:
	Size visibleSize;
	double scale;
	PhysicsWorld* m_world;
	Sprite* player;
	Animate *action_walk;
	Animation *walk;
	Sprite* sign;
	Sprite* bee;
	bool left;

	void begin();
	void addPlayer();
	void update(float f);
	void addMouseListener();
	void mouseClick(Event* event);
	void mouseMove(Event* event);
	Vec2 mousePosition = Vec2::ZERO;
	void addBackground();
	void signIn(Ref *pSender, TouchEventType type);
	void signUp(Ref *pSender, TouchEventType type);
};