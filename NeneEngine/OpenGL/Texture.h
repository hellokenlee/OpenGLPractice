/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string.h>
#include <FreeImage.h>
#include <memory>

#include "Types.h"
#include "Utils.h"

//
//	纹理类（用于Model类）
//

class Texture {
public:
    // 参数构造
    Texture(
        const char* filepath,				//路径
        GLenum image_format = GL_BGR,		//图片数据格式
        GLint internal_format = GL_RGB,		//内部数据格式
        GLint level = 0,					//多级纹理(Mipmapping) level
        GLint border = 0,					//边界宽度
        GLenum wrap = GL_REPEAT,			//环绕模式(处理越界访问)
        GLenum filter = GL_LINEAR			//过滤模式(处理上采样)
    );
     // 析构
    ~Texture();
    // 绑定纹理
    void inline use(const GLuint texSlot = 0) {
        glActiveTexture(GL_TEXTURE0 + texSlot);
        glBindTexture(GL_TEXTURE_2D, texID);
    }
    // 获取图片数据
    static std::unique_ptr<BYTE[]> loadImage(const char* filepath, GLuint &width, GLuint &height);
    //
    GLuint inline getTexID() {
        return texID;
    }
private:
    // 禁止拷贝
    Texture();
    Texture(const Texture& rhs);
    Texture& operator=(const Texture& rhs);
    // 纹理ID
    GLuint texID;
};

#endif	//	TEXTURE_H
