#pragma once
#include "ofMain.h"
#include "Ray.h"


//http://www.alejandrosegovia.net/2014/03/31/bump-map-generation/
//Class Texture
//
class Texture {
public:
	Texture() {}
	Texture(ofImage i) { loadImage(i); }

	void loadImage(ofImage i);
	void loadBumpMapImmage(ofImage i);
	void converTextureToBumpMap();


	float getWidth() { return width; }
	float getHeight() { return height; }

	ofColor getColor(float u, float v);
	glm::vec3 getNormal(float u, float v);
	
	ofImage getImage() { return image; }
	

private:
	ofImage image;
	float width, height;

	ofImage bumpMapImage;
	float bmWidth, bmHeight;
	
	vector<vector<glm::vec3>> bumpMap;
};

//  Base class for any renderable object in the scene
//
class SceneObject {
public:
	SceneObject() {}
	virtual void draw() = 0;    // pure virtual funcs - must be overloaded
	virtual bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return false; }
	
	glm::mat4 rotateToVector(glm::vec3 v1, glm::vec3 v2);

	virtual void setTexture(Texture t, int tle) {
		texture = t;
		tile = tle;
		haveTexture = true;
	}

	virtual void setBumpMap(bool bm) {
		if (haveTexture)
			bBumpMapOn = bm;
	}

	void setMouseSelected(bool s) { bMouseSelected = s; }
	bool isMouseSelected() { return bMouseSelected; }

	void setKeyBoardSelected(bool s) { bKeyBoardSelected = s; }
	bool isKeyBoardSelected() { return bKeyBoardSelected; }

	virtual void move(glm::vec3 diff) {};
	virtual void increaseSize() {};
	virtual void decreaseSize() {};

	// any data common to all scene objects goes here
	glm::vec3 position = glm::vec3(0, 0, 0);

	ofColor getDiffuseColor(const glm::vec3 &point);
	ofColor getSpecularColor(const glm::vec3 &point); 
	ofColor getTextureColor(const glm::vec3 &point);

	virtual void setDiffuseColor(ofColor d) { diffuseColor = d; }
	virtual void setSpecularColor(ofColor s) { specularColor = s; }
	
	void loadBumpMapImage(ofImage i) {
		texture.loadBumpMapImmage(i);
	}

	virtual void getNormal(const glm::vec3 &point, glm::vec3 & normal) {
		if (!bBumpMapOn) return;

		float u, v;

		getUV(point, u, v);

		normal = texture.getNormal(u, v);
	}

	//translate a point at v using matrix m. 
	glm::vec3 multMatrix4ToVec3(glm::mat4 m, glm::vec3 v) {
		glm::vec4 vWorld = m * glm::vec4(v, 1);

		return glm::vec3(vWorld.x, vWorld.y, vWorld.z);
	}

	//This function depends on each type of scene object
	virtual void getUV(const glm::vec3 &point, float &u, float &v) {}

protected:
	// material properties (we will ultimately replace this with a Material class - TBD)
	//
	ofColor diffuseColor = ofColor::grey;    // default colors - can be changed.
	ofColor specularColor = ofColor::white;

	bool haveTexture = false,
		 bBumpMapOn = false;


	Texture texture;

	int tile = 1;

	bool bMouseSelected = false,
		bKeyBoardSelected = false;
};

//  General purpose sphere  (assume parametric)
//
class Sphere : public SceneObject {
public:
	Sphere(glm::vec3 p, float r, ofColor diffuse = ofColor::lightGray) { position = p; radius = r; diffuseColor = diffuse; }
	Sphere() {}
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal);
	
	void draw(); 

	void move(glm::vec3 diff);

	void increaseSize();
	void decreaseSize();

	void getNormal(const glm::vec3 &point, glm::vec3 & n);


	void getUV(const glm::vec3 &point, float &u, float &v);

	float radius = 1.0;


};

//  Mesh class (will complete later- this will be a refinement of Mesh from Project 1)
//
class Mesh : public SceneObject {
	bool intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) { return false; }
	void draw() { }
};


//  General purpose plane 
//
class Plane : public SceneObject {
public:
	Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse = ofColor::darkOliveGreen, float w = 20, float h = 20);
	Plane(); 


	void setPlaneNormal(glm::vec3 n);

	bool intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normal);

	void move(glm::vec3 diff);
	void increaseSize(); 
	void decreaseSize(); 

	void draw(); 

	
	void getUV(const glm::vec3 &point, float &u, float &v);

	// Set &normal to the normal vector at &point from the bump map
	void getNormal(const glm::vec3 & point, glm::vec3 & normal);
	
	glm::vec3 normal = glm::vec3(0, 1, 0);
	
	ofPlanePrimitive plane;
	float width = 20;
	float height = 20;

	glm::vec3 origin;


};
