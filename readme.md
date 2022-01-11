This app uses raytracing algorithm to model how a camera works in real life. Once open the app, user will see a setup with 1 camera, 2 planes, 3 balls with different sizes, and 2 lights. User can move and rotate the camera, its lenses can be zoom in or out. The 2 planes, one vertical and one horizontal, can also be moved, resized, and changed into different texture. User can also adjust the light intensitive, move and rotate them. The balls can also be resized and changed into different texture. User can add more lights and balls as well as remove them from the scene. After adjusting everything, user can press ENTER to “take a picture”. The picture will be shown up in the same folder with the app. 

It was written in C++ using openframeworks, one of the best frameworks I know so far for progammers with creativity. I coded this app in the Introduction to Graphic class I took at school and it became one of my most complex project I had ever done. I will go back to it some days in the future, but right now I’m focusing on React and web development. 

INSTRUCTION
----------------
1) Install openframeworks from here: https://openframeworks.cc/ 
2) Copy all files in "src" to "src" folder in openframeworks' folder.
3) Copy all files in "data" to "bin/data" folder in openframeworks' folder.
4) Open a any C++ IDE of your choice and compile it.
 

CONTROL KEYS:
----------------
Press ENTER to run raytracing algorithm, then press SPACEBAR to save the photo to bin/data. The photo's name will 
be image_x.jpg with x to be the next available number, starting from 1.

To move/adjust/change texture of a sphere, first press "S" to select the first sphere. The selected sphere will 
turn red. If it is not the sphere you want, press "S" again to switch to the second sphere, then press "S" again
to switch to the third sphere and so on ...

To select a plane, press "P", then "P" again to switch to another plane, and so on ... 
To select a light, press "L", then "L" again to switch to another light, and so on ...

When a plane, a sphere or a light is selected, here are what you can do:
	- Move up (increase y coordinate): press ARROW UP.
	- Move up (decrease y coordinate): press ARROW DOWN.
	- Move left (decrease x coordinate): press ARROW LEFT.
	- Move right (increase x coordinate): press ARROW RIGHT.
	- Move backward (decrease z coordinate): press "W".
	- Move forward (increase z coordinate): press "S".
	- Increase Size (not applicable to light): press "H".
	- Decrease Size (not applicable to light): press "N". 
	- Load Texture (not applicable to light): press "T". Then a file browser window will pop up so that you can choose 
	the texture image. After pressing OK, another dialog window will pop up asking for tile. The number you enter here 
	is how many time the texture image will repeat on the object. 
	- Load BumpMap (not applicable to light): when you load texture file, the bump map option is off, meaning that the 
	object's texture is absolutely flat. To load bump map (or normal map), press "B".
	- Delete: press DEL 	 

While a spot light is selected, press "X", "Y", or "Z" to rotate its direction around X-axis, Y-axis, or Z-axis respectively.

ADDING OBJECT:
	- When you press "1", a point light will be added to the scene at the mouse's position.
To add a point light to the scene, press "1", Add Point Light: "1", Add Spot Light: "2", Add Sphere: "3". 

In this program, you can manipulate Render Cam. Render Cam is the camera taking the picture when you press ENTER and
SPACEBAR. It is presented on screen by a box and a plane is parallel to the box. The plane is the view box.

To turn on/off Render Cam Control mode, press "R". While in Render Cam Control mode, here are what you can do:
	- Using arrow keys (up, down, left, right) and "W" and "S" to move Render Cam similar to moving other object.
	- Moving view plane nearer to the camera: press "M" 
	- Moving view plane further away from the camera: press "J"
	- Increase size of the view plane: press "H"
	- Decrease size of the view plane: press "N"
	- Rotate around X-axis: press "X" to turn on Rotate-X mode, then press "I" or "K" to rotate the camera.
	- Rotate around Y-axis: press "Y" to turn on Rotate-Y mode, then press "I" or "K" to rotate the camera.
	- Rotate around Z-axis: press "Z" to turn on Rotate-Z mode, then press "I" or "K" to rotate the camera.			

You can look at the scene through 3 cameras:
	- Easy Cam: press F1
	- Side Cam: press F2
	- Preview Cam: press F3, this mode is similar to looking through the lens of the Render Cam.
       		





