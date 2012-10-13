/*
 * Trooper.cpp
 *
 *  Created on: 13.10.2012
 *      Author: jjay
 */

#include "Trooper.h"
#include <iostream>
USING_NS_CC;

Trooper::Trooper() {
	m_current_sprite = CCSprite::spriteWithFile("troop_falling_90x60.png");
	m_is_falling = true;
	CCNode();
	addChild(m_current_sprite);

}
Trooper* Trooper::trooper(){
	Trooper* trooper = new Trooper();
	trooper->autorelease();
	return trooper;
}

void Trooper::onEnter() {
	cout << "onEnter\n";
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
	CCNode::onEnter();
	cout << "exit from onEnter\n";
}

void Trooper::onExit() {
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool Trooper::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
	if (!m_is_falling){
		return false;
	}
	CCPoint point = touch->locationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);
	bool touched = CCRect::CCRectContainsPoint(getBoundingBox(), point);
	//CCPoint p;
	//cout << "Touch began" << touched << " " << point.x << " " << point.y << "\n";
	if (touched){
		removeChild(m_current_sprite, true);
		//m_falling_sprite->release();
		CCSprite * flying = CCSprite::spriteWithFile("troop_01_70x140.png");
		m_current_sprite = flying;
		addChild(flying);
		stopAllActions();
		CCActionInterval* action = CCMoveBy::actionWithDuration(1, CCPointMake(0, -50));
		runAction(CCRepeatForever::actionWithAction(action));
		m_is_falling = false;
	}
	return touched;
}

void Trooper::ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {

}

void Trooper::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {

}

cocos2d::CCRect Trooper::getBoundingBox() {
	return CCRectMake(
		getPosition().x - (m_current_sprite->getTextureRect().size.width/2) * m_current_sprite->getScaleX(),
		getPosition().y - (m_current_sprite->getTextureRect().size.height/2) * m_current_sprite->getScaleY(),
		m_current_sprite->getTextureRect().size.width * m_current_sprite->getScaleX() ,
		m_current_sprite->getTextureRect().size.height * m_current_sprite->getScaleY()
	);
}
