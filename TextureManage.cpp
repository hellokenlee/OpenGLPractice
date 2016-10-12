/*Copyright reserved by KenLee@2016 ken4000kl@gmail.com*/
#include "TextureManager.h"

//static init
TextureManager* TextureManager::manager=nullptr;

TextureManager* TextureManager::getManager(){
    if(TextureManager::manager==nullptr){
        return new TextureManager();
    }
    return manager;
}
TextureManager::TextureManager(){
    texturesMap.clear();
}
TextureManager::~TextureManager(){
    freeAllTextures();
    manager=nullptr;
}

bool TextureManager::loadTexture(const char* filename,const unsigned int texMapID,GLenum image_format,GLint internal_format,GLint level,GLint border,GLenum wrap,GLenum filter){
    //纹理ID
    GLuint texID;
    //图片格式
    FREE_IMAGE_FORMAT fif=FIF_UNKNOWN;
    //图片数据
    BYTE *bits(0);
    //图片指针
    FIBITMAP *image(0);
    //宽，高
    GLuint width,height;
    //检查格式
    fif=FreeImage_GetFileType(filename,0);
    if(fif==FIF_UNKNOWN)
        fif=FreeImage_GetFIFFromFilename(filename);
    if(fif==FIF_UNKNOWN){
        return false;
    }
    //载入图片
    if(FreeImage_FIFSupportsReading(fif))
		image = FreeImage_Load(fif, filename);
    if(!image){
        return false;
    }
    //获取数据
    bits=FreeImage_GetBits(image);
    width=FreeImage_GetWidth(image);
    height=FreeImage_GetHeight(image);
    if((bits == 0) || (width == 0) || (height == 0)){
        return false;
    }
    //生产纹理
    glGenTextures(1,&texID);
    //指定纹理类型
    glBindTexture(GL_TEXTURE_2D,texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    //载入图片数据到纹理
    glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,border, image_format, GL_UNSIGNED_BYTE, bits);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //释放图片
    FreeImage_Unload(image);
    //恢复状态机状态
    glBindTexture(GL_TEXTURE_2D,0);
    //
    texturesMap.insert(std::pair<unsigned int,GLuint>(texMapID,texID));
    return true;
}

bool TextureManager::bindTexture(unsigned int texMapID){
    if(texturesMap.find(texMapID)!=texturesMap.end()){
        glBindTexture(GL_TEXTURE_2D,this->texturesMap[texMapID]);
        return true;
    }
    return false;

}

bool TextureManager::freeTexture(unsigned int texMapID){
    if(texturesMap.find(texMapID)!=texturesMap.end()){
        glDeleteTextures(1, &(texturesMap[texMapID]));
        texturesMap.erase(texMapID);
        return true;
    }
    return false;
}

void TextureManager::freeAllTextures(){
    for(auto it=this->texturesMap.begin();it!=texturesMap.end();++it){
        freeTexture(it->first);
    }
    texturesMap.clear();
}
