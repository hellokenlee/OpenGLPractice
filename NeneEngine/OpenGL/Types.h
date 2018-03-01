/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TYPES_H
#define TYPES_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#ifndef M_PI
#define M_PI 3.141592653f
#endif // M_PI

#define M_PI_TIMES_2 6.283185306f
#define M_PI_OVER_2 1.5707963265f

#define Float GLfloat
#define Int GLint
#define UInt GLuint
#define Char GLchar
#define Vec2 glm::vec2
#define Vec3 glm::vec3
#define Vec4 glm::vec4
#define Mat3 glm::mat3
#define Mat4 glm::mat4
#define Mat4Identity glm::mat4()
#define Radians(degree) glm::radians(degree)

#define CreateTranslation(pos) glm::translate(Mat4Identity, pos)
#define CreateRotationX(mat, angle) glm::rotate(mat, angle, Vec3(1.0f, 0.0f, 0.0f))
#define CreateRotationY(mat, angle) glm::rotate(mat, angle, Vec3(0.0f, 1.0f, 0.0f))
#define CreateRotationZ(mat, angle) glm::rotate(mat, angle, Vec3(0.0f, 0.0f, 1.0f))
#define CreateScale(scale) glm::scale(Mat4Identity, scale)

//	顶点的数据格式
enum VertexFormat {POSITIONS, POSITIONS_TEXTURES, POSITIONS_NORMALS, POSITIONS_NORMALS_TEXTURES};

// 纹理种类定义
enum TextureType {
	TEXTURE_DIFFUSE = 0, TEXTURE_SPECULAR = 1, TEXTURE_REFLECT = 2, TEXTURE_NORMAL = 3, TEXTURE_ROUGHNESS = 4,
	TEXTURE_HEIGHT = 5, TEXTURE_AMBIENT_OCCLUSION = 6
};

#endif // TYPES_H
