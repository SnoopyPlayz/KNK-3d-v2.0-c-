#include <GL/glew.h>
#include <Jolt/Jolt.h>
#include "Jolt/Math/Quat.h"
#include "Jolt/Math/Real.h"
#include <cstdio>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "model.h"
#include "opengl.h"
#include "physics.h"

typedef struct Entity{
	Model model;
	BodyID body;
	glm::vec3 size;
} Entity;

std::vector<Entity> Entities;

/*
btVector3 getEntityPosition(Entity e){
	btTransform t;
	e.body->getMotionState()->getWorldTransform(t);

	return btVector3(t.getOrigin().getX(), t.getOrigin().getY(), t.getOrigin().getZ());
}
*/

void entitiesDraw(){
	//BodyID t = addSphere(3, (glm::vec3){0,10,0}, EMotionType::Dynamic);
	//addSphere(3, (glm::vec3){0,20,0.1}, EMotionType::Dynamic);

	for (unsigned long i = 0; i < Entities.size(); i++) {
		Entity e = Entities[i];
		JPH::RVec3 pos;
		JPH::Quat quat;
		bodyInterface->GetPositionAndRotation(e.body, pos, quat);
		//pos = bodyInterface->GetPosition(Entities[i].body);
		glm::quat q;
		q.x = quat.GetX();
		q.y = quat.GetY();
		q.z = quat.GetZ();
		q.w = quat.GetW();

		glm::vec3 glmPos;
		glmPos.x = pos.GetX();
		glmPos.y = pos.GetY();
		glmPos.z = pos.GetZ();
		drawModel(e.model, glmPos, q, e.size);
	}
}

Entity* addEntity(Model m, BodyID body, vec3 size){
	Entities.push_back((Entity){m, body, size});
	return &Entities[Entities.size() - 1];
}
Entity* addEntity(Model m, BodyID body){
	return addEntity(m, body, (vec3){1, 1, 1});
}

/*
// check if already loaded model or tex with same Name
typedef struct ModelChache{
	Model model;
	btConvexHullShape * physicsShape;
	const char * modelFileName;
	const char * textureFileName;
} ModelChache;

std::vector<ModelChache> modelChache;

void entityInit(const char * modelFile, const char * textureFile, btVector3 pos, btVector3 rot, int mass){
	std::vector<float> vertecies;
	std::vector<int> indecies;
	int vertAmount, indsAmount;

	Model m = (Model){};
	btConvexHullShape * hull;

	bool modelLoaded = false;
	bool textureLoaded = false;

	// find chached model if already loaded
	for (unsigned long i = 0; i < modelChache.size(); i++) {
		if (strcmp(modelChache[i].modelFileName, modelFile) == 0){
			m = modelChache[i].model;
			hull = modelChache[i].physicsShape;
			modelLoaded = true;
		}

		if (strcmp(modelChache[i].textureFileName, textureFile) == 0){
			m.texture = modelChache[i].model.texture;
			textureLoaded = true;
		}
	}

	if (!modelLoaded) {
		loadPlyModel(modelFile, vertecies, indecies, vertAmount, indsAmount);
		m = bufferModel(vertecies, indecies);
	}
	if (!textureLoaded) {
		m.texture = loadTexture(textureFile);
	}

	std::vector<float> physicsVerts; // just vert without tex coords or normals.
	for (unsigned long i = 0; i < vertecies.size(); i+=8) {
		physicsVerts.push_back(vertecies[i]);
		physicsVerts.push_back(vertecies[i+1]);
		physicsVerts.push_back(vertecies[i+2]);
	}

	//physics
	btQuaternion quaternion; //rot
    	quaternion.setEulerZYX(rot.z(), rot.y(), rot.x());

	btTransform t;
	t.setIdentity();
	t.setOrigin(pos);
	t.setRotation(quaternion);
	btMotionState* motion = new btDefaultMotionState(t);

	if (!modelLoaded) {
		hull = new btConvexHullShape((const btScalar*)physicsVerts.data(),physicsVerts.size(),sizeof(physicsVerts[0]));
	}

	btVector3 interia(0, 0, 0);
	if (mass != 0)
		hull->calculateLocalInertia(mass, interia);
	
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, hull, interia);
	btRigidBody* body = new btRigidBody(info);

	world->addRigidBody(body);

	Entities.push_back((Entity) {m, body});

	//chache new model
	if (!textureLoaded || !modelLoaded)
		modelChache.push_back((ModelChache){m, hull, modelFile, textureFile});
}*/
