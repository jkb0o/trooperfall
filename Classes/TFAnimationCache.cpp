/*
 * TFAnimationCache.cpp
 *
 *  Created on: 26.10.2012
 *      Author: jjay
 */

#include "TFAnimationCache.h"
#include "CCFileUtils.h"
#include "CCMutableDictionary.h"
#include "CCMutableArray.h"
#include "CCObject.h"
#include "CCString.h"
#include "CCSpriteFrameCache.h"
#include "CCAnimation.h"
#include <iostream>




namespace cocos2d {
void TFAnimationCache::addAnimationsFromFile(const char* pFileName){
	const char* path = CCFileUtils::fullPathFromRelativePath(pFileName);
	CCDictionary<std::string, CCObject*>* plist = CCFileUtils::dictionaryWithContentsOfFile(path);
	CCDictionary<std::string, CCObject*>* animations = (CCDictionary<std::string, CCObject*>*)plist->objectForKey("animations");


	std::string key = "";
	CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCDictionary<std::string, CCObject*>* animationMeta = NULL;

	animations->begin();
	while ( (animationMeta = (CCDictionary<std::string, CCObject*>*)animations->next(&key) )){
		const char* framesPlist = ( (CCString*)animationMeta->objectForKey("plist") )->toStdString().c_str();
		CCMutableArray<CCObject*> *frameNames = (CCMutableArray<CCObject*>*)animationMeta->objectForKey("frames");

		frameCache->addSpriteFramesWithFile(framesPlist);
		CCAnimation* anim = CCAnimation::animation();


		std::cout << "adding animation with name " << key << " in " << framesPlist << "\n";

		for (unsigned int i=0; i<frameNames->count(); i++){
			const char* frameName= ( (CCString*)frameNames->getObjectAtIndex(i) )->toStdString().c_str();
			std::cout << "    add frame " << frameName << "\n";
			anim->addFrame(frameCache->spriteFrameByName(frameName));
		}
		anim->setDelay(0.028);
		this->addAnimation(anim, key.c_str());
		// Sigfault on anim->retain()
		// still thing it is wrong
	}
	plist->release();




}
}

