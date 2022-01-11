#include "RenderCam.h"



// Get a ray from the current camera position to the (u, v) position on
// the ViewPlane
//
Ray RenderCam::getRay(float u, float v) {

	glm::vec3 pointOnPlane = toWorld(u, v);
	return(Ray(position, glm::normalize(pointOnPlane - position)));
}

glm::vec3 RenderCam::toWorld(float u, float v) {
	float vWidth = view.width();
	float vHeight = view.height();

	glm::vec3 pointOnCamCoord = view.min + glm::vec3(u * vWidth, v * vHeight, 0);  

	glm::vec4 pointOnWorldCoord = M_camera() * glm::vec4(pointOnCamCoord,1);

	return glm::vec3(pointOnWorldCoord.x, pointOnWorldCoord.y, pointOnWorldCoord.z);
}

glm::mat4 RenderCam::M_camera() {

	glm::vec3 w = -aim;
	glm::vec3 u = glm::normalize(glm::cross(up, w));
	glm::vec3 v = glm::normalize(glm::cross(w, u));
	glm::mat4 m(1.0);
	glm::mat4 t(1.0);

	// setting these up in row order (like the textbook and slides)
	// *but we need to transpose the result for GLM (to column order)
	//
	m[0][0] = u.x;
	m[0][1] = u.y;
	m[0][2] = u.z;
	m[1][0] = v.x;
	m[1][1] = v.y;
	m[1][2] = v.z;
	m[2][0] = w.x;
	m[2][1] = w.y;
	m[2][2] = w.z;

	m[0][3] = -dot(u, position);
	m[1][3] = -dot(v, position);
	m[2][3] = -dot(w, position);

	
	m = glm::inverse(m);
	// return M*T (as in book) but don't forget to transpose
	//
	return (glm::transpose(m));
}

void RenderCam::keyControl(int key) {
	switch (key) {
	case OF_KEY_UP:
		position += glm::vec3(0, .5, 0);
		break;
	case OF_KEY_DOWN:
		position -= glm::vec3(0, .5, 0);
		break;
	case OF_KEY_LEFT:
		position -= glm::vec3(.5, 0, 0);
		break;
	case OF_KEY_RIGHT:
		position += glm::vec3(.5, 0, 0);
		break;
	case 'W':
	case 'w':
		position -= glm::vec3(0, 0, .5);
		break;
	case 'S':
	case 's':
		position += glm::vec3(0, 0, .5);
		break;
	case 'J':
	case 'j':
		view.setDistance(view.getDistance() + .1);
		break;
	case 'M':
	case 'm':
		view.setDistance(view.getDistance() - .1);
		break;
	case 'H':
	case 'h':
		view.increaseSize();
		break;
	case 'N':
	case 'n':
		view.decreaseSize();
		break;
	case 'X':
	case 'x':
		bXRotate = !bXRotate;
		bYRotate = false;
		bZRotate = false;
		break;
	case 'Y':
	case 'y':
		bYRotate = !bYRotate;
		bXRotate = false;
		bZRotate = false;
		break;
	case 'Z':
	case 'z':
		bZRotate = !bZRotate;
		bXRotate = false;
		bYRotate = false;
		break;
	case 'I':
	case 'i':
		rotateIncrease();
		break;
	case 'K':
	case 'k':
		rotateDecrease();
		break;
	default:
		break;
	}
}

void RenderCam::rotateIncrease() {
	float angle = 1;

	if (bXRotate) {
		aim = glm::rotateX(aim, glm::radians(angle));
		up = glm::rotateX(up, glm::radians(angle));
	}
	else if (bYRotate) {
		aim = glm::rotateY(aim, glm::radians(angle));
		up = glm::rotateY(up, glm::radians(angle));
	}
	else if (bZRotate) {
		aim = glm::rotateZ(aim, glm::radians(angle));
		up = glm::rotateZ(up, glm::radians(angle));
	}
}

void RenderCam::rotateDecrease() {
	float angle = -1;

	if (bXRotate) {
		aim = glm::rotateX(aim, glm::radians(angle));
		up = glm::rotateX(up, glm::radians(angle));
	}
	else if (bYRotate) {
		aim = glm::rotateY(aim, glm::radians(angle));
		up = glm::rotateY(up, glm::radians(angle));
	}
	else if (bZRotate) {
		aim = glm::rotateZ(aim, glm::radians(angle));
		up = glm::rotateZ(up, glm::radians(angle));
	}
}

