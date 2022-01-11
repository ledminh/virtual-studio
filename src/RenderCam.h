#pragma once
#include "ofMain.h"
#include "SceneObjects.h"

// view plane for render camera
// 
class  ViewPlane : public Plane {
public:
	ViewPlane(glm::vec3 p0, glm::vec3 p1) { min = p0; max = p1; }

	ViewPlane() {                         // create reasonable defaults (6x4 aspect)
		min = glm::vec3(-3, -2, -distance);		// in Camera space
		max = glm::vec3(3, 2, -distance);			// in Camera space
		
		normal = glm::vec3(0, 0, 1);      // in Camera coord
	}

	void setSize(glm::vec3 min, glm::vec3 max) { this->min = min; this->max = max; }
	
	void setDistance(float d) {
		if (d >= 8 || d <= 3) return;

		distance = d;
		min = glm::vec3(min.x, min.y, -distance);
		max = glm::vec3(max.x, max.y, -distance);
	}

	float getDistance() {
		return distance;
	}

	void increaseSize() {

		if (width() > 10) return;
		
		float aspect = getAspect();						//		aspect = w/h
		float wIncr = 0.1, hIncr = wIncr / aspect;		//	  => h = w/aspect
		
		glm::vec3 vecIncr(wIncr, hIncr, 0);
		
		min -= vecIncr;
		max += vecIncr;
	}

	void decreaseSize() {
		if (width() < 3) return;

		float aspect = getAspect();						//		aspect = w/h
		float wDecr = 0.1, hDecr = wDecr / aspect;		//	  => h = w/aspect

		glm::vec3 vecDecr(wDecr, hDecr, 0);

		min += vecDecr;
		max -= vecDecr;
	}

	float getAspect() { return width() / height(); }

	//glm::vec3 toWorld(float u, float v);   //   (u, v) --> (x, y, z) [ world space ]

	void draw(glm::mat4 mCam) {
		glm::vec3 p1 = multMatrix4ToVec3(mCam, bottomLeft());
		glm::vec3 p2 = multMatrix4ToVec3(mCam, topLeft());
		glm::vec3 p3 = multMatrix4ToVec3(mCam, topRight());
		glm::vec3 p4 = multMatrix4ToVec3(mCam, bottomRight());
		
		ofDrawLine(p1, p2);
		ofDrawLine(p2, p3);
		ofDrawLine(p3, p4);
		ofDrawLine(p4, p1);
		
	}


	

	float width() {
		return (max.x - min.x);
	}
	float height() {
		return (max.y - min.y);
	}

	// some convenience methods for returning the corners
	//
	glm::vec3 topLeft() { return glm::vec3(min.x, max.y, -distance); }
	glm::vec3 topRight() { return max; }
	glm::vec3 bottomLeft() { return min; }
	glm::vec3 bottomRight() { return glm::vec3(max.x, min.y, -distance); }

	//  To define an infinite plane, we just need a point and normal.
	//  The ViewPlane is a finite plane so we need to define the boundaries.
	//  We will define this in terms of min, max  in 2D.  
	//  (in local 2D space of the plane)
	//  ultimately, will want to locate the ViewPlane with RenderCam anywhere
	//  in the scene, so it is easier to define the View rectangle in a local'
	//  coordinate system.
	//
	glm::vec3 min, max;

	float distance = 5;	//distance from RenderCam
};


//  render camera  - currently must be z axis aligned (we will improve this in project 4)
//
class RenderCam : public SceneObject {
public:
	RenderCam() {
		position = glm::vec3(0, 0, 15);

		aim = glm::vec3(0, 0, -1);
		up = glm::vec3(0, 1, 0);

		
	}
	Ray getRay(float u, float v);

	glm::vec3 toWorld(float u, float v);

	glm::mat4 M_camera();


	void rotateIncrease();
	void rotateDecrease();

	glm::vec3 viewPlanePosition() {
		glm::vec4 pWorld = M_camera() * glm::vec4(view.position, 1);

		return glm::vec3(pWorld.x, pWorld.y, pWorld.z);
	}

	void draw() {

		if (isMouseSelected() || isKeyBoardSelected()) {
			ofSetColor(ofColor::red);
		}
		else {
			ofSetColor(ofColor::cadetBlue);
		}


		
		drawCam();
		
		view.draw(M_camera());
	};
	//void drawFrustum();

	void keyControl(int key);



	void drawCam() {
		glm::mat4 mCam = M_camera();

		glm::vec3 p1 = multMatrix4ToVec3(mCam, glm::vec3(-0.5, 0.5, 0.5));
		glm::vec3 p2 = multMatrix4ToVec3(mCam, glm::vec3(-0.5, 0.5, -0.5));
		glm::vec3 p3 = multMatrix4ToVec3(mCam, glm::vec3(0.5, 0.5, -0.5));
		glm::vec3 p4 = multMatrix4ToVec3(mCam, glm::vec3(0.5, 0.5, 0.5));

		glm::vec3 p5 = multMatrix4ToVec3(mCam, glm::vec3(-0.5, -0.5, 0.5));
		glm::vec3 p6 = multMatrix4ToVec3(mCam, glm::vec3(-0.5, -0.5, -0.5));
		glm::vec3 p7 = multMatrix4ToVec3(mCam, glm::vec3(0.5, -0.5, -0.5));
		glm::vec3 p8 = multMatrix4ToVec3(mCam, glm::vec3(0.5, -0.5, 0.5));

		ofDrawLine(p1, p2);
		ofDrawLine(p2, p3);
		ofDrawLine(p3, p4);
		ofDrawLine(p4, p1);

		ofDrawLine(p5, p6);
		ofDrawLine(p6, p7);
		ofDrawLine(p7, p8);
		ofDrawLine(p8, p5);

		ofDrawLine(p1, p5);
		ofDrawLine(p2, p6);
		ofDrawLine(p3, p7);
		ofDrawLine(p4, p8);
	}



	glm::vec3 aim;
	glm::vec3 up;
	ViewPlane view;          // The camera viewplane, this is the view that we will render 

	bool bXRotate = false;
	bool bYRotate = false;
	bool bZRotate = false;
};