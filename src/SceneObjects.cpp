#include "SceneObjects.h"



//Texture
//
void Texture::loadImage(ofImage i) {
	image = i;

	width = image.getWidth();
	height = image.getHeight();

	converTextureToBumpMap();
}

void Texture::loadBumpMapImmage(ofImage i) {
	//Reset bumpMap
	for (int i = 0; i < bumpMap.size(); i++) {
		bumpMap[i].clear();
	}

	bumpMap.clear();

	bumpMapImage = i;

	
	bmWidth = bumpMapImage.getWidth();
	bmHeight = bumpMapImage.getHeight();

	for (int i = 0; i < bmWidth; i++) {
		vector<glm::vec3> ns;
		for (int j = 0; j < bmHeight; j++) {
			ofColor c = bumpMapImage.getColor(i, j);
			float x = 2* c.r/255.0 -1,
				  y = 2* c.g/255.0 - 1,
				  z = 2* c.b/255.0 - 1 ;

			ns.push_back(glm::normalize(glm::vec3(x,y,z)));

			

		}

		bumpMap.push_back(ns);
	}
	
}

void Texture::converTextureToBumpMap() {
	//Reset bumpMap
	for (int i = 0; i < bumpMap.size(); i++) {
		bumpMap[i].clear();
	}

	bumpMap.clear();


	//Generate heightMap (grayscale map)
	vector<vector<float>> heightMap;
	float baseHeight = -1;

	for (int i = 0; i < width; i++) {
		vector<float> heights;

		for (int j = 0; j < height; j++) {
			ofColor c; 
			
			c = image.getColor(i, j);

			

			float red = c.r,
				green = c.g,
				blue = c.b;
			float h = 0.33*red + 0.66*green + 0.11*blue;
			
			if (baseHeight == -1)
				baseHeight = h;
			else if (baseHeight < h)
				baseHeight = h;

			heights.push_back(h);
		}

		heightMap.push_back(heights);
	}

	//Generate bumpMap
	
	
	for (int i = 0; i < width; i++) {
		vector<glm::vec3> ns;

		for (int j = 0; j < height; j++) {

			float hLeft = (i > 0)? heightMap[i - 1][j] : baseHeight,
				hRight = (i + 1 < width)? heightMap[i + 1][j] : baseHeight,
				hUp = (j > 0)? heightMap[i][j - 1] : baseHeight,
				hDown = (j + 1 < height)? heightMap[i][j + 1]: baseHeight;
			float scale = .01;

			glm::vec3 s =  glm::vec3(1, 0, scale * (hRight - hLeft));
			glm::vec3 t =  glm::vec3(0, 1, scale * (hDown - hUp));

			glm::vec3 n = glm::normalize(glm::cross(s, t));
		
			ns.push_back(n);
		}

		bumpMap.push_back(ns);
	}
}

ofColor Texture::getColor(float u, float v) {
	u = u - (int)u;
	v = v - (int)v;

	int i = u * width - 0.5,
		j = v * height - 0.5;

	return image.getColor(i, j);
	
}

glm::vec3 Texture::getNormal(float u, float v) {
	u = u - (int)u;
	v = v - (int)v;

	int i = u * width - 0.5,
		j = v * height - 0.5;

	return bumpMap[i][j];
}


//SceneObject
// Generate a rotation matrix that rotates v1 to v2
// v1, v2 must be normalized
//
glm::mat4 SceneObject::rotateToVector(glm::vec3 v1, glm::vec3 v2) {

	glm::vec3 axis = glm::cross(v1, v2);
	glm::quat q = glm::angleAxis(glm::angle(v1, v2), glm::normalize(axis));
	return glm::toMat4(q);
}

ofColor SceneObject::getTextureColor(const glm::vec3 &point) {
	float u, v;

	getUV(point, u, v);

	return texture.getColor(u, v);
}

ofColor SceneObject::getDiffuseColor(const glm::vec3 &point) {
	if (!haveTexture) return diffuseColor;

	return getTextureColor(point);

}

ofColor SceneObject::getSpecularColor(const glm::vec3 &point) {
	if (!haveTexture) return specularColor;

	return getTextureColor(point);

}


// Sphere
//
bool Sphere::intersect(const Ray &ray, glm::vec3 &point, glm::vec3 &normal) {
	bool result = glm::intersectRaySphere(ray.p, ray.d, position, radius, point, normal);
	
	return result;
}



void Sphere::draw() {
	if (isKeyBoardSelected() || isMouseSelected()) {
		ofSetColor(ofColor::red);
	}
	else {
		ofSetColor(diffuseColor);
	}

	ofDrawSphere(position, radius);
}

void Sphere::move(glm::vec3 diff) {
	if (isKeyBoardSelected() || isMouseSelected())
		position = position + diff;
}




void Sphere::getUV(const glm::vec3 &point, float &u, float &v) {
	glm::vec3 d = glm::normalize(point - position),
		dx = glm::vec3(d.x, 0, 0),
		dy = glm::vec3(0, d.y, 0),
		dz = glm::vec3(0, 0, d.z);



	u = tile * (0.5 + glm::atan(d.x, d.z) / (2 * PI));
	v = tile * (0.5 - glm::asin(d.y) / PI);
}


void Sphere::getNormal(const glm::vec3 &point, glm::vec3 & n) {
	if (!bBumpMapOn) return;

	float u, v;

	getUV(point, u, v);

	glm::vec3 bmNormal = texture.getNormal(u, v);
	
	glm::mat4 m = rotateToVector(glm::vec3(0,0,1), bmNormal);

	glm::vec3 rVec = glm::normalize(point - position);

	

	glm::vec4 n4 = m * glm::vec4(rVec.x, rVec.y, rVec.z, 1.0) ;

	n = glm::vec3(n4.x, n4.y, n4.z);
		
}

void Sphere::increaseSize() {
	if (isKeyBoardSelected()) {
		radius += .1;
	}
		
}

void Sphere::decreaseSize() {
	if (isKeyBoardSelected()) {
		radius -= .1;
	}
		
}


// Plane
//
Plane::Plane(glm::vec3 p, glm::vec3 n, ofColor diffuse, float w, float h) {
	position = p; 
	width = w;
	height = h;

	setPlaneNormal(n);

	diffuseColor = diffuse;

	

}


Plane::Plane() {
	normal = glm::vec3(0, 1, 0);
	plane.rotateDeg(90, 1, 0, 0);
}

void Plane::setPlaneNormal(glm::vec3 n) {
	normal = n;
	
	if (normal.x == 1) {
		origin = glm::vec3(position.x, position.y - height / 2, position.z + width / 2);
		plane.rotateDeg(90, 0, 1, 0);
	}
	else if (normal.y == 1) {
		origin = glm::vec3(position.x - width / 2, position.y, position.z + height / 2);
		plane.rotateDeg(90, 1, 0, 0);
	}
	else if (normal.z == 1) {
		origin = glm::vec3(position.x - width / 2, position.y - height / 2, position.z);

	}
}

//*****************************************************************************************
// Intersect Ray with Plane  (wrapper on glm::intersect*
//
bool Plane::intersect(const Ray &ray, glm::vec3 & point, glm::vec3 & normalAtIntersect) {
	float dist;
	bool insidePlane = false;
	bool hit = glm::intersectRayPlane(ray.p, ray.d, position, this->normal, dist);
	if (hit) {
		Ray r = ray;
		point = r.evalPoint(dist);
		normalAtIntersect = this->normal;

		if (normal.x == 1) {
			glm::vec2 yrange = glm::vec2(position.y - width / 2, position.y + width / 2);
			glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);
			if (point.y < yrange[1] && point.y > yrange[0] && point.z < zrange[1] && point.z > zrange[0]) {
				insidePlane = true;
			}
		}
		else if (normal.y == 1) {
			glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
			glm::vec2 zrange = glm::vec2(position.z - height / 2, position.z + height / 2);

			if (point.x < xrange[1] && point.x > xrange[0] && point.z < zrange[1] && point.z > zrange[0]) {
				insidePlane = true;
			}
		}
		else if (normal.z == 1) {
			glm::vec2 xrange = glm::vec2(position.x - width / 2, position.x + width / 2);
			glm::vec2 yrange = glm::vec2(position.y - width / 2, position.y + width / 2);
			if (point.x < xrange[1] && point.x > xrange[0] && point.y < yrange[1] && point.y > yrange[0]) {
				insidePlane = true;
			}

		}



	}
	return insidePlane;
}

void Plane::move(glm::vec3 diff) {
	if (isKeyBoardSelected()) {
		position = position + diff;

		if (normal.x == 1) {
			origin = glm::vec3(position.x, position.y - height / 2, position.z + width / 2);
		}
		else if (normal.y == 1) {
			origin = glm::vec3(position.x - width / 2, position.y, position.z + height / 2);
		}
		else if (normal.z == 1) {
			origin = glm::vec3(position.x - width / 2, position.y - height / 2, position.z);

		}
	}

}

void Plane::increaseSize() {
	if (isKeyBoardSelected()) {
		width += .5;
		height += .5;

		if (normal.x == 1) {
			origin = glm::vec3(position.x, position.y - height / 2, position.z + width / 2);
		}
		else if (normal.y == 1) {
			origin = glm::vec3(position.x - width / 2, position.y, position.z + height / 2);
		}
		else if (normal.z == 1) {
			origin = glm::vec3(position.x - width / 2, position.y - height / 2, position.z);

		}
	}

}

void Plane::decreaseSize() {
	if (isKeyBoardSelected()) {
		width -= .5;
		height -= .5;

		if (normal.x == 1) {
			origin = glm::vec3(position.x, position.y - height / 2, position.z + width / 2);
		}
		else if (normal.y == 1) {
			origin = glm::vec3(position.x - width / 2, position.y, position.z + height / 2);
		}
		else if (normal.z == 1) {
			origin = glm::vec3(position.x - width / 2, position.y - height / 2, position.z);

		}
	}

}

void Plane::draw() {


	if (isKeyBoardSelected()) {
		ofSetColor(ofColor::red);
	}
	else {
		ofSetColor(diffuseColor);
	}

	plane.setPosition(position);
	plane.setWidth(width);
	plane.setHeight(height);
	plane.setResolution(4, 4);
	plane.drawWireframe();

}


void Plane::getUV(const glm::vec3 &point, float &u, float &v) {
	if (normal.x == 1) {
		u = tile * (origin.z - point.z) / width;
		v = tile * (point.y - origin.y) / height;
	}
	else if (normal.y == 1) {
		u = tile * (point.x - origin.x) / width;
		v = tile * (origin.z - point.z) / height;
	}
	else if (normal.z == 1) {
		u = tile * (point.x - origin.x) / width;
		v = tile * (point.y - origin.y) / height;


	}
}


void Plane::getNormal(const glm::vec3 &point, glm::vec3 & n) {
	if (!bBumpMapOn) return;

	float u, v;

	getUV(point, u, v);

	glm::vec3 bmNormal = texture.getNormal(u, v);

	if (normal.x == 1) {
		n = glm::vec3(bmNormal.z, bmNormal.y, -bmNormal.x);
	}
	else if (normal.y == 1) {
		n = glm::vec3(bmNormal.x,bmNormal.z, -bmNormal.y);
	}
	else if (normal.z == 1) {
		n = bmNormal;

	}


}

