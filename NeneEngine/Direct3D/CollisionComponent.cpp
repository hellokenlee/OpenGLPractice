#include "CollisionComponent.h"

Vec3 CollisionComponent::slide(std::vector<Vec3>& vertPos, std::vector<UINT>& indices) {
	//
	eVelocity = wVelocity / ellipsoidSpace;
	ePosition = wPosition / ellipsoidSpace;
	//
	collisionRecursionDepth = 0;
	Vec3 finalPosition = collideWithWorld(vertPos, indices);
	//
	eVelocity = GRAVITY / ellipsoidSpace;
	ePosition = finalPosition;
	collisionRecursionDepth = 0;
	finalPosition = collideWithWorld(vertPos, indices);

	//
	finalPosition = finalPosition * ellipsoidSpace;
	return finalPosition;
}

Vec3 CollisionComponent::collideWithWorld(std::vector<Vec3>& vertPos, std::vector<UINT>& indices) {
	// These are based off the UNITS_PER_METER from above
	FLOAT unitScale = UNITS_PER_METER / 100.0f;
	FLOAT veryCloseDistance = 0.005f * unitScale;    

	if (collisionRecursionDepth > 5)
		return ePosition;

	eNormalizedVelocity = eVelocity;
	eNormalizedVelocity.Normalize();

	foundCollision = false;
	nearestDistance = 0.0f;
	// TRIANGLE_LIST
	for (int triCounter = 0; triCounter < indices.size() - 2 ; triCounter++) {
		// Get triangle
		Vec3 p0, p1, p2, tempVec;
		p0 = vertPos[indices[triCounter + 0]];
		p1 = vertPos[indices[triCounter + 1]];
		p2 = vertPos[indices[triCounter + 2]];
		//
		p0 = p0 / ellipsoidSpace;
		p1 = p1 / ellipsoidSpace;
		p2 = p2 / ellipsoidSpace;
		// 
		Vec3 triNormal;
		triNormal = (p1 - p0).Cross(p2 - p0);
		triNormal.Normalize();
		//
		SphereCollidingWithTriangle(p0, p1, p2, triNormal);
	}
	// 
	if (foundCollision == false) {
		return ePosition + eVelocity;
	}

	Vec3 destinationPoint = ePosition + eVelocity;

	Vec3 newPosition = ePosition;    
	if (nearestDistance >= veryCloseDistance) {
		Vec3 V = eVelocity;
		V.Normalize();
		V = V * (nearestDistance - veryCloseDistance);
		newPosition = ePosition + V;
		//
		V = XMVector3Normalize(V);
		intersectionPoint -= veryCloseDistance * V;
	}
	// 
	Vec3 slidePlaneOrigin = intersectionPoint;
	Vec3 slidePlaneNormal = newPosition - intersectionPoint;
	slidePlaneNormal.Normalize();
	//
	FLOAT x = XMVectorGetX(slidePlaneOrigin);
	FLOAT y = XMVectorGetY(slidePlaneOrigin);
	FLOAT z = XMVectorGetZ(slidePlaneOrigin);
	// 
	FLOAT A = XMVectorGetX(slidePlaneNormal);
	FLOAT B = XMVectorGetY(slidePlaneNormal);
	FLOAT C = XMVectorGetZ(slidePlaneNormal);
	FLOAT D = -((A*x) + (B*y) + (C*z));
	//
	FLOAT planeConstant = D;
	//
	FLOAT signedDistFromDestPointToSlidingPlane = destinationPoint.Dot(slidePlaneNormal) + planeConstant;
	//
	Vec3 newDestinationPoint = destinationPoint - signedDistFromDestPointToSlidingPlane * slidePlaneNormal;

	// 
	Vec3 newVelocityVector = newDestinationPoint - intersectionPoint;
	//
	if (newVelocityVector.Length() < veryCloseDistance) {
		return newPosition;
	}
	//
	collisionRecursionDepth++;
	ePosition = newPosition;
	eVelocity = newVelocityVector;
	return collideWithWorld(vertPos, indices);
}

bool CollisionComponent::SphereCollidingWithTriangle(Vec3 & p0, Vec3 & p1, Vec3 & p2, Vec3 & triNormal) {
	return false;
}

bool CollisionComponent::checkPointInTriangle(const Vec3 & point, const Vec3 & triV1, const Vec3 & triV2, const Vec3 & triV3) {
	Vec3 cp1 = (triV3 - triV2).Cross(point - triV2);
	Vec3 cp2 = (triV3 - triV2).Cross(triV1 - triV2);
	if (cp1.Dot(cp2) >= 0) {
		cp1 = (triV3 - triV1).Cross(point - triV1);
		cp2 = (triV3 - triV1).Cross(triV2 - triV1);
		if (cp1.Dot(cp2) >= 0) {
			cp1 = (triV2 - triV1).Cross(point - triV1);
			cp2 = (triV2 - triV1).Cross(triV3 - triV1);
			if (cp1.Dot(cp2) >= 0) {
				return true;
			}
		}
	}
	return false;
}

bool CollisionComponent::getLowestRoot(FLOAT a, FLOAT b, FLOAT c, FLOAT maxR, FLOAT * root) {
	FLOAT determinant = b * b - 4.0f * a * c;
	
	if (determinant < 0.0f) 
		return false;
	
	FLOAT sqrtD = sqrt(determinant);
	FLOAT r1 = (-b - sqrtD) / (2 * a);
	FLOAT r2 = (-b + sqrtD) / (2 * a);

	if (r1 > r2) {
		FLOAT temp = r2;
		r2 = r1;
		r1 = temp;
	}

	if (r1 > 0 && r1 < maxR) {
		*root = r1;
		return true;
	}
	
	if (r2 > 0 && r2 < maxR) {
		*root = r2;
		return true;
	}

	return false;
}
