/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Skybox.h"

const CHAR skyboxShaderSourceVS[] =
	"//--------------------------------------------------------------------------------------\n"
	"// Constant Buffer Variables\n"
	"//--------------------------------------------------------------------------------------\n"
	"cbuffer ConstantBuffer\n"
	"{\n"
	"	float4x4 view;\n"
	"	float4x4 projection;\n"
	"}\n"
	"cbuffer PerObjectConstantBuffer\n"
	"{\n"
	"	float4x4 model;\n"
	"}\n"
	"//--------------------------------------------------------------------------------------\n"
	"// Output structure\n"
	"//--------------------------------------------------------------------------------------\n"
	"struct VS_OUT\n"
	"{\n"
	"	float4 position : SV_POSITION;\n"
	"	float3 texCoord : TEXCOORD;\n"
	"};\n"
	"//--------------------------------------------------------------------------------------\n"
	"// Veretex Shader function\n"
	"//--------------------------------------------------------------------------------------\n"
	"VS_OUT VS(float3 position : POSITION)\n"
	"{\n"
	"	VS_OUT res;\n"
	"	// 移除view矩阵的平移分量 !FIXME: HLSL有没有更好的写法?\n"
	"	float4x4 view_without_translate = {\n"
	"		view._m00, view._m01, view._m02, 0.0f,\n"
	"		view._m10, view._m11, view._m12, 0.0f,\n"
	"		view._m20, view._m21, view._m22, 0.0f,\n"
	"		0.0f,      0.0f,      0.0f,      1.0f\n"
	"	};\n"
	"	// 深度值永远设为1\n"
	"	res.position = float4(position, 1.0f);\n"
	"	// 进行mvp乘法\n"
	"	res.position = mul(res.position, model);\n"
	"	res.position = mul(res.position, view_without_translate);\n"
	"	res.position = mul(res.position, projection);\n"
	"	// 把z值设为1\n"
	"	res.position = res.position.xyww;\n"
	"	// 把纹理坐标设置为位置\n"
	"	res.texCoord = position;\n"
	"	return res;\n"
	"}\n";

const CHAR skyboxShaderSourcePS[] =
	"//--------------------------------------------------------------------------------------\n"
	"// Texture Variables\n"
	"//--------------------------------------------------------------------------------------\n"
	"TextureCube textureSkyMap;\n"
	"SamplerState samplerLinear;\n"
	"//--------------------------------------------------------------------------------------\n"
	"// Input structue\n"
	"//--------------------------------------------------------------------------------------\n"
	"struct PS_IN\n"
	"{\n"
	"	float4 position : SV_POSITION;\n"
	"	float3 texCoord : TEXCOORD;\n"
	"};\n"
	"//--------------------------------------------------------------------------------------\n"
	"// Pixel Shader function\n"
	"//--------------------------------------------------------------------------------------\n"
	"float4 PS(PS_IN ps_in) : SV_TARGET\n"
	"{\n"
	"	//return float4(ps_in.texCoord, 1.0f);\n"
	"	return textureSkyMap.Sample(samplerLinear, ps_in.texCoord);\n"
	"}\n";

Skybox::Skybox(const WCHAR * filePath[6]) : 
	sphere(nullptr), 
	cubeTexture(filePath), 
	shader(skyboxShaderSourceVS, sizeof(skyboxShaderSourceVS), skyboxShaderSourcePS, sizeof(skyboxShaderSourcePS), POSITIONS){
	// 天空盒模型->一个球
	sphere = Geometry::createIcosphere(1, CLOCK_WISE);
	// 初始化深度缓冲描述符
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(dssDesc));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	// 创建属于天空盒的深度缓冲上下文
	HRESULT hr = Utils::getDevice()->CreateDepthStencilState(&dssDesc, &pDSState);
	checkResult(hr, "Failed to create depth-stencil-state.");
}


Skybox::~Skybox() {
	if (pDSState) pDSState->Release();
}

void Skybox::draw(Camera *pCamera) {
	Utils::getContext()->OMGetDepthStencilState(&pPrevDSState, &prevStencilRef);
	Utils::getContext()->OMSetDepthStencilState(pDSState, 0);
	Utils::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cubeTexture.use();
	sphere->draw(&shader, pCamera);
	Utils::getContext()->OMSetDepthStencilState(pPrevDSState, prevStencilRef);
}

