/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#include "Texture.h"

Texture::Texture(const char* filename, GLenum image_format, GLint internal_format,
                 GLint level, GLint border, GLenum wrap, GLenum filter) {
    // 初始化
    texID = 0;
    std::unique_ptr<BYTE[]> imageData = nullptr;
    // 宽，高
    GLuint width, height;
    // 读取图片
    imageData = loadImage(filename, width, height);
    if(imageData == nullptr || width == 0 || height == 0) {
        dLog("[Error] Broken image data! Could not load texture(%s)\n", filename);
        return;
    }
    // 生产纹理
    glGenTextures(1, &texID);
    // 指定纹理类型
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    // 载入图片数据到纹理
    glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, imageData.get());
    // 恢复状态机状态
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &texID);
}

std::unique_ptr<BYTE[]> Texture::loadImage(const char* filename, GLuint &width, GLuint &height) {
    //
    checkFileExist(filename);
    // 图片格式
    FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
    // 图片数据
    BYTE *pBits = nullptr;
    // 图片指针
    FIBITMAP *pImage = nullptr;
    // 获取格式
    format = FreeImage_GetFileType(filename, 0);
    if(format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(filename);
    if(format == FIF_UNKNOWN) {
        dLog("[Error] Unknown image type (%s)\n", filename);
        return nullptr;
    }
    // 载入图片
    if(FreeImage_FIFSupportsReading(format))
        pImage = FreeImage_Load(format, filename);
    else
        dLog("[Error] Unsupported image type (%s)\n", filename);
    if(!pImage) {
        return nullptr;
    }
    // 检查是否PNG图片，是转换成32位
    if(format == FIF_PNG) {
        FIBITMAP *tmp = pImage;
        pImage = FreeImage_ConvertTo32Bits(pImage);
        FreeImage_Unload(tmp);
    }
    //
    FreeImage_FlipVertical(pImage);
    // 获取数据
    pBits = FreeImage_GetBits(pImage);
    width = FreeImage_GetWidth(pImage);
    height = FreeImage_GetHeight(pImage);
    // 拷贝一份数据返回
    UInt BBP = FreeImage_GetBPP(pImage) / 8; // bits per pixel -> Byte per pixel
    BYTE *res = new BYTE[width * height * BBP];
    memcpy(res, pBits, width * height * BBP);
    // 释放原来的指针
    FreeImage_Unload(pImage);
    // 返回
    return std::unique_ptr<BYTE[]>(res);
}
