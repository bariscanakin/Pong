//
//  SceneNode.h
//  Ders 7
//
//  Created by Gurel Erceis on 3/31/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//

#ifndef Ders_7_SceneNode_h
#define Ders_7_SceneNode_h
#include <windows.h>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <vector>

class SceneNode{
private:
    glm::mat4 modelMatrix;
public:
    std::vector<SceneNode*> children;
    SceneNode *parent;
    
    SceneNode():modelMatrix(glm::mat4x4(1.0f)),parent(NULL){
        
    }
    virtual ~SceneNode(){
            
    }
    SceneNode* translate(float x, float y, float z){
        modelMatrix = glm::translate(modelMatrix, x,y,z);
        return this;
    }
    SceneNode* rotate(float angle, float x, float y, float z){
        modelMatrix = glm::rotate(modelMatrix, angle, x, y, z);
        return this;
    }
    SceneNode* scale(float x, float y, float z){
        modelMatrix = glm::scale(modelMatrix, x, y, z);
        return this;
    }
    SceneNode* identity(){
        modelMatrix = glm::mat4x4(1.0f);
        return this;
    }
    glm::vec3 getPosition(){
        glm::mat4 matrix = getModelMatrix();
        return glm::vec3(matrix[3][0],matrix[3][1],matrix[3][2]);
    }
    
    glm::mat4 getModelMatrix(){
        glm::mat4 result =  modelMatrix;
        SceneNode* temp = parent;
        while( temp != NULL ){
            result =  temp->getModelMatrix() * result;
            temp = temp->parent;
        }
        return result;
    }
	void setModelMatrix(glm::mat4 modelMatrix)
	{
		this->modelMatrix = modelMatrix;
	}
    glm::mat4 getInverseModelMatrix(){ return glm::inverse(getModelMatrix()); }
    SceneNode* addNode( SceneNode *child){
        children.push_back(child); 
        child->parent = this;
        return child;
    }
    virtual void draw(SceneNode *camera){}
};


#endif
