import os, sys
from PIL import Image
import subprocess


output_path = '/home/guoze/Templates/image/'
imageDir = "/home/guoze/Templates/image/studpid_bird/"

if not os.path.isdir( imageDir ):
  print testDir, "isn't a directory"
  sys.exit( 1 )

def mergeImg():
    files = os.listdir( output_path )
    for dirpath, dirnames, filenames in os.walk(imageDir):
        images = [] 
        for img in filenames:
            images.append(img)

        if not os.path.exists(output_path):
            os.makedirs(output_path)

        images.sort(key=lambda x:int(x[-5:-4]))
        print(images)
        
        # print os.path.basename(dirpath)
        imagefile = []
        j = 0
        image_width=0
        image_height=0
        if (len(images) > 0) :
            im = Image.open(dirpath+'/'+images[0])
            image_width = im.size[0]
            image_height = im.size[1]

            all_width = len(images) * image_width
            for j in range(len(images)):
                im = Image.open(dirpath+'/'+images[j])
                im = im.resize((image_width,image_height), Image.ANTIALIAS)
                imagefile.append(im)

            target = Image.new('RGBA', (all_width, image_height))  # width , height
            left = 0
            right = image_width
            # print(imagefile)
            for image in imagefile:
                # image = image.resize((width, height),Image.ANTIALIAS)
                target.paste(image, (left, 0, right, image_height))
                # target.paste(image, (left,right), mask=image)
                left += image_width
                right += image_width
                print(left, right)
                quality_value = 1000
            target.save(output_path+os.path.basename(dirpath)+'_{}.png'.format(j+1), quality=quality_value)
        else:
            print("\tERROR: No images in the folders.e")
   

if __name__ == '__main__':
    mergeImg()
