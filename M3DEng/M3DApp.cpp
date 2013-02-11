#include <GL\glew.h>
#include <GL\glfw.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "M3DApp.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "RenderSystem.h"

namespace M3D{
	M3DApp::M3DApp(){
		initialized = false;
		running = false;
	}

	M3DApp::~M3DApp(){
	
	}

	bool M3DApp::initialize(int width, int height){
		using std::cout;

		if(initialized){
			cout<<"Application already initilized!\n";
			return false;
		}

		cout<<"Starting initialization.\n";
		screenWidth = width;
		screenHeight = height;

		if(glfwInit() == GL_FALSE){
			cout<<"Could not initialize GLFW!\n";
			return false;
		}

		cout<<"GLFW has been initialized.\n";

		if(glfwOpenWindow(width, height,0 , 0, 0 ,0 ,16 ,0 ,GLFW_WINDOW) == GL_FALSE){
			cout<<"Could not create window!\n";
			glfwTerminate();
			return false;
		}

		cout<<"Window opened.\n";

		if(glewInit() != GLEW_OK){
			cout<<"Could not initialize GLEW!\n";
			glfwTerminate();
			return false;
		}

		cout<<"GLEW has been initialized\n";

		if(!GLEW_VERSION_2_1){
			cout<<"OpenGL version 2.1 must be supported!\n";
			glfwTerminate();
			return false;
		}

		cout<<"OpenGL 2.1 is supported\n";

		if(!resourceManager.initialize()){
			cout<<"ResourceManager initialization failed!\n";
			glfwTerminate();
			return false;
		}

		cout<<"ResourceManager initialized.\n";

		if(!renderSystem.initialize(screenWidth, screenHeight)){
			cout<<"RenderSystem initialization failed!\n";
			glfwTerminate();
			return false;
		}

		cout<<"RenderSystem initialized.\n";

		if(!sceneManager.initialize(&renderSystem)){
			cout<<"SceneManager initialization failed\n";
			glfwTerminate();
			return false;
		}

		cout<<"SceneManager initialized.\n";

		cout<<"Initialization complete!\n";
		initialized = true;
		return true;
	}

	int M3DApp::run(){
		if(!initialized){
			std::cout<<"Application has not been initialized first!\n";
			return EXIT_FAILURE;
		}

		std::cout<<"Application starting.\n";
		
		running = true;
		//do stuff

		GLuint vert = resourceManager.loadShader(GL_VERTEX_SHADER, "light_120.vert");
		GLuint frag = resourceManager.loadShader(GL_FRAGMENT_SHADER, "light_120.frag");
		std::vector<GLuint> shaders;
		shaders.push_back(vert);
		shaders.push_back(frag);

		GLuint prog = resourceManager.createProgram(shaders);

		sceneManager.setGlobalLightDir(glm::vec3(20.0f));
		sceneManager.setGlobalLightIntensity(glm::vec4(1.0f));
		sceneManager.setAmbientLightIntensity(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

		
		Mesh* mesh = resourceManager.loadObjFile("land.obj");
		if(mesh == NULL){
			std::cout<<"Mesh was Null, shuting down\n";
			//shutdown();
		}
		//mesh->setOriginOffset(glm::vec3(-0.5f, -0.5f, -0.5f));
		/*
		Mesh* mesh = new Mesh();
		std::vector<glm::vec4>* verts = mesh->getVerticies();
		std::vector<GLushort>* faces = mesh->getElements();

		verts->push_back(glm::vec4(0.75f, 0.75f, 0.0f, 1.0f));
		verts->push_back(glm::vec4(0.75f, -0.75f, 0.0f, 1.0f));
		verts->push_back(glm::vec4(-0.75f, -0.75f, 0.0f, 1.0f));

		faces->push_back(0);
		faces->push_back(1);
		faces->push_back(2);

		mesh->calculateNormals();
		mesh->setupBuffers();
		*/
		Material mat;
		mat.setProgram(prog);
		mat.setDiffuseColor(glm::vec4(0.0, 0.7f, 0.0f, 1.0f));
		Entity entity;
		entity.setMaterial(&mat);
		entity.setMesh(mesh);
		entity.setVisible(true);
		//entity.setPosition(glm::vec3(-0.5f, -0.5f, -0.5f));

		sceneManager.addEntity(&entity);
		
		Camera *camera = sceneManager.getMainCamera();
		camera->setPosition(glm::vec3(2.0f, 1.0f, 0.0f));
		camera->setTarget(glm::vec3(0.0f, 0.0f, 0.0f));

		float dir = 1;
		double lastTime = 0;
		float camDeg = 0;
		while(running){
			double currentTime = glfwGetTime();
			
			
			
			

			if(glfwGetKey( GLFW_KEY_ESC) || !glfwGetWindowParam( GLFW_OPENED )){
				delete mesh;
				shutdown();
			}
		}

		return EXIT_SUCCESS;
	}

	void M3DApp::update(double time){
		Camera *camera = sceneManager.getMainCamera();
		/*
			glm::vec3 pos = entity.getPosition();
			if(pos.y > 2 || pos.y < -2)
				dir = -dir;
			
			pos.y += 0.0001 * dir;
			entity.setPosition(pos);
			*/

			
			/*
			glm::vec3 scale = entity.getScale();
			if(scale.x > 2 || scale.x <0.5)
				dir = -dir;

			scale.x += 0.0001 * dir;
			entity.setScale(scale);
			*/
			
			glm::vec3 camPos = camera->getPosition();
			camDeg += .01;
			if(camDeg >= 360)
				camDeg = 0;
			camPos.x = cos(camDeg) * 2;
			camPos.z = sin(camDeg) * 2;
			camera->setPosition(camPos);

			/*glm::vec3 entityRot = entity.getRotation();
			camDeg += .01;
			if(camDeg >= 360)
				camDeg = 0;
			entityRot.y = camDeg;
			
			entity.setRotation(entityRot); */
	}

	void M3DApp::render(double time){
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Swap front and back rendering buffers
			sceneManager.renderScene();
			glfwSwapBuffers();
	}

	void M3DApp::shutdown(){
		std::cout<<"Shutting down application.\n";
		if(initialized){
			glfwTerminate();

			initialized = false;
		}

		running = false;
	}
}