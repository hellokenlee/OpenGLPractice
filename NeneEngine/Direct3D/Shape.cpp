/*Copyright reserved by KenLee@2018 hellokenlee@163.com*/
#include "Shape.h"

using namespace DirectX::SimpleMath;

Shape::Shape() :
	pVertexBuffer(nullptr),
	verticesNum(0),
	vertexFormat(NO_FORMAT),
	perVertexSize(0),
	offset(0),
	indicesNum(0),
	pIndexBuffer(nullptr),
	drawMode(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
	model(Mat4Identity),
	scale(1.0f),
	position(0.0f, 0.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f)
{}

Shape::Shape(FLOAT* vertices, UINT vArrayLen, VertexFormat vf) {
	// 成员初始化数据
	pVertexBuffer = nullptr;                           // 顶点缓冲初始化
	pIndexBuffer = nullptr;                            // 索引缓冲初始化
	vertexFormat = vf;                                 // 记录顶点格式
	offset = 0;                                        // 偏移为0
	indicesNum = 0;                                    // 非索引模式
	perVertexSize = (UINT)vertexFormat * sizeof(FLOAT);// 计算每个顶点所占位宽
	verticesNum = vArrayLen / vertexFormat;            // 计算顶点数量
	model = Mat4Identity;                              // Model矩阵初始化为单位矩阵
	drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;  // 默认绘制模式为三角形
	scale = Vec3(1.0f);
	//
	dLogIf(vArrayLen % vertexFormat != 0, "[Error] Vertex Array's length(%d) is not aligned in decleared format(%d).\n", vArrayLen, vertexFormat);
	// 使用显存描述符表示显存格式
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = perVertexSize * verticesNum;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// 准备初始化数据
	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;
	// 申请显存同时写入数据
	HRESULT hr = S_OK;
	hr = Utils::getDevice()->CreateBuffer(&vertexBufferDesc, &initData, &pVertexBuffer);
	checkResult(hr, "[Error] Failded to create vertex buffer.");
}

Shape::Shape(FLOAT* vertices, UINT vArrayLen, UINT* indices, UINT iArrayLen, VertexFormat vf) {
	// 成员初始化数据
	pVertexBuffer = nullptr;                           // 顶点缓冲初始化
	pIndexBuffer = nullptr;                            // 索引缓冲初始化
	vertexFormat = vf;                                 // 记录顶点格式
	offset = 0;                                        // 偏移为0
	indicesNum = iArrayLen;                            // 计算索引数量
	perVertexSize = (UINT)vertexFormat * sizeof(FLOAT);// 计算每个顶点所占位宽
	verticesNum = vArrayLen / vertexFormat;            // 计算顶点数量
	model = Mat4Identity;                          // Model矩阵初始化为单位矩阵
	drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;  // 默认绘制模式为三角形
	scale = Vec3(1.0f);
	//
	dLogIf(vArrayLen % vertexFormat != 0, "[Error] Vertex Array's length(%d) is not aligned in decleared format(%d).\n", vArrayLen, vertexFormat);
	// 初始化索引缓冲描述符
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indicesNum;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// 初始化索引数据
	D3D11_SUBRESOURCE_DATA iInitData;
	ZeroMemory(&iInitData, sizeof(iInitData));
	iInitData.pSysMem = indices;
	// 写入到索引缓冲
	HRESULT hr = S_OK;
	hr = Utils::getDevice()->CreateBuffer(&indexBufferDesc, &iInitData, &pIndexBuffer);
	checkResult(hr, "Failded to create index buffer.");
	// 初始化顶点缓冲描述符
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = perVertexSize * verticesNum;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// 初始化顶点数据
	D3D11_SUBRESOURCE_DATA vInitData;
	ZeroMemory(&vInitData, sizeof(vInitData));
	vInitData.pSysMem = vertices;
	// 写入到顶点缓冲
	hr = Utils::getDevice()->CreateBuffer(&vertexBufferDesc, &vInitData, &pVertexBuffer);
	checkResult(hr, "Failded to create vertex buffer.");
}

void Shape::draw(Shader* const shader, Camera* const cam, BOOL isUpdateCB) {
	// 如果着色器不为空， 使用该着色器
	if (shader) shader->use();
	if (cam) cam->use();
	// 更新Model矩阵
	if (isUpdateCB) {
		ConstantBuffer::perObjectCBData.model = model.Transpose();
		ConstantBuffer::updatePerObject();
	}
	// 设置顶点缓冲
	Utils::getContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &perVertexSize, &offset);
	// 设置绘制模式
	Utils::getContext()->IASetPrimitiveTopology(drawMode);
	// 分模式绘制
	if (indicesNum == 0) {
		// 非索引模式
		Utils::getContext()->Draw(verticesNum, 0);
	} else {
		// 索引模式
		Utils::getContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		Utils::getContext()->DrawIndexed(indicesNum, 0, 0);
	}
}

void Shape::moveTo(Vec3 const _position) {
	position = _position;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}

void Shape::scaleTo(Vec3 const _scale) {
	scale = _scale;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
	
}
void Shape::rotateX(FLOAT const rox) {
	rotation.x = rox;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}
void Shape::rotateY(FLOAT const roy) {
	rotation.y = roy;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}
void Shape::rotateZ(FLOAT const roz) {
	rotation.z = roz;
	model = Matrix::CreateScale(scale);
	model *= Matrix::CreateRotationX(rotation.x);
	model *= Matrix::CreateRotationY(rotation.y);
	model *= Matrix::CreateRotationZ(rotation.z);
	model *= Matrix::CreateTranslation(position);
}


void Shape::setDrawMode(D3D11_PRIMITIVE_TOPOLOGY _drawMode) {
	drawMode = _drawMode;
}

Shape::~Shape() {
	if(pVertexBuffer) pVertexBuffer->Release();
	if(pIndexBuffer) pIndexBuffer->Release();
}
