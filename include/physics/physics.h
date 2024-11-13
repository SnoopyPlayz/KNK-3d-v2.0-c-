#pragma once
#include <Jolt/Jolt.h>
#include <glm/glm.hpp>
#include "Jolt/Physics/Body/BodyInterface.h"
#include <Jolt/Physics/Body/MotionType.h>
#include <Jolt/Physics/Body/BodyID.h>

using JPH::BodyID;
using JPH::EMotionType;
extern JPH::BodyInterface *bodyInterface;

BodyID addSphere(float size, glm::vec3 position, EMotionType);
BodyID addBox(glm::vec3 size, glm::vec3 position, EMotionType);
void physicsInit();
void physicsUpdate();
void physicsDraw();
