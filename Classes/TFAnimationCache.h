/*
 * TFAnimationCache.h
 *
 *  Created on: 26.10.2012
 *      Author: jjay
 */

#ifndef TFANIMATIONCACHE_H_
#define TFANIMATIONCACHE_H_

#include <CCAnimationCache.h>
namespace cocos2d {
class TFAnimationCache: public CCAnimationCache {
public:
	void addAnimationsFromFile(const char* pFileName);
};
}
#endif /* TFANIMATIONCACHE_H_ */
