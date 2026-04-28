#include "scenebasic_uniform.h"

#define STB_IMAGE_IMPLEMENTATION
#include "helper/stb/stb_image.h"

#include <cstdio>
#include <cstdlib>

#include <sstream>

#include <string>
#include <filesystem>

using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::mat4; //lab2
using glm::vec4;
using glm::mat3;

#include <glm/gtc/constants.hpp>

//image


//below used to be angl(0.0f)

SceneBasic_Uniform::SceneBasic_Uniform() : plane (10.0f, 10.0f, 100, 100), angle(0.0f), tPrev(0.0f), rotSpeed(glm::radians(10.0f)) {

    mesh = ObjMesh::load("media/pig_triangulated.obj", true);
    mesh2 = ObjMesh::load("media/eyeball.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
 

    //texture 
    int width, height, channels;
    unsigned char* data = stbi_load("media/brat-album-font.png", &width, &height, &channels, 4);
    glGenTextures(1, &eyeballTex);
    glBindTexture(GL_TEXTURE_2D, eyeballTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    //textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //fog
    prog.setUniform("FogColor", vec3(0.7f, 0.7f, 0.8f)); // should be greylish blue
    prog.setUniform("FogStart", 2.0f);//0.5 for strong | weaker = 2
    prog.setUniform("FogEnd", 12.0f);// 3.0 for strong | weaker = 12


    //cube 
    projection = mat4(1.0f);
    angle = glm::radians(90.0f);
    cubeTex = Texture::loadHdrCubeMap("media/pisa-hdr/pisa");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);//binding
    //skybox
    sky = new SkyBox(50.0f);

    

    //set the camera and the 3 lights 
    view = glm::lookAt(vec3(3.6f, 0.75f, 0.75f),
        vec3(0.0f, 0.1f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
    // Single light for per-fragment Phong
    vec3 lightPosEye = vec3(view * vec4(-2.0f, 4.0f, 2.0f, 1.0f));
    prog.setUniform("LightPosition", lightPosEye);
    prog.setUniform("LightLa", vec3(0.2f));
    prog.setUniform("LightLd", vec3(0.8f));
    prog.setUniform("LightLs", vec3(0.8f));

  
}

void SceneBasic_Uniform::compile()
{
    std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
	try {
      

        prog.compileShader("shader/PerFragment_phong.vert");
        prog.compileShader("shader/PerFragment_phong.frag");

		prog.link();
		prog.use();

        skyProg.compileShader("shader/skybox.vert");
        skyProg.compileShader("shader/skybox.frag");
        skyProg.link();

	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	//update your angle here
  //  if (m_animate) {
    //    angle += 0.1f;
      //  if (angle >= 360.0f) angle -= 360.0f;
    //}
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0F;
    tPrev = t;

    angle += rotSpeed * deltaT;
    if (angle > glm::two_pi<float>())
        angle -= glm::two_pi<float>();
   

}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //skybox
    glDepthMask(GL_FALSE);
    skyProg.use();

    mat4 viewNoTrans = mat4(mat3(view));
    mat4 MVP = projection * viewNoTrans;

    skyProg.setUniform("MVP", MVP);
    skyProg.setUniform("SkyBoxTex", 0);
    sky->render();
    glDepthMask(GL_TRUE);

    prog.use();

    
    prog.setUniform("UseTexture", 0);
    prog.setUniform("Kd", 1.0f, 0.6f, 0.7f);
    prog.setUniform("Ks", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Ka", 0.6f, 0.3f, 0.4f);
    prog.setUniform("Shininess", 200.0f);

    
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    mesh->render();

    //for eye
    prog.setUniform("UseTexture", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, eyeballTex);
    prog.setUniform("Tex", 0);

    ///material for eyeball

    prog.setUniform("Kd", 1.0f, 1.0f, 1.0f);
    prog.setUniform("Ks", 0.2f, 0.2f, 0.2f);
    prog.setUniform("Ka", 0.3f, 0.3f, 0.3f);
    prog.setUniform("Shininess", 50.0f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(1.5f, 0.0f, 0.0f));
    model = glm::scale(model, vec3(0.2f));
    setMatrices();
    mesh2->render();


    prog.setUniform("Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Shininess", 180.0f);
    prog.setUniform("UseTexture", 0);


    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();

   
}

void SceneBasic_Uniform::setMatrices() 
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", 
        glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::resize(int w, int h)
{
  //  width = w;
  //  height = h;
    glViewport(0,0,w,h);
    float aspect = (float)w / h;
    projection = glm::perspective(glm::radians(60.0f), aspect, 0.3f, 100.0f);
}
