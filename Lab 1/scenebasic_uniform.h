#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//lab 2 work
//#include "helper/torus.h"

//lab3 work
#include "helper/plane.h"
#include "helper/objmesh.h"
//skybox
#include "helper/skybox.h"
#include "helper/cube.h"
#include "helper/texture.h"

class SceneBasic_Uniform : public Scene
{
private:

    float angle;
    float tPrev;
    float rotSpeed;

    //skybox
    SkyBox* sky;
    GLSLProgram skyProg;
    GLuint cubeTex;
    //GLuint vaoHandle; LAB1 LINE
   // Torus torus; 

    //texture
    GLuint eyeballTex;
    //skybox
  

    //lab3 plane and objmesh
    Plane plane; //plane surface
    std::unique_ptr<ObjMesh>mesh; //pig mesh
    std::unique_ptr<ObjMesh>mesh2; //for eye

    GLSLProgram prog;


    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

  //  float angle;LAB1 LINE
    //glm::mat4 rotationMatrix;LAB1 LINE





    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);

    void setMatrices();//lab2 

};

#endif // SCENEBASIC_UNIFORM_H

