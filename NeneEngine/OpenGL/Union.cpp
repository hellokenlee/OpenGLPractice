/*Copyright reserved by KenLee@2017 ken4000kl@gmail.com*/

#include "Union.h"

Union::Union(){
    objList.clear();
    unionModel = glm::mat4();
    isShareModel = false;
}


void Union::addShape(Shape* obj){
    objList.push_back(obj);
    objModel.push_back(obj->getModelMat() * unionModel);
}

void Union::setModel(glm::mat4 mat){
    this->unionModel = mat;
    if(!isShareModel){
        for(unsigned int i = 0; i < objList.size(); ++i){
            objModel[i] = objList[i]->getModelMat() * unionModel;
        }
    }
}

void Union::setShader(Shader* _shader) {
    shader = _shader;
}


void Union::setCamera(Camera* _camera) {
    camera = _camera;
}

void Union::draw(){
    shader->use();

    glUniformMatrix4fv(glGetUniformLocation(shader->programID, "view"), 1, GL_FALSE, camera->getViewMatrixVal());
    glUniformMatrix4fv(glGetUniformLocation(shader->programID, "projection"), 1, GL_FALSE, camera->getProjectionMatrixVal());

    if(isShareModel){
        glUniformMatrix4fv(glGetUniformLocation(shader->programID, "model"), 1, GL_FALSE, glm::value_ptr(unionModel));
        for(unsigned int i = 0; i < objList.size(); ++i){
            objList[i]->draw();
        }
    } else {
        for(unsigned int i = 0; i < objList.size(); ++i){
            glUniformMatrix4fv(glGetUniformLocation(shader->programID, "model"), 1, GL_FALSE, glm::value_ptr(objModel[i]));
            objList[i]->draw();
        }
    }

}
