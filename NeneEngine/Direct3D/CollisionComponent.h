/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include "Types.h"
#include <vector>

#define GRAVITY Vec3(0.0f, -0.2f, 0.0f)
#define UNITS_PER_METER 100.0f

class CollisionComponent {
public:
	Vec3 slide(std::vector<Vec3>& vertPos, std::vector<UINT>& indices);

	Vec3 collideWithWorld(std::vector<Vec3>& vertPos, std::vector<UINT>& indices);

	bool SphereCollidingWithTriangle(Vec3 &p0, Vec3 &p1, Vec3 &p2, Vec3 &triNormal);

	bool checkPointInTriangle(const  Vec3& point, const Vec3& triV1, const Vec3& triV2, const Vec3& triV3);

	bool getLowestRoot(FLOAT a, FLOAT b, FLOAT c, FLOAT maxR, FLOAT* root);
private:
	// Information about ellipsoid (in world space)
	Vec3 ellipsoidSpace;
	Vec3 wPosition;
	Vec3 wVelocity;
	// Information about ellipsoid (in ellipsoid space)
	Vec3 ePosition;
	Vec3 eVelocity;
	Vec3 eNormalizedVelocity;
	// Collision Information
	bool foundCollision;
	float nearestDistance;
	Vec3 intersectionPoint;
	int collisionRecursionDepth;
};
