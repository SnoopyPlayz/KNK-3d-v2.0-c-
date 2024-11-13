#include <cstdio>
#include <vector>
#include <GLFW/glfw3.h>
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/MotionType.h>
#include "Jolt/Physics/Collision/ObjectLayer.h"
#include "glm/gtc/quaternion.hpp"
#include "opengl.h"
//this is needed
#include "broadPhase.cpp"

using namespace JPH;
static void TraceImpl(const char *inMessage, ...){
	printf("[Trace]: %s \n",inMessage);
}

// Custom assert failed function
void AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, int inLine){

    printf("[Assertion Failed]: Expression ( %s ) failed in file %s at line %d \n", inExpression, inFile, inLine);

    if (inMessage != nullptr){
        printf("Message: %s", inMessage);
    }
    std::abort(); // Stop execution if an assertion fails
}

PhysicsSystem physics_system;
TempAllocatorImpl *temp_allocator;
JobSystemThreadPool job_system;
BPLayerInterfaceImpl *broad_phase_layer_interface;
ObjectVsBroadPhaseLayerFilterImpl *object_vs_broadphase_layer_filter;
ObjectLayerPairFilterImpl *object_vs_object_layer_filter;

BodyInterface *bodyInterface; //extern
std::vector<BodyID> bodies;

void physicsInit(){
	RegisterDefaultAllocator();

	Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;);

	Factory::sInstance = new Factory();

	RegisterTypes();
	
	temp_allocator = new TempAllocatorImpl(10 * 1024 * 1024);

	job_system.Init(cMaxPhysicsJobs, cMaxPhysicsBarriers, thread::hardware_concurrency() - 1);

	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodies = 8192;
	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodyPairs = 8192;
	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
	const uint cMaxContactConstraints = 8192;

	// This took 1.5d to fix
	broad_phase_layer_interface = new BPLayerInterfaceImpl();
	object_vs_broadphase_layer_filter = new ObjectVsBroadPhaseLayerFilterImpl();
	object_vs_object_layer_filter = new ObjectLayerPairFilterImpl();

	// Now we can create the actual physics system.
	physics_system.Init(cMaxBodies, 0, cMaxBodyPairs, cMaxContactConstraints, *broad_phase_layer_interface, *object_vs_broadphase_layer_filter, *object_vs_object_layer_filter);
	bodyInterface = &physics_system.GetBodyInterface();
}

ObjectLayer layerType(EMotionType motionType){
	if (motionType == EMotionType::Static) {
		return Layers::NON_MOVING;
	}else {
		return Layers::MOVING;
	}
}

BodyID addSphere(float size, glm::vec3 position, EMotionType motionType){
	ObjectLayer layer = layerType(motionType);

	BodyCreationSettings sphereSettings(new SphereShape(size), RVec3(position.x, position.y, position.z), Quat::sIdentity(), motionType, layer);

	BodyID sphere = bodyInterface->CreateAndAddBody(sphereSettings, EActivation::Activate);
	bodies.push_back(sphere);
	return sphere;
}

BodyID addBox(glm::vec3 size, glm::vec3 position, EMotionType motionType){
	ObjectLayer layer = layerType(motionType);

	BodyCreationSettings boxSettings(new BoxShape((JPH::Vec3){size.x, size.y, size.z}), RVec3(position.x, position.y, position.z), Quat::sIdentity(), motionType, layer);


	BodyID box = bodyInterface->CreateAndAddBody(boxSettings, EActivation::Activate);
	bodies.push_back(box);
	return box;
}

void physicsUpdate(){
	static bool firstUpdate = true;
	if (firstUpdate) {
		physics_system.OptimizeBroadPhase();
		firstUpdate = false;
	}
	physics_system.Update(1. / 60., 1, temp_allocator, &job_system);
}

void physicsDraw(){

	/*for (unsigned long i = 0; i < bodies.size(); i++) {
		RVec3 position = bodyInterface->GetPosition(bodies[i]);
		Quat rotation = bodyInterface->GetRotation(bodies[i]);
		
		glm::quat q;
		q.x = rotation.GetX();
		q.y = rotation.GetY();
		q.z = rotation.GetZ();
		q.w = rotation.GetW();

		bodyInterface->GetShape(bodies[i]);

		drawModel(sphereModel,(glm::vec3){position.GetX(),position.GetY(),position.GetZ()}, q, (glm::vec3){3,3,3});
	}*/
}
