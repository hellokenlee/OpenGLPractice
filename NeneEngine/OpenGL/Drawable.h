/*Copyright reserved by KenLee@2018 ken4000kl@gmail.com*/
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Shader.h"
#include "Camera.h"
#include "Types.h"

//
//    Drawable: Abstract Drawable Class
//

class Drawable {
public:
    // 构造
	Drawable();
    // 绘制
	virtual void draw(const Shader* pShader = nullptr, const Camera* pCamera = nullptr) = 0;
	virtual void drawInstanced(const Shader* pShader = nullptr, const Camera* pCamera = nullptr) = 0;
	// 变换函数
	virtual void moveTo(const Vec3 &position);
	virtual void scaleTo(const Vec3 &scale);
	virtual void scaleTo(const Float scale);
	virtual void rotateX(const Float radians);
	virtual void rotateY(const Float radians);
	virtual void rotateZ(const Float radians);
	// 更改变换矩阵
	inline const Mat4& getModelMat() {
        return modelMat;
	}
	void setModelMat(const Mat4& model);
private:
	//
	Mat4 modelMat;
	//
	Vec3 translation;
	Mat4 translationMat;
	//
	Vec3 rotation;
	Mat4 rotationMat;
    //
	Vec3 scale;
	Mat4 scaleMat;
};

#endif  //	DRAWABLE_H
