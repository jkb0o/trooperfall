/*
 * Trooper.cpp
 *
 *  Created on: 13.10.2012
 *      Author: jjay
 */

#include "Trooper.h"
#include <iostream>
#include "TFAnimationCache.h"
#include "CCTouchDispatcher.h"
#include "CCDrawingPrimitives.h"
USING_NS_CC;

Trooper::Trooper() {
	m_current_sprite = NULL;
	m_animate_action = NULL;
	m_falling_action = NULL;
	m_state = Falling;
	CCNode();
}
Trooper* Trooper::trooper(){
	Trooper* trooper = new Trooper();
	trooper->autorelease();
	return trooper;
}

void Trooper::onEnter() {
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
	CCNode::onEnter();
}

void Trooper::onExit() {
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

bool Trooper::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
	if (m_state == Plane){
		return false;
	}
	CCPoint point = touch->locationInView();
	point = CCDirector::sharedDirector()->convertToGL(point);
	bool touched = CCRect::CCRectContainsPoint(getBoundingBox(), point);
	//CCPoint p;
	//cout << "Touch began" << touched << " " << point.x << " " << point.y << "\n";
	if (touched && m_state == Falling){
		this->animate("big_parafallon", "big_parafall", NULL);
		m_state = SmallOpening;
		return touched;
	}

	if ( touched && m_state == SmallOpened ) {
		stopAllActions();
		CCActionInterval* action = CCMoveBy::actionWithDuration(1, CCPointMake(0, -50));
		runAction(CCRepeatForever::actionWithAction(action));
		m_state = Plane;
		this->animate("big_paraon", "big_plane", NULL);

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

void Trooper::draw(){
	if (m_current_sprite==NULL){
		return;
	}
	return;
	CCRect box = CCRectMake(
			-(m_current_sprite->getTextureRect().size.width/2) * m_current_sprite->getScaleX(),
			-(m_current_sprite->getTextureRect().size.height/2) * m_current_sprite->getScaleY(),
			m_current_sprite->getTextureRect().size.width * m_current_sprite->getScaleX() ,
			m_current_sprite->getTextureRect().size.height * m_current_sprite->getScaleY()
	);
	const CCPoint p1 = CCPoint(box.origin.x, box.origin.y);
	const CCPoint p2 = CCPoint(box.origin.x+box.size.width, box.origin.y);
	const CCPoint p3 = CCPoint(box.origin.x+box.size.width, box.origin.y+box.size.height);
	const CCPoint p4 = CCPoint(box.origin.x, box.origin.y+box.size.height);
	ccDrawLine(p1, p2);
	ccDrawLine(p2, p3);
	ccDrawLine(p3, p4);
	ccDrawLine(p4, p1);
	CCNode::draw();
}

void Trooper::onAnimationStart(CCObject* animationName){
	this->onAnimationStart( ( (CCString*)animationName )->toStdString().c_str());
}

void Trooper::onAnimationStart(const char * animationName){
	if (std::string(animationName).compare("big_parafall")==0){
		m_state = SmallOpened;
	}
}


void Trooper::animateForever(CCObject* animation){
	CCAnimation* anim = (CCAnimation*)animation;
	if (m_current_sprite != NULL && m_animate_action != NULL){
		m_current_sprite->stopAction(m_animate_action);
		m_animate_action = NULL;
	}
	if (m_current_sprite == NULL){
		m_current_sprite = CCSprite::spriteWithSpriteFrame(anim->getFrames()->getObjectAtIndex(0));
	}
	m_animate_action = CCRepeatForever::actionWithAction(CCAnimate::actionWithAnimation(anim));
	m_current_sprite->runAction(m_animate_action);
}


void Trooper::animate(const char* animationName, ...){
	va_list params;
	va_start(params, animationName);
	const char* nextName = animationName;
	if (m_current_sprite != NULL && m_animate_action != NULL){
		m_current_sprite->stopAction(m_animate_action);
		m_animate_action = NULL;
	}
	CCArray* animations = CCArray::array();

	while (nextName){
		CCAnimation* anim = TFAnimationCache::sharedAnimationCache()->animationByName(nextName);
		if (m_current_sprite == NULL){
			m_current_sprite = CCSprite::spriteWithSpriteFrame(anim->getFrames()->getObjectAtIndex(0));
			this->addChild(m_current_sprite);
		}
		CCCallFuncO* notifyAction = CCCallFuncO::actionWithTarget(this, callfuncO_selector(Trooper::onAnimationStart), new CCString(nextName));
		animations->addObject(notifyAction);
		nextName = va_arg(params, const char*);
		if (nextName == NULL){
			CCCallFuncO* animAction = CCCallFuncO::actionWithTarget(this, callfuncO_selector(Trooper::animateForever), anim);
			animations->addObject(animAction);
			animAction->retain();
		} else {
			animations->addObject(CCAnimate::actionWithAnimation(anim));
		}
		notifyAction->retain();
	}
	m_current_sprite->runAction(CCSequence::actionsWithArray(animations));
	va_end(params);
	animations->retain();
}
