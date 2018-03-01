/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/
#include "Drawable.h"

Drawable::Drawable() {
	modelMat = Mat4Identity;
	translation = Vec3(0.0f, 0.0f, 0.0f);
	translationMat = Mat4Identity;
	rotation = Vec3(0.0f, 0.0f, 0.0f);
	rotationMat = Mat4Identity;
	scale = Vec3(0.0f, 0.0f, 0.0f);
	scaleMat = Mat4Identity;
}


void Drawable::moveTo(const Vec3& position) {
    translation = position;
    translationMat = CreateTranslation(translation);
    modelMat = translationMat * rotationMat * scaleMat;
}

void Drawable::scaleTo(const Vec3& _scale) {
    scale = _scale;
    scaleMat = CreateScale(scale);
    modelMat = translationMat * rotationMat * scaleMat;
}

void Drawable::scaleTo(const Float _scale) {
    scale = Vec3(_scale);
    scaleMat = CreateScale(scale);
    modelMat = translationMat * rotationMat * scaleMat;
}

void Drawable::rotateX(const Float radians) {
    rotation.x = radians;
    rotationMat = Mat4Identity;
    rotationMat = CreateRotationX(rotationMat, rotation.x);
    rotationMat = CreateRotationX(rotationMat, rotation.y);
    rotationMat = CreateRotationX(rotationMat, rotation.z);
    modelMat = translationMat * rotationMat * scaleMat;
}

void Drawable::rotateY(const Float radians) {
    rotation.y = radians;
    rotationMat = Mat4Identity;
    rotationMat = CreateRotationX(rotationMat, rotation.x);
    rotationMat = CreateRotationX(rotationMat, rotation.y);
    rotationMat = CreateRotationX(rotationMat, rotation.z);
    modelMat = translationMat * rotationMat * scaleMat;
}

void Drawable::rotateZ(const Float radians) {
    rotation.z = radians;
    rotationMat = Mat4Identity;
    rotationMat = CreateRotationX(rotationMat, rotation.x);
    rotationMat = CreateRotationX(rotationMat, rotation.y);
    rotationMat = CreateRotationX(rotationMat, rotation.z);
    modelMat = translationMat * rotationMat * scaleMat;
}

void Drawable::setModelMat(const Mat4& model) {
    modelMat = model;
}
