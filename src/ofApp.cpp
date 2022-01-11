#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(ofColor::black);
	
	// Set up mainCam
	theCam = &mainCam;
	mainCam.setDistance(10);
	mainCam.setNearClip(.1);

	//Set up preview Cam
		//position = position of renderCam
	previewCam.setPosition(renderCam.position);


	// Fov = Fov of renderCam

	ofVec4f tWorld = renderCam.M_camera() * glm::vec4((renderCam.view.topRight() + renderCam.view.topLeft()) / 2, 1);
	ofVec3f v1 = glm::vec3(tWorld.x, tWorld.y, tWorld.z) - renderCam.position;

	ofVec4f bWorld = renderCam.M_camera() * glm::vec4((renderCam.view.bottomRight() + renderCam.view.bottomLeft()) / 2, 1);
	ofVec3f v2 = glm::vec3(bWorld.x, bWorld.y, bWorld.z) - renderCam.position;

	previewCam.setFov(v1.angle(v2));
	
	//Set up sideCam
	sideCam.setPosition(glm::vec3(-40,renderCam.position.y, renderCam.position.z/2));
	sideCam.lookAt(glm::vec3(0,2,10));

	
	initScene();


	//Set up ofImage
	image.allocate(imageWidth, imageHeight, ofImageType::OF_IMAGE_COLOR);


	//Gui setup
	intensitySlider.addListener(this, &ofApp::intensityListener);

	gui.setup();

	gui.add(intensitySlider.setup("Intensity", 2, 0, 10, 200, 20));


	
	


	
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	theCam->begin();
	
	drawAxis(5);

	ofNoFill();

	for (int i = 0; i < scene.size(); i++) {
		scene[i]->draw();
	}

	ofSetColor(ofColor::deepSkyBlue);
	renderCam.draw();



	theCam->end();


	if (bShowImage) {
		image.draw(glm::vec3(0, 0, 0), imageWidth, imageHeight);
	}

	gui.draw();
		
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key){

	if (bRenderCamControlMode) {
		renderCam.keyControl(key);

		//Set up preview Cam
		//position = position of renderCam
		previewCam.setPosition(renderCam.position);


		// Fov = Fov of renderCam
		
		ofVec4f tWorld = renderCam.M_camera() * glm::vec4((renderCam.view.topRight() + renderCam.view.topLeft())/2, 1);
		ofVec3f v1 = glm::vec3(tWorld.x, tWorld.y, tWorld.z) - renderCam.position;
		
		ofVec4f bWorld = renderCam.M_camera() * glm::vec4((renderCam.view.bottomRight() + renderCam.view.bottomLeft())/2, 1);
		ofVec3f v2 = glm::vec3(bWorld.x, bWorld.y, bWorld.z) - renderCam.position;

		previewCam.setFov(v1.angle(v2));
		

		
		previewCam.lookAt(renderCam.position + 3*renderCam.aim, renderCam.up);


		//Set up sideCam
		sideCam.setPosition(glm::vec3(-40, renderCam.position.y, renderCam.position.z / 2));
		sideCam.lookAt(renderCam.position + renderCam.aim);
	}
		

	glm::vec3 mP = getMousePos();

	switch (key) {
	case OF_KEY_F1:
		theCam = &mainCam;
		break;
	case OF_KEY_F2:
		theCam = &sideCam;
		break;
	case OF_KEY_F3:
		theCam = &previewCam;
		break;
	case OF_KEY_RETURN:
		rayTrace();
		break;
	case ' ':
		saveImage();
		break;
	case 'C':
	case 'c':
		if (mainCamMouseEnable)
			mainCam.disableMouseInput();
		else
			mainCam.enableMouseInput();

		mainCamMouseEnable = !mainCamMouseEnable;
		break;
	case 'P':
	case 'p':
		//Unselect all scene objects
		unselectScene();
		
		//Unselect RenderCam
		bRenderCamControlMode = false;
		renderCam.setKeyBoardSelected(bRenderCamControlMode);
		
		selectPlane();
		break;
	case 'L':
	case 'l':
		//Unselect all scene objects
		unselectScene();

		//Unselect RenderCam		
		bRenderCamControlMode = false;
		renderCam.setKeyBoardSelected(bRenderCamControlMode);
		
		selectLight();
		break;
	case 'O':
	case 'o':
		//Unselect all scene objects
		unselectScene();

		//Unselect RenderCam		
		bRenderCamControlMode = false;
		renderCam.setKeyBoardSelected(bRenderCamControlMode);
		
		selectObject();
		break;
	case 'R':
	case 'r':
		bRenderCamControlMode = !bRenderCamControlMode;
		renderCam.setKeyBoardSelected(bRenderCamControlMode);

		if (bRenderCamControlMode) {
			unselectScene();
			currentLightIndex = -1;
			currentObjectIndex = -1;
			currentPlaneIndex = -1;
		}

		break;
	case OF_KEY_UP:
		sceneMove(glm::vec3(0, .5, 0));
		break;
	case OF_KEY_DOWN:
		sceneMove(glm::vec3(0, -.5, 0));
		break;
	case OF_KEY_LEFT:
		sceneMove(glm::vec3(-.5, 0, 0));
		break;
	case OF_KEY_RIGHT:
		sceneMove(glm::vec3(.5, 0, 0));
		break;
	case OF_KEY_DEL:
		deleteSceneObject();
		break;		
	case 'W':
	case 'w':
		sceneMove(glm::vec3(0, 0, -.5));
		break;
	case 'S':
	case 's':
		sceneMove(glm::vec3(0, 0, .5));
		break;
	case 'H':
	case 'h':
		increaseSizeScene();
		break;
	case 'N':
	case 'n':
		decreaseSizeScene();
		break;
	case 'X':
	case 'x':
		XRotateLight();
		break;
	case 'Y':
	case 'y':
		YRotateLight();
		break;
	case 'Z':
	case 'z':
		ZRotateLight();
		break;
	case 'T':
	case 't':
		loadTexture();
		break;
	case 'B':
	case 'b':
		loadBumpMap();
		break;
	case '1':
		addPointLight(mP, 2);
		break;
	case '2':		
		addSpotLight(mP, -glm::normalize(mP), .4, 20);
		break;
	case '3':		
		addSphere(getMousePos(), .5, ofColor(0, 102, 204), "beachball_texture.jpg", 1);
		break;
	default:
		break;
	}


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (mainCamMouseEnable) return;
	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 rn = glm::normalize(p - theCam->getPosition());

	Ray r = Ray(p, rn);
	int index;
	glm::vec3 point, normal;

	sceneIntersect(r, index, point, normal);


	glm::vec3 diff = point - currentMousePos;

	sceneMove(diff);

	currentMousePos = point;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (mainCamMouseEnable) return;

	unselectScene();

	glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 rn = glm::normalize(p - theCam->getPosition());

	Ray r = Ray(p, rn);
	int index;
	glm::vec3 point, normal;

	sceneIntersect(r, index, point, normal);

	if (index != -1) {
		scene[index]->setMouseSelected(true);
		currentMousePos = point;
	}
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->setMouseSelected(false);
	}

	currentMousePos = glm::vec3(-100000, -100000, -100000);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//New methods
void ofApp::deleteSceneObject() {
	if (currentLightIndex != -1) {
		SceneObject *selectedLight = lights[currentLightIndex];

		int selectedSceneIndex = -1;

		for (int i = 0; i < scene.size(); i++) {
			if (selectedLight == scene[i])
				selectedSceneIndex = i;
		}

		scene.erase(scene.begin() + selectedSceneIndex);
		lights.erase(lights.begin() + currentLightIndex);

		currentLightIndex = -1;
	}
	else if (currentObjectIndex != -1) {
		SceneObject *selectedObject = objects[currentObjectIndex];

		int selectedSceneIndex = -1;

		for (int i = 0; i < scene.size(); i++) {
			if (selectedObject == scene[i])
				selectedSceneIndex = i;
		}

		scene.erase(scene.begin() + selectedSceneIndex);
		objects.erase(objects.begin() + currentObjectIndex);

		currentObjectIndex = -1;
	}
}


void ofApp::loadBumpMap() {
	if (currentPlaneIndex == -1 && currentObjectIndex == -1) return;


	ofFileDialogResult result = ofSystemLoadDialog("Load BumpMap file", false, ofFilePath::getCurrentWorkingDirectory());

	if (result.bSuccess) {
		string imgPath = result.getPath();

		if (currentPlaneIndex != -1) {
			planes[currentPlaneIndex]->setBumpMap(true);
			planes[currentPlaneIndex]->loadBumpMapImage(ofImage(imgPath));
		}
		else if (currentObjectIndex != -1) {
			objects[currentObjectIndex]->setBumpMap(true);
			objects[currentObjectIndex]->loadBumpMapImage(ofImage(imgPath));
		}



	}
}


void ofApp::loadTexture() {
	if (currentPlaneIndex == -1 && currentObjectIndex == -1) return;


	ofFileDialogResult result = ofSystemLoadDialog("Load texture file", false, ofFilePath::getCurrentWorkingDirectory());
		
	if (result.bSuccess) {
		string imgPath = result.getPath();

		int tile = stoi(ofSystemTextBoxDialog("Tile: ", "3"));

		if (currentPlaneIndex != -1) {
			planes[currentPlaneIndex]->setTexture(Texture(ofImage(imgPath)), tile);
		}
		else if (currentObjectIndex != -1) {
			objects[currentObjectIndex]->setTexture(Texture(ofImage(imgPath)), tile);
		}


		
	}
}


void ofApp::addPointLight(glm::vec3 p, float intensity) {
	Light *l = new Light(p, intensity);

	lights.push_back(l);
	scene.push_back(l);

}

void ofApp::addSpotLight(glm::vec3 p, glm::vec3 d, float i, float a) {
	SpotLight *sL = new SpotLight(p, i, d, a);

	lights.push_back(sL);
	scene.push_back(sL);
}

glm::vec3 ofApp::getMousePos() {
	glm::vec3 p = theCam->screenToWorld(glm::vec3(ofGetMouseX(), ofGetMouseY(), 0));
	glm::vec3 d = p - theCam->getPosition();
	glm::vec3 dn = glm::normalize(d);

	float dist;
	glm::vec3 pos = glm::vec3(0, 0, 0);


	glm::intersectRayPlane(p, dn, pos, glm::normalize(theCam->getZAxis()), dist);

	glm::vec3 point = p + dn * dist;

	return point;
}

void ofApp::addSphere(glm::vec3 position, float radius, ofColor diffuseColor, string imgTexturePath, int tileTexture, bool bm, string imgBumpMapPath) {
	Sphere* s = new Sphere(position, radius, diffuseColor);
	s->setTexture(Texture(ofImage(imgTexturePath)), tileTexture);
	s->setBumpMap(bm);

	s->loadBumpMapImage(ofImage(imgBumpMapPath));
	

	scene.push_back(s);
	objects.push_back(s);
}

void ofApp::intensityListener(float &i) {
	if (currentLightIndex != -1) {
		lights[currentLightIndex]->setIntensity(i);
	}
}

void ofApp::XRotateLight() {
	for (int i = 0; i < lights.size(); i++) {
		lights[i]->XRotate();
	}
}

void ofApp::YRotateLight() {
	for (int i = 0; i < lights.size(); i++) {
		lights[i]->YRotate();
	}
}

void ofApp::ZRotateLight() {
	for (int i = 0; i < lights.size(); i++) {
		lights[i]->ZRotate();
	}
}

void ofApp::increaseSizeScene() {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->increaseSize();
	}
}

void ofApp::decreaseSizeScene() {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->decreaseSize();
	}
}


void ofApp::unselectScene() {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->setMouseSelected(false);
		scene[i]->setKeyBoardSelected(false);
	}

	
}

void ofApp::sceneMove(glm::vec3 diff) {
	for (int i = 0; i < scene.size(); i++) {
		scene[i]->move(diff);
	}
}

void ofApp::selectObject() {
	currentLightIndex = -1;
	currentPlaneIndex = -1;

	currentObjectIndex++;

	if (currentObjectIndex == objects.size()) {
		currentObjectIndex = -1;
	}
	else {
		objects[currentObjectIndex]->setKeyBoardSelected(true);
	}


}

void ofApp::selectPlane() {	
	currentLightIndex = -1;
	currentObjectIndex = -1;
	
	currentPlaneIndex++;
	
	if (currentPlaneIndex == planes.size()) {
		currentPlaneIndex = -1;
	}
	else {
		planes[currentPlaneIndex]->setKeyBoardSelected(true);
	}
	
	
}

void ofApp::selectLight() {
	currentObjectIndex = -1;
	currentPlaneIndex = -1;

	currentLightIndex++;

	if (currentLightIndex == lights.size()) {
		currentLightIndex = -1;
	}
	else {
		lights[currentLightIndex]->setKeyBoardSelected(true);
		intensitySlider = lights[currentLightIndex]->getIntensity();
	}
		
}

void ofApp::addPlane(glm::vec3 pos, glm::vec3 n, ofColor c, string imagePath, int tile, int width, int height, bool bumpMap, string bmImgPath) {
	Plane* p = new Plane(pos, n, c, width, height);
	p->setTexture(Texture(ofImage(imagePath)), tile);
	p->setBumpMap(bumpMap);

	if (bmImgPath != "")
		p->loadBumpMapImage(ofImage(bmImgPath));

	scene.push_back(p);
	planes.push_back(p);
}



void ofApp::sceneIntersect(const Ray &r, int &index, glm::vec3 &point, glm::vec3 &normal) {
	float dSquare = 1000000;
	index = -1;

	for (int iS = 0; iS < scene.size(); iS++) {

		glm::vec3 p;
		glm::vec3 n;

		if (scene[iS]->intersect(r, p, n)) {
			float thisDSquare = ofDistSquared(p.x, p.y, p.z, renderCam.position.x, renderCam.position.y, renderCam.position.z);


			if (thisDSquare < dSquare) {
				dSquare = thisDSquare;
				index = iS;
				point = p;
				normal = n;
			}

		}


	}
}

void ofApp::initScene() {
	// Create horizontal plane
	addPlane(glm::vec3(0, -2, 0), glm::vec3(0, 1, 0), ofColor::white, "t2.tif", 20, 60, 60, true, "t2_normal.tif");

	// Create vertical plane
	addPlane(glm::vec3(0, 10, -30), glm::vec3(0, 0, 1), ofColor::white, "Stone_Wall_014_basecolor.jpg", 3, 60, 60, false, "Stone_Wall_014_normal.jpg");

	
	// Create 3 spheres
	addSphere(glm::vec3(-1, -1, -1), 1, ofColor(153, 76, 0), "t2.tif", 1, true, "t2_normal.tif");
	
	addSphere(glm::vec3(-.5, -1.5, .5), .5, ofColor(102, 0, 51), "Blue_Ice_001_COLOR.jpg", 1, true, "Blue_Ice_001_NORM.jpg");

	addSphere(glm::vec3(1, -1, 0), 1, ofColor(0, 102, 204), "beachball_texture.jpg", 1);

	

	//Create lights
	addPointLight(glm::vec3(0, 8, 0), 2);


	//Spot light
	glm::vec3 posSpotLight1 = glm::vec3(-6, 5, 6);
	glm::vec3 direction = glm::normalize(-posSpotLight1);

	addSpotLight(posSpotLight1, direction, .4, 20);






}

void ofApp::saveImage() {
	int i = 1;
	string fileName = "image_" + to_string(i) + ".jpg";
	ofFile file = ofFile(fileName);
	
	while (file.exists()) {
		i++;
		fileName = "image_" + to_string(i) + ".jpg";
		file = ofFile(fileName);
	}

	image.save(fileName);
}

ofColor ofApp::lambert(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse) {
	ofColor result = ofColor::black;
	
	for (int i = 0; i < lights.size(); i++) {
		if (!isBlocked(p, lights[i]->position)) {
			
			if (lights[i]->isVisible(p)) {
				float I = lights[i]->getIntensity();
				
				glm::vec3 l = lights[i]->position - p;
									   
				float r = glm::length(l)*0.1;
				result += diffuse * (I / (r * r))*glm::max(0.0, (double)glm::dot(norm, glm::normalize(l)));
			}
			
		
		
		
		}


		
	}

	return result;
}

ofColor ofApp::phong(const glm::vec3 &p, const glm::vec3 &norm, const ofColor diffuse, const ofColor specular, float power) {
	ofColor result = ofColor::black;

	glm::vec3 v = glm::normalize(renderCam.position - p);

	for (int i = 0; i < lights.size(); i++) {
		
		if(lights[i]->isVisible(p)){
			float I = lights[i]->getIntensity();


			glm::vec3 l = lights[i]->position - p;
			float r = glm::length(l);

			l = glm::normalize(l);

			glm::vec3 h = (v + l) / glm::length(v + l);


			result += specular * (I / (r * r)) *  glm::pow(glm::max(0.0, (double)glm::dot(norm, h)), power);
		}
		
		

		
	}

	return result;
}

void ofApp::rayTrace() {	
	
	for (int w = 0; w < imageWidth; w++) {
		for (int h = 0; h < imageHeight; h++) {
			int index;
			glm::vec3 point, normal;
					
			float u = (w + 0.5) / imageWidth,
				v = 1 - (h + 0.5) / imageHeight;
			Ray r = renderCam.getRay(u, v);

			sceneIntersect(r, index, point, normal);
			
					
			if (index == -1) { //Background
				image.setColor(w, h, ofColor::black);
			}
			else { //Scene
				ofColor color = ofColor::white * .04; //Ambient light

				scene[index]->getNormal(point, normal);

				color += lambert(point, normal, scene[index]->getDiffuseColor(point));

				float power = 100;


				color += phong(point, normal, scene[index]->getDiffuseColor(point), scene[index]->getSpecularColor(point), power);

				

							   				 				
				image.setColor(w, h, color);
			}
			
		}
	}
	
	image.update();

}

bool ofApp::isBlocked(glm::vec3 point, glm::vec3 lightPos) {
	glm::vec3 vFromPtoL = lightPos - point;
	glm::vec3 d = glm::normalize(vFromPtoL);

	Ray r = Ray(point + d * 0.01, d);
	glm::vec3 pIntersect, n;

	bool blocked = false;

	for (int i = 0; i < scene.size() && !blocked; i++) {
		if (scene[i]->intersect(r, pIntersect, n)) {
			float distPoint2Intersect = glm::distance2(pIntersect, point),
				distPoint2Light = glm::distance2(lightPos, point);
			
			if (distPoint2Intersect < distPoint2Light)
				blocked = true;
		}
	}

	return blocked;
}


void ofApp::drawAxis(double s) {
	//X
	ofSetColor(ofColor::red);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(s, 0, 0));

	//Y
	ofSetColor(ofColor::green);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, s, 0));

	//Z
	ofSetColor(ofColor::blue);
	ofDrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, s));
}





