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
private:
	cocos2d::CCSprite *m_current_sprite;

	bool m_is_falling;
public:
	Trooper();
	static Trooper* trooper();
	virtual void onEnter();
	virtual void onExit();

	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);



	cocos2d::CCRect getBoundingBox();
};

#endif /* TROOPER_H_ */
