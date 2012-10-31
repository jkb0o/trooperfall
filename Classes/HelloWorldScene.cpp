#include "HelloWorldScene.h"
#include "Trooper.h"
#include "CCAnimationCache.h"
#include "TFAnimationCache.h"
#include <iostream>

USING_NS_CC;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Arial", 24);
	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("background_01_480x640.jpg");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	//CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	//cache->addSpriteFramesWithFile("animations/big/big_fly.plist");
	//cache->addSpriteFramesWithFile("animations/big/bigone_die.plist");
	//cache->addSpriteFramesWithFile("animations/big/test.plist");
	TFAnimationCache* animCache = (TFAnimationCache*)CCAnimationCache::sharedAnimationCache();
	animCache->addAnimationsFromFile("animations.plist");

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
	schedule(schedule_selector(HelloWorld::spawnTrooper), 3);
	spawnTrooper(0.0);




	return true;
}

void HelloWorld::spawnTrooper(float dt){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int minX = 50;
	int rangeX = size.width - 2*minX;
	int startPos = ( rand() % rangeX ) + minX;


	Trooper* pTroop = Trooper::trooper();
	pTroop->setPosition(ccp(startPos, size.height + 50));
	this->addChild(pTroop, 1);

	CCActionInterval* action = CCMoveBy::actionWithDuration(1, CCPointMake(0, -250));
	pTroop->runAction(CCRepeatForever::actionWithAction(action));

	pTroop->animate("big_fly", NULL);

	// Cleanup
	CCArray* nodesToRemove = CCArray::array();
	for (unsigned int i=0; i<this->getChildrenCount(); i++){
		CCNode* node = (CCNode*)this->getChildren()->objectAtIndex(i);
		if ( node->getPosition().y < -100 ) {
			nodesToRemove->addObject(node);
		}
	}
	this->getChildren()->removeObjectsInArray(nodesToRemove);
	std::cout << "Children count " << this->getChildren()->count() << "\n";
	nodesToRemove->release();

}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
