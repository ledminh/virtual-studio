#pragma once

#include "SceneObjects.h"



// Light class
class Light : public SceneObject {
public:
	Light() {}
	Light(glm::vec3 p, float i) {
		intensity = i;
		position = p;
	}

	void setIntensity(float i) {
		intensity = i;
	}

	float getIntensity() {
		return intensity;
	}

	void move(glm::vec3 diff) {
		if (isKeyBoardSelected()) { //			light can't be selected by mouse because if doing so, the intersect function 
									//			must return true when doing raytracing, which shouldn't be because lights are 
									//			not shown up on the image.
			position = position + diff;
		}
	}

	void draw() {
		if (isMouseSelected() || isKeyBoardSelected()) {
			ofSetColor(ofColor::red);
		}
		else {
			ofSetColor(ofColor::blue);
		}

		ofDrawSphere(position, .1);
	}

	virtual void XRotate() {}
	virtual void YRotate() {}
	virtual void ZRotate() {}


	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
		return false;
		//return (glm::intersectRaySphere(ray.p, ray.d, position, .1, point, normal));
	}

	virtual bool isVisible(glm::vec3 p) { return true; }



private:
	float intensity = 0;


};


class SpotLight : public Light {
public:
	SpotLight() {}

	SpotLight(glm::vec3 p, float i, glm::vec3 d, float cA) {
		setIntensity(i);
		position = p;
		direction = d;
		coneAngle = cA;

	}

	void setDirection(glm::vec3 d) {
		direction = d;
	}

	bool isVisible(glm::vec3 p) {
		bool result = false;

		glm::vec3 line = glm::normalize(p - position);


		if (glm::abs(glm::degrees(glm::angle(line, direction))) <= coneAngle)
			result = true;

		return result;
	}

	

	glm::vec3 getDirection() { return direction; }

	void setConeAngle(float cA) { coneAngle = cA; }
	float getConeAngle() { return coneAngle; }

	void draw() {
		if (isMouseSelected() || isKeyBoardSelected()) {
			
			//X
			ofSetColor(ofColor::red);
			ofDrawLine(position, position + glm::vec3(3, 0, 0));

			//Y
			ofSetColor(ofColor::green);
			ofDrawLine(position, position + glm::vec3(0, 3, 0));

			//Z
			ofSetColor(ofColor::blue);
			ofDrawLine(position, position + glm::vec3(0, 0, 3));
		

			ofSetColor(ofColor::coral);
		}
		else {
			ofSetColor(ofColor::blue);
		}

		ofDrawSphere(position, .1);

		if (isMouseSelected() || isKeyBoardSelected()) 
			ofDrawLine(position, position + direction * 7);
		else
			ofDrawLine(position, position + direction * 2);
	}


	void XRotate() {
		if (!isKeyBoardSelected()) return;
		
		float angle = 1;

		direction = glm::rotateX(direction, glm::radians(angle));

		
	}
	
	void YRotate() {
		if (!isKeyBoardSelected()) return;
		float angle = 1;

		direction = glm::rotateY(direction, glm::radians(angle));
	}

	void ZRotate() {
		if (!isKeyBoardSelected()) return;

		float angle = 1;

		direction = glm::rotateZ(direction, glm::radians(angle));
	}


private:
	glm::vec3 direction = glm::vec3(0, 0, 0);
	float coneAngle = 0;

};