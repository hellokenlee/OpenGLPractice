/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <windows.h>
#include <math.h>
#include "TextureLoader\SimpleMath.h"

#define NDEBUG

#define Vec2 DirectX::SimpleMath::Vector2
#define Vec3 DirectX::SimpleMath::Vector3
#define Vec4 DirectX::SimpleMath::Vector4
#define Mat4 DirectX::SimpleMath::Matrix
#define Mat4Identity DirectX::SimpleMath::Matrix::Identity

#define M_PI 3.141592653f
#define M_PI_TIMES_2 6.283185306f
#define M_PI_OVER_2 1.5707963265f

#define radians(fDegrees) DirectX::XMConvertToRadians(fDegrees)

static inline FLOAT randf(FLOAT lb = 0.0f, FLOAT ub = 1.0f) {
	return lb + (((FLOAT)rand() / (FLOAT)RAND_MAX) *(ub - lb));
}

static inline Vec3 randVec3() {
	FLOAT u = randf();
	FLOAT v = randf();
	FLOAT theta = M_PI_TIMES_2 * u;
	FLOAT phi = acosf(2.0f * v - 1.0f);
	return Vec3(sin(theta) * sin(phi), cos(theta) * sin(phi), cos(phi));
}

// 顶点格式定义
enum VertexFormat {
	NO_FORMAT = 0, POSITIONS = 3, POSITIONS_COLORS = 7, POSITIONS_TEXTURES = 5, POSITIONS_NORMALS_TEXTURES = 8, POSITIONS_NORMALS_COLORS = 10,
	POSITIONS_NORMALS_TEXTURES_IMODELS,
};

// 顶点顺序定义
enum VertexOrder {
	CLOCK_WISE, COUNTER_CLOCK_WISE
};

// 常量缓冲定义
enum ConstantBufferType{
	UPDATE_PER_FRAME, UPDATE_PER_OBJECT
};

// 光源种类定义
enum LightType {
	DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT
};

// 纹理种类定义
enum TextureType {
	TEXTURE_DIFFUSE = 1, TEXTURE_SPECULAR = 2, TEXTURE_NORMAL = 3, TEXTURE_ROUGHNESS = 4, TEXTURE_HEIGHT = 5, TEXTURE_AMBIENT_OCCLUSION = 6
};

enum PolygonMode {
	MODE_POLYGON_CULL = 0, MODE_WIREFRAME = 1, POLYGON_MODES_NUM = 2
};

enum ShaderType {
	VERTEX_SHADER, PIXEL_SHADER, GEOMETRY_SHADER, GEOMETRY_SHADER_STREAM_OUT
};