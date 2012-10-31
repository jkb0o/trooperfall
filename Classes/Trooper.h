/*
 * Trooper.h
 *
 *  Created on: 13.10.2012
 *      Author: jjay
 */

#ifndef TROOPER_H_
#define TROOPER_H_

#include <cocos2d.h>

class Trooper: public cocos2d::CCNode, public cocos2d::CCTouchDelegate {

public:
	Trooper();
	static Trooper* trooper();
	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	virtual void draw();

	void animate(const char* animation, ...);
	void animateForever(cocos2d::CCObject* animation);
	void onAnimationStart(cocos2d::CCObject* animationName);
	void onAnimationStart(const char * animationName);

	enum States {
		Falling,
		SmallOpening,
		SmallOpened,
		BigOpening,
		Plane
	};


	cocos2d::CCRect getBoundingBox();

protected:
	cocos2d::CCAnimate* prepareAnimation(const char* animationName);
private:
	cocos2d::CCSprite *m_current_sprite;
	cocos2d::CCAction *m_animate_action;
	cocos2d::CCAction *m_falling_action;
	States m_state;


};

#endif /* TROOPER_H_ */
