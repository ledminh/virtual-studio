//
//  RayCaster - Set of simple classes to create a camera/view setup for our Ray Tracer HW Project
//
//  I've included these classes as a mini-framework for our introductory ray tracer.
//  You are free to modify/change.   
//
//  These classes provide a simple render camera which can can return a ray starting from
//  it's position to a (u, v) coordinate on the view plane.
//
//  The view plane is where we can locate our photorealistic image we are rendering.
//  The field-of-view of the camera by moving it closer/further 
//  from the view plane.  The viewplane can be also resized.  When ray tracing an image, the aspect
//  ratio of the view plane should the be same as your image. So for example, the current view plane
//  default size is ( 6.0 width by 4.0 height ).   A 1200x800 pixel image would have the same
//  aspect ratio.
//
//  This is not a complete ray tracer - just a set of skelton classes to start.  The current
//  base scene object only stores a value for the diffuse/specular color of the object (defaut is gray).
//  at some point, we will want to replace this with a Material class that contains these (and other 
//  parameters)
//  
//  (c) Kevin M. Smith  - 24 September 2018
//
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "SceneObjects.h"
#include "Ray.h"
#include "RenderCam.h"
#include "Light.h"



class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void rayTrace();
	
	void drawAxis(double s);

	void saveImage();
	void addPlane(glm::vec3 pos, glm::vec3 n, ofColor c, string imagePath, int tile, int width, int height, bool bumpMap, string bumpmapImgPath = "");

	void addSphere(glm::vec3 position, float radius, ofColor diffuseColor, string imgTexturePath, int tileTexture, bool bm = false, string imgBumpMapPath = "");
	void addPointLight(glm::vec3 p, float intensity);
	void addSpotLight(glm::vec3 p, glm::vec3 d, float i, float a);

	void initScene();
	void sceneIntersect(const Ray &r, int &index, glm::vec3 &point, glm::vec3 &normal);
	void sceneMove(glm::vec3 diff);
	void unselectScene();
	void increaseSizeScene();
	void decreaseSizeScene();

	void selectLight();
	void XRotateLight();
	void YRotateLight();
	void ZRotateLight();

	void intensityListener(float &i);

	void loadTexture();
	void loadBumpMap();


	void selectObject();

	glm::vec3 getMousePos();

	void ofApp::deleteSceneObject();


	ofColor ofApp::lambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse);

	ofColor ofApp::phong(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power);

	bool isBlocked(glm::vec3 point, glm::vec3 lightPos); // Check if the point is blocked from the light by any item on scene

	void selectPlane();

	bool bHide = true;
	bool bShowImage = false;

	ofEasyCam  mainCam;
	ofCamera sideCam;
	ofCamera previewCam;
	ofCamera  *theCam;    // set to current camera either mainCam or sideCam
	bool mainCamMouseEnable = true;



	glm::vec3 currentMousePos = glm::vec3(-100000, -100000, -100000);

	// set up one render camera to render image throughn
	//
	RenderCam renderCam;
	ofImage image;
	bool bRenderCamControlMode = false;

	vector<SceneObject *> scene;

	vector<Plane *> planes;
	int currentPlaneIndex = -1;

	vector<Light *> lights;
	int currentLightIndex = -1;
	
	vector<SceneObject *> objects;
	int currentObjectIndex = -1;
	

	ofxPanel gui;
	ofxFloatSlider intensitySlider;


	int imageWidth = 2400;
	int imageHeight = 1600;
};
