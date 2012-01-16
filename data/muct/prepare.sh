#!/bin/bash

echo "Downloading files if necessary..."
cd download
wget -nc http://muct.googlecode.com/files/muct-landmarks-v1.tar.gz
wget -nc http://muct.googlecode.com/files/muct-{a,b,c,d,e}-jpg-v1.tar.gz
cd ..

for i in download/muct-*.tar.gz; do 
	echo "Extracting $i..."
	tar xfzk $i
done

#echo "Flipping images (imagemagick needed, slow)..."
#for i in `find jpg -regex '.*i[0-9].*\.jpg'`; do
	#mainname=`basename $i`
	#newname=`dirname $i`/ir${mainname##*i}
	#convert -flip $i $newname
#done

./genPTS.py

echo "Training files are ready. You can run something like: "
echo "    PATH_TO_DEMO/demo -b -m muct76.model -d muct76.def -l camA_Q.list"
