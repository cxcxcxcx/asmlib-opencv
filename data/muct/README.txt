Here are some scripts that generate training files from MUCT landmarked face
database ( http://www.milbo.org/muct )

0. The scripts here should work on Linux/Mac. On windows, it might work 
   with Cygwin. To create your own data set, you don't have to use Linux,
   check http://code.google.com/p/asmlib-opencv/wiki/BuildModel

1. Run ./prepare.sh , it will download MUCT dataset and prepare training 
   set automatically. You need bash and Python 2/3.

2. If you have ImageMagick, uncomment commented lines in prepare.sh, you will
   be able to train on flipped images.

3. If succeeded, you will see some ".list" files in this directory.

4. Build the model:
  PATH_TO_DEMO/demo -b -m muct76.model -d muct76.def -l camA_Q.list
     NOTE: currently all the images are read into memory before training, so 
           camA_all will consume 3Gb memory... So please try camA_Q if you 
           don't have enough memory before the problem is solved...

5. Then you can view or use the model file, eg:
  PATH_TO_DEMO/demo -v -m muct76.model
   OR
  PATH_TO_DEMO/demo -f -m muct76.model -pc -C <opencv_face_detect_xml>

6. If you have any questions, submit a issue in Google Code or contact me 
   directly.
