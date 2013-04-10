//
//  ShadedGeometry.h
//  Ders 7
//
//  Created by Gurel Erceis on 4/1/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//

#ifndef Ders_7_ShadedGeometry_h
#define Ders_7_ShadedGeometry_h
#include <windows.h>
#include <GL/gl.h>
#include <fstream>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <cmath>
#define BUFFER_OFFSET(i) ((void*)(i))
#define fmax(a,b) (a>b)?a:b

class ShadedGeometry: public Geometry{
public:
    ShadedGeometry(char* filename, char *vertexShaderFile, char *fragmentShaderFile):Geometry(filename){
        programId = LoadShaders(vertexShaderFile, fragmentShaderFile);
    }
    virtual ~ShadedGeometry(){
        glDeleteProgram(programId);
    } 
protected:
    GLuint programId;
private:
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path){
        
        // Create the shaders
        GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        
        // Read the Vertex Shader code from the file
        std::string VertexShaderCode;
        std::ifstream VertexShaderStream(vertex_file_path, std::ifstream::in);
        if(VertexShaderStream.is_open())
        {
            std::string Line = "";
            while(getline(VertexShaderStream, Line))
                VertexShaderCode += "\n" + Line;
            VertexShaderStream.close();
        }
        
        // Read the Fragment Shader code from the file
        std::string FragmentShaderCode;
        std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
        if(FragmentShaderStream.is_open()){
            std::string Line = "";
            while(getline(FragmentShaderStream, Line))
                FragmentShaderCode += "\n" + Line;
            FragmentShaderStream.close();
        }
        
        GLint Result = GL_FALSE;
        int InfoLogLength;
        
        // Compile Vertex Shader
        printf("Compiling Vertex Shaders\n");
        char const * VertexSourcePointer = VertexShaderCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);
        
        // Check Vertex Shader
        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        if(!Result)
        {
            glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> VertexShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
            fprintf(stdout, "VertexShaderErrorMessage: %s\n", &VertexShaderErrorMessage[0]);
        }
        
        // Compile Fragment Shader
        printf("Compiling Fragment Shader\n");
        char const * FragmentSourcePointer = FragmentShaderCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);
        
        // Check Fragment Shader
        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        if(!Result)
        {
            glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
            fprintf(stdout, "FragmentShaderErrorMessage: %s\n", &FragmentShaderErrorMessage[0]);
        }
        
        // Link the program
        fprintf(stdout, "Linking program\n");
        GLuint ProgramID = glCreateProgram();
        glAttachShader(ProgramID, VertexShaderID);
        glAttachShader(ProgramID, FragmentShaderID);
        glLinkProgram(ProgramID);
        
        // Check the program
        glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
        if(!Result)
        {
            glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<char> ProgramErrorMessage( fmax(InfoLogLength, int(1)) );
            glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
            fprintf(stdout, "ProgramErrorMessage: %s\n", &ProgramErrorMessage[0]);
        }
        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        
        return ProgramID;
    }
};


#endif
