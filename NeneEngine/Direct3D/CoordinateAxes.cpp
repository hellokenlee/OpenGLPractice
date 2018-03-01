/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "CoordinateAxes.h"

const CHAR axesShaderSource[] =
	"\n"
	"cbuffer ConstantBuffer\n"
	"{\n"
	"  float4x4 view;\n"
	"  float4x4 projection;\n"
	"}\n"
	"cbuffer PerObjectConstantBuffer\n"
	"{\n"
	"  float4x4 model;\n"
	"}\n"
	"struct VS_OUT\n"
	"{\n"
	"  float4 position : SV_POSITION;\n"
	"  float4 color : COLOR;\n"
	"};\n"
	"VS_OUT VS(float4 position : POSITION, float4 color : COLOR)\n"
	"{\n"
	"  VS_OUT res;\n"
	"  res.position = mul(position, model);\n"
	"  res.position = mul(res.position, view);\n"
	"  res.position = mul(res.position, projection);\n"
	"  res.color = color;\n"
	"  return res;\n"
	"}\n"
	"float4 PS(float4 position_VS_OUT : SV_POSITION, float4 color : COLOR) : SV_TARGET{\n"
	"  return color;\n"
	"}\n";

const CHAR gridShaderSource[] =
	"cbuffer ConstantBuffer\n"
	"{\n"
	"	float4x4 view;\n"
	"	float4x4 projection;\n"
	"}\n"
	"cbuffer PerObjectConstantBuffer\n"
	"{\n"
	"	float4x4 model;\n"
	"}\n"
	"float4 VS(float4 position : POSITION) : SV_POSITION\n"
	"{\n"
	"	float4 res = position;\n"
	"	res = mul(res, model);\n"
	"	res = mul(res, view);\n"
	"	res = mul(res, projection);\n"
	"	return res;\n"
	"}\n"
	"float4 PS(): SV_TARGET{\n"
	"	return float4(0.5f, 0.5f, 0.5f, 1.0f);\n"
	"}\n";

CoordinateAxes::CoordinateAxes() {
	// 初始化
	mLen = 512.0f;
	gapLen = 10.0f;
	isDrawGrid = true;
	isDrawAxes = true;
	pAxesShader = nullptr;
	pGridShader = nullptr;
	pAxes = nullptr;
	pGrid = nullptr;
	// 坐标轴顶点
	std::vector<FLOAT> verticesAxes = {
	//FLOAT verticesAxes[42] = {
		// POSITION-XYZ       COLOR-RGBA
		0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,// X轴
		mLen, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,// Y轴
		0.0f, mLen, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,// Z轴
		0.0f, 0.0f, mLen,   0.0f, 0.0f, 1.0f, 1.0f
	};
	// 平行于X轴的网格
	std::vector<FLOAT> verticesGrid(((int)(mLen / gapLen + 1) * 2 * 2 * 2 * 3), 0);
	int counter = 0;
	FLOAT x = 0, z = 0;
	while (x < mLen) {
		verticesGrid[counter * 3 + 0] = x;
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = (x == 0 ? 0 : mLen);
		counter++;
		verticesGrid[counter * 3 + 0] = x;
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = -mLen;
		counter++;
		verticesGrid[counter * 3 + 0] = -x;
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = (x == 0 ? 0 : mLen);
		counter++;
		verticesGrid[counter * 3 + 0] = -x;
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = -mLen;
		counter++;
		x += gapLen;
	}
	// 平行于Z轴的网格
	while (z < mLen) {
		verticesGrid[counter * 3 + 0] = (z == 0 ? 0 : mLen);
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = z;
		counter++;
		verticesGrid[counter * 3 + 0] = -mLen;
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = z;
		counter++;
		verticesGrid[counter * 3 + 0] = (z == 0 ? 0 : mLen);
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = -z;
		counter++;
		verticesGrid[counter * 3 + 0] = -mLen;
		verticesGrid[counter * 3 + 1] = 0;
		verticesGrid[counter * 3 + 2] = -z;
		counter++;
		z += gapLen;
	}
	// 
	pAxesShader = new Shader(axesShaderSource, sizeof(axesShaderSource), axesShaderSource, sizeof(axesShaderSource), POSITIONS_COLORS);
	pGridShader = new Shader(gridShaderSource, sizeof(gridShaderSource), gridShaderSource, sizeof(gridShaderSource), POSITIONS);
	//
	pAxes = new Shape(verticesAxes, POSITIONS_COLORS);
	pAxes->setDrawMode(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pGrid = new Shape(verticesGrid, POSITIONS);
	pGrid->setDrawMode(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

CoordinateAxes::~CoordinateAxes() {
	if (pAxes) delete pAxes;
	if (pGrid) delete pGrid;
	if (pAxesShader) delete pAxesShader;
	if (pGridShader) delete pGridShader;
}

void CoordinateAxes::draw(Camera * const cam) {
	if(isDrawAxes) pAxes->draw(pAxesShader, cam);
	if(isDrawGrid) pGrid->draw(pGridShader, cam);
}

void CoordinateAxes::extend() {
	;
}
