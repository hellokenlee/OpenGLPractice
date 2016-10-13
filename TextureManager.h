/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

//OpenGL
#include <GL/glew.h>
//FreeImage
#include <FreeImage.h>
//HashMap
#include <unordered_map>

//
//	纹理管理器
//

class TextureManager{
public:
	static TextureManager* getManager();
	~TextureManager();
	//从文件加载纹理
	bool loadTexture(
		const char* filepath,				//路径
		const unsigned int texID,			//纹理ID
		GLenum image_format = GL_BGR,		//图片数据格式
		GLint internal_format = GL_RGB,		//内部数据格式
		GLint level = 0,					//mipmapping level
		GLint border = 0,					//边界宽度
		GLenum wrap = GL_REPEAT,
		GLenum filter = GL_LINEAR
		);
	//绑定纹理
	bool bindTexture(unsigned int texID);
	//释放
	bool freeTexture(unsigned int texID);
	//释放所有
	void freeAllTextures();
private:
    //单例变量
	static TextureManager *manager;
	//单例模式禁止构造/拷贝构造
	TextureManager();
	TextureManager(const TextureManager& tm);
	TextureManager& operator=(const TextureManager& tm);
	//纹理池
	std::unordered_map<unsigned int,GLuint> texturesMap;
};


#endif	//	TEXTURE_MANAGER_H
