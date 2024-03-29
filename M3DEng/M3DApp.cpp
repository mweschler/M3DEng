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
	const int UPDATES_PER_SECOND = 60;
	const double MS_PER_UPDATE = 1.0 / UPDATES_PER_SECOND;
	const int MAX_FRAMESKIP = 5;

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

		GLuint vert = resourceManager.loadShader(GL_VERTEX_SHADER, "lightSpec_120.vert");
		GLuint frag = resourceManager.loadShader(GL_FRAGMENT_SHADER, "lightSpec_120.frag");
		std::vector<GLuint> shaders;
		shaders.push_back(vert);
		shaders.push_back(frag);

		GLuint prog = resourceManager.createProgram(shaders);

		GLuint basicVert = resourceManager.loadShader(GL_VERTEX_SHADER, "basic.vert");
		GLuint basicFrag = resourceManager.loadShader(GL_FRAGMENT_SHADER, "basic.frag");
		std::vector<GLuint> basicShaders;
		basicShaders.push_back(basicVert);
		basicShaders.push_back(basicFrag);

		GLuint basicProg = resourceManager.createProgram(basicShaders);

		GLuint difVert = resourceManager.loadShader(GL_VERTEX_SHADER, "light_120.vert");
		GLuint difFrag = resourceManager.loadShader(GL_FRAGMENT_SHADER, "light_120.frag");
		std::vector<GLuint> difShaders;
		difShaders.push_back(difVert);
		difShaders.push_back(difFrag);

		GLuint difProg = resourceManager.createProgram(difShaders);

		sceneManager.setGlobalLightDir(glm::vec3(1.0f));
		sceneManager.setGlobalLightIntensity(glm::vec4(1.0f));
		sceneManager.setAmbientLightIntensity(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

		
		Mesh* landMesh = resourceManager.loadObjFile("land.obj");
		Mesh* boxMesh = resourceManager.loadObjFile("decocube_nf4k.obj");
		if(landMesh == NULL || boxMesh == NULL) {
			std::cout<<"Mesh was Null, shuting down\n";
			shutdown();
		}
		boxMesh->setOriginOffset(glm::vec3(-0.5f, -0.5f, -0.5f));
		
		Material landMat;
		landMat.setProgram(prog);
		landMat.setDiffuseColor(glm::vec4(0.0f, 0.7f, 0.0f, 1.0f));
		landMat.setShininessFactor(2.0f);

		Material box1Mat;
		box1Mat.setProgram(prog);
		box1Mat.setDiffuseColor(glm::vec4(0.4f, 0.0f, 0.8f, 1.0f));
		box1Mat.setShininessFactor(2.0f);

		Material box2Mat;
		box2Mat.setProgram(prog);
		box2Mat.setDiffuseColor(glm::vec4(0.7f, 0.0f, 0.0f, 1.0f));
		box2Mat.setShininessFactor(70.0f);

		Material box3Mat;
		box3Mat.setProgram(prog);
		box3Mat.setDiffuseColor(glm::vec4(0.8f, 0.8f, 0.0f, 1.0f));
		box3Mat.setShininessFactor(7.0f);

		Entity land;
		land.setMaterial(&landMat);
		land.setMesh(landMesh);
		land.setScale(glm::vec3(3.0f));
		land.setVisible(true);
		
		Entity box1;
		box1.setMaterial(&box1Mat);
		box1.setMesh(boxMesh);
		box1.setPosition(0.0f, 1.0f, 0.0f);
		box1.setVisible(true);

		Entity box2;
		box2.setMaterial(&box2Mat);
		box2.setMesh(boxMesh);
		box2.setPosition(-2.0f, 1.0f, 0.0f);
		box2.setVisible(true);

		Entity box3;
		box3.setMaterial(&box3Mat);
		box3.setMesh(boxMesh);
		box3.setPosition(2.0f, 1.0f, 0.0f);
		box3.setVisible(true);

		sceneManager.addEntity(&land);
		sceneManager.addEntity(&box1);
		sceneManager.addEntity(&box2);
		sceneManager.addEntity(&box3);
		
		Camera *camera = sceneManager.getMainCamera();
		camera->setPosition(glm::vec3(5.0f, 3.0f, 5.0f));
		camera->setTarget(glm::vec3(0.0f, 1.0f, 0.0f));

		float dir = 1;
		float scaleDir = 1;
		double lastTime = glfwGetTime();
		float camDeg = 0;
		float boxDeg = 0;
		float camDistance = 5;
		bool autoCamera = true;
		bool spacePressed = false;


		std::cout<<"Starting main loop. Updates per second: "<<UPDATES_PER_SECOND<<" Min MS per update "<< MS_PER_UPDATE<<" Framskip "<<MAX_FRAMESKIP<<std::endl;
		while(running){
			double currentTime = glfwGetTime();
			double elapsedTime = currentTime - lastTime;
			float interpolation;
			int loops = 0;
			
			
			while(currentTime >= lastTime && loops < MAX_FRAMESKIP){
			
				glm::vec3 pos = box3.getPosition();
				if(pos.y > 2 || pos.y < 0.75)
					dir = -dir;
			
				pos.y += 0.01 * dir;
				box3.setPosition(pos);
			
				glm::vec3 scale = box1.getScale();
				if(scale.x > 1.5 || scale.x <0.5)
					scaleDir = -scaleDir;

				scale.x += 0.01 * scaleDir;
				box1.setScale(scale);
				
				if(autoCamera){
					glm::vec3 camPos = camera->getPosition();
					camDeg += .001;
					if(camDeg >= 360)
						camDeg = 0;
					camPos.x = cos(camDeg) * camDistance;
					camPos.z = sin(camDeg) * camDistance;
					camera->setPosition(camPos);
				}

				glm::vec3 entityRot = box2.getRotation();
				boxDeg += 1;
				if(boxDeg >= 360)
					boxDeg = 0;
				entityRot.y = boxDeg;
				entityRot.x = boxDeg;
			
				box2.setRotation(entityRot);

				lastTime += MS_PER_UPDATE;
			}

			interpolation = float( glfwGetTime() + MS_PER_UPDATE - lastTime) / float(MS_PER_UPDATE);

			render(interpolation);

			

			if(glfwGetKey( GLFW_KEY_ESC) || !glfwGetWindowParam( GLFW_OPENED )){
				delete landMesh;
				delete boxMesh;
				shutdown();
			}

			if(glfwGetKey( GLFW_KEY_F1))
			{
				landMat.setProgram(basicProg);
				box1Mat.setProgram(basicProg);
				box2Mat.setProgram(basicProg);
				box3Mat.setProgram(basicProg);
			}

			if(glfwGetKey( GLFW_KEY_F2))
			{
				landMat.setProgram(difProg);
				box1Mat.setProgram(difProg);
				box2Mat.setProgram(difProg);
				box3Mat.setProgram(difProg);
			}
			if(glfwGetKey( GLFW_KEY_F3))
			{
				landMat.setProgram(prog);
				box1Mat.setProgram(prog);
				box2Mat.setProgram(prog);
				box3Mat.setProgram(prog);
			}
			int spaceStatus = glfwGetKey(GLFW_KEY_SPACE);
			if(spaceStatus && !spacePressed){
				spacePressed = true;
			} else if(!spaceStatus && spacePressed){
				spacePressed = false;
				autoCamera = !autoCamera;
			}

			if(!autoCamera){
				glm::vec3 camPos = camera->getPosition();
				glm::vec3 camTarget = camera->getTarget();
				if(glfwGetKey(GLFW_KEY_LEFT)){
					camDeg += .01;
				}
				if(glfwGetKey(GLFW_KEY_RIGHT)){
					camDeg -= .01;
				}

				if(glfwGetKey(GLFW_KEY_UP)){
					camPos.y += .05;
					camTarget.y += .05;
				}
				if(glfwGetKey(GLFW_KEY_DOWN)){
					camPos.y -= .05;
					camTarget.y -= .05;
				}

				if(glfwGetKey('W')){
					camDistance += .05;
				}
				if(glfwGetKey('S')){
					camDistance -= .05;
				}
				if(glfwGetKey('A')){
					camTarget.x -= .05;
				}

				if(glfwGetKey('D')){
					camTarget.x += .05;
				}

				if(camDeg >= 360)
					camDeg = 0;
				
				camPos.x = cos(camDeg) * camDistance + camTarget.x;
				camPos.z = sin(camDeg) * camDistance;
				camera->setPosition(camPos);
				camera->setTarget(camTarget);
			}

		}

		return EXIT_SUCCESS;
	}

	void M3DApp::update(double time){
		
		
	}

	void M3DApp::render(double time){
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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