First navigate into libtiny3d folder:

1.cd libtiny3d

To compile all files, enter following command:

1.make

(make sure you are in the libtiny3d folder)

To make the clockface pgm (Task 1), Enter following command:

1. ./output

To make the cube animation frame pgms (Task 2),Enter following command:

1. ./cube

(This will create several frames in .pgm format.We are using ffmpeg to convert them in to .mp4)

To make the soccer ball animation frame pgms (Task 3),Enter following command:

1. ./soccer

(This will create several frames in .pgm format.We are using ffmpeg to convert them in to .mp4)

To make the animation with lights (Task 4), Enter the following command:

1. ./light

(This will create several frames in .pgm format.We are using ffmpeg to convert them in to .mp4)

Installation of imagemagick to convert pgm to png,enter following commands:

1.sudo apt update
2.sudo apt install imagemagic

wait until imagemagick is installed...

To convert pgm to png using imagemagick,use following command:

1.convert output.pgm clockface.png

(This will make a png file in libtiny3d folder)

Installation of ffmpeg to convert pmg frames to mp4, enter following commands:
1.sudo apt update
2.sudo apt install ffmpeg
Wait until ffmpeg is installed...

Now if you have installed both imagemagick and ffmpeg, use following commands to get png and mp4 files:

To convert output.pgm to clockface.png:

1.convert output.pgm clockface.png

To convert cube frame pgms to mp4:

1.ffmpeg -framerate 30 -i cube%03d.pgm -c:v libx264 -pix_fmt yuv420p cube.mp4

To convert soccer ball frame pgms to mp4:

1.ffmpeg -framerate 30 -i frame_%03d.pgm -c:v libx264 -pix_fmt yuv420p soccer.mp4

To convert animation with light frame pgms to mp4:

1.ffmpeg -framerate 30 -i frames%03d.pgm -c:v libx264 -pix_fmt yuv420p light.mp4

Now you will have following files:

1.clockface.png(Task 1)
2.cube.mp4(Task 2)
3.soccer.mp4(Task 3)
4.light.mp4(Task 4)

If you want a fresh start again (undo the make),enter following:

1.make clean

(This will remove any newly created file)

