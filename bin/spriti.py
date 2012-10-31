#! /usr/bin/python
import argparse
import functools
import operator
import os
import plistlib
import re

import Image

parser = argparse.ArgumentParser(description='Make sprites ready for cocos2d')
parser.add_argument("--resources-path", type=str, default='Resources',
    help="Where resources is stored"
)
parser.add_argument("location", type=str, default='.',
    help="Location of animations")

args = parser.parse_args()

def minimize(args, image):
    width, height = image.size
    x, y = 0, 0
    minx, miny, maxx, maxy = 10000, 10000, 0, 0
    for pixel in image.getdata():
        if pixel[3] != 0:
            minx, miny = min(minx, x), min(miny, y)
            maxx, maxy = max(maxx, x), max(maxy, y)

        x += 1
        if x >= width:
            y += 1
            x = 0

    image = image.crop((minx, miny, maxx, maxy))
    image.origsize = width, height
    image.offset = minx, miny
    image.newsize = maxx-minx, maxy-miny
    image.centeroffset = ( 0, 0 )

    # Looks like cocos2d has different mind about offset.
    # There is a correct way to calc offset:
    # image.centeroffset = (
    #     int(width*0.5 - ( minx + 0.5*(maxx-minx))),
    #     int(height*0.5 - ( miny + 0.5*(maxy-miny)))
    # )
    return image
            

def make_sprite(path, name, files, args):
    images = [ Image.open(os.path.join(path, f)) for f in files ]
    images = map(functools.partial(minimize, args), images)
    height = max([ i.size[1] for i in  images])
    width = reduce(operator.add, [i.size[0] for i in images])
    print "Make sprite for %s, frames: %s" % (name, len(images))
    image = Image.new('RGBA', (width, height), (0, 0, 0, 0))
    corner = (0, 0)
    frames = dict()
    counter = 0

    for img in images:
        image.paste(img, corner)
        img.name = "%s_%03d.png" % (name, counter)
        counter += 1
        frames[img.name] = dict(
            frame = "{{%s,%s},{%s,%s}}" % (corner + img.size),
            offset = "{%s,%s}" % img.centeroffset,
            rotated = False,
            sourceColorRect = "{{%s,%s},{%s,%s}}" % (img.offset + img.size),
            sourceSize = "{%s,%s}" % img.origsize,
        )
        corner = (img.size[0] + corner[0], 0)

    plist_filename = os.path.join(os.path.dirname(path), name+'.plist')
    png_filename = os.path.join(os.path.dirname(path), name+'.png')
    meta = dict(
        format=2,
        realTextureFileName=name+'.png',
        size="{%s,%s}" % image.size,
        textureFileName=name+'.png'
    )
    
    plistlib.writePlist(dict(frames=frames, metadata=meta), plist_filename)
    image.save(png_filename)
    return plist_filename, images


if __name__ == '__main__':
    os.chdir(args.resources_path)
    animations = {}
    for path, dirnames, filenames in os.walk(args.location):
        # there is no animations in root dir
        if path in ('', '.'): continue

        # there is already generated animations
        if filter(lambda s: s.endswith('.plist'), filenames): continue

        # no images in path
        if not filter(lambda s: s.endswith('.png'), filenames): continue

        filenames = [ f for f in sorted(filenames) if f.endswith('.png') ]
        if not filenames: continue

        name = path.strip('./')
        name = re.sub('^%s/?' % args.location, '', name)
        name = name.replace('/', '_')
        
        plist_file, images = make_sprite(path, name, filenames, args)
        animations[name] = dict(plist=plist_file, frames=[i.name for i in images])

    plistlib.writePlist(dict(animations=animations), 'animations.plist')

        



