#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <cmath>
#include<vector>
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"shaderClass.h" 
#include"Planet.h"
#include"Camera.h"
//#include"Texture.h"



#define _USE_MATH_DEFINES

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

const unsigned int width = 800, heigth = 800;


float Planet::velocityFactor = 0.001f;
std::vector<GLfloat>* Planet::global_vertices_vect = NULL;

void drawCircle(GLfloat x, GLfloat y, GLfloat r) {
	static const double inc = 3.14 / 30;
	static const double max = 2 * 3.14;
	glBegin(GL_LINE_LOOP);

	for (double d = 0; d < max; d += inc)  {
		glVertex2f(cos(d) * r + x, sin(d) * r + y);
	}
	glEnd();
}
void drawCircleFULL(GLfloat x, GLfloat y, GLfloat r) {
	static const double inc = 3.14 / 12;
	static const double max = 2 * 3.14;
	glBegin(GL_POLYGON);
	for (double d = 0; d < max; d += inc) {
		glVertex2f(cos(d) * r + x, sin(d) * r + y);
	}
	glEnd();
}

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main() {
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);       //informacje o wersji biblioteki
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, heigth, "Opengl-window", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" <<
			std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window

	std::vector<Planet> Planets_vector;
	// Planet(size, radius, red, green, blue);
	Planets_vector.push_back(Planet(0.02f, 0.2f, 1.0f, 1.0f, 1.0f));
	Planets_vector.push_back(Planet(0.01f, 0.2f, 1.0f, 1.0f, 0));
	Planets_vector.push_back(Planet(0.03f, 0.4f, 0, 0, 1.0f));
	Planets_vector.push_back(Planet(0.02f, 0.5f, 1.0f, 0, 0));
	Planets_vector.push_back(Planet(0.08f, 0.6f, 1.0f, 0.2f, 0.2f));
	Planets_vector.push_back(Planet(0.07f, 0.7f, 1.0f, 0.1f, 0.1f));
	Planets_vector.push_back(Planet(0.06f, 0.8f, 0.2f, 1.0f, 0.2f));
	Planets_vector.push_back(Planet(0.05f, 0.9f, 0, 1.0f, 0));

	Planet::global_vertices_vect = &vertices;

	for (int i = 0; i < Planets_vector.size(); i++) {
		Planets_vector[i].generateGeometry(&indices, 20);
		int a = Planets_vector[i].indices_vect.size();
	}

	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices.data(), sizeof(GLfloat) * vertices.size());
	EBO EBO1(indices.data(), sizeof(GLuint) * indices.size());

	VAO1.LinkVBO(VBO1, 0, 1);

	// Links VBO attributes such as coordinates and colors to VAO
	//VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	//VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	//VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	//VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));


	//VAO1.Unbind();
	//VBO1.Unbind();
	//EBO1.Unbind();



	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	shaderProgram.Activate();

	glViewport(0, 0, width, heigth);

	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	Camera camera(width, heigth, glm::vec3(2.f, 2.f, 2.0f));
	
	// Tells OpenGL which Shader Program we want to use
	lightShader.Activate();

	while (!glfwWindowShouldClose(window))
	{
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		


		camera.Inputs(window);
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");

		
		// Bind the VAO so OpenGL knows to use it
		lightVBO.Bind();
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) /*/ sizeof(int)*/, GL_UNSIGNED_INT, 0);
		lightVAO.Unbind();
		lightVBO.Unbind();
		
		
		
		/*drawCircleFULL(0.0, 0.0, 0.1);
		drawCircle(0.0, 0.0, 0.2);
		drawCircle(0.0, 0.0, 0.3);
		drawCircle(0.0, 0.0, 0.4);
		drawCircle(0.0, 0.0, 0.5);
		drawCircle(0.0, 0.0, 0.6);
		drawCircle(0.0, 0.0, 0.7);
		drawCircle(0.0, 0.0, 0.8);
		drawCircle(0.0, 0.0, 0.9);*/
		
		//VBO1.Unbind();

		for (int i = 0; i < Planets_vector.size(); i++) {
			Planets_vector[i].move();
		}

		// Ustaw kolor t³a (RGBA, z przedzia³u <0, 1>)
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Wyczyœæ buffor I nadaj mu kolor
		glClear(GL_COLOR_BUFFER_BIT);


		


		//glBindVertexArray(VAO);
		// Narysuj trójk¹t
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		VAO1.Bind();
		VBO1.Bind();
		
		//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
		VBO1.Data(vertices.data(), sizeof(GLfloat) * vertices.size());
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));

		VAO1.Unbind();
		VBO1.Unbind();
		// Odœwie¿ widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}