#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <cmath>
#include<vector>

#define _USE_MATH_DEFINES
// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"in vec2 position;\n"
"in vec3 color;\n"
"out vec3 Color;\n"
"void main()\n"
"{\n"
" Color = color;\n"
" gl_Position = vec4(position, 0.0, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 Color;\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
" outColor = vec4(Color, 1.0);\n"
"}\n\0";

class Planet {
public:
	float x, y, size, radius, angularVelocity, angle, r, g, b;
	float dx, dy;
	static float velocityFactor;
	static std::vector<GLfloat>* global_vertices_vect;
	std::vector<int> indices_vect;

	Planet() {}

	Planet(float size, float radius, float r, float g, float b) {
		this->size = size;
		this->angle = 0;
		this->radius = radius;

		float velocity = velocityFactor * pow(radius, -0.5f);
		this->angularVelocity = velocity / radius;

		this->r = r;
		this->g = g;
		this->b = b;

		calculatePosition();
	}

	void generateGeometry(std::vector < GLuint>* global_indices = NULL, int steps = 20, std::vector<GLfloat>* global_vert = NULL) {
		if (!global_indices) return;
		if (!global_vert && !(this->global_vertices_vect)) return;
		else if (this->global_vertices_vect) {
			global_vert = this->global_vertices_vect;
		}

		if (!(this->global_vertices_vect)) this->global_vertices_vect = global_vert;

		std::vector<float> current = { this->x, this->y + this->size }, nextStep = { 0,0 };
		float pi = 2 * std::acos(0);
		int indices_offset = global_vert->size() / 5;

		//center
		global_vert->push_back(this->x);
		global_vert->push_back(this->y);
		global_vert->push_back(this->r);
		global_vert->push_back(this->g);
		global_vert->push_back(this->b);

		int center_vertex_index = indices_offset;
		indices_offset++;
		this->indices_vect.push_back(center_vertex_index);

		//initial vertex
		global_vert->push_back(current[0]);
		global_vert->push_back(current[1]);
		global_vert->push_back(this->r);
		global_vert->push_back(this->g);
		global_vert->push_back(this->b);
		this->indices_vect.push_back(indices_offset);
		int first_vertex_index = indices_offset;

		for (int i = 0; i < steps - 1; i++) {


			float angle = (i + 1) / (float)steps * 2 * pi;

			nextStep[0] = this->x + std::sin(angle) * this->size;
			nextStep[1] = this->y + std::cos(angle) * this->size;

			global_vert->push_back(nextStep[0]);
			global_vert->push_back(nextStep[1]);
			global_vert->push_back(this->r);
			global_vert->push_back(this->g);
			global_vert->push_back(this->b);

			this->indices_vect.push_back(indices_offset + 1);

			global_indices->push_back(indices_offset); //current vertex index
			global_indices->push_back(indices_offset + 1); // next vertex index
			global_indices->push_back(center_vertex_index);

			indices_offset++;
			current.swap(nextStep);
		}

		global_indices->push_back(indices_offset); // n-th vertex index
		global_indices->push_back(first_vertex_index); // 1st vertex index
		global_indices->push_back(center_vertex_index); // center
	}


	void calculatePosition() {
		float x_next = radius * cos(angle),
			y_next = radius * sin(angle);

		dx = x_next - x;
		dy = y_next - y;

		x = x_next;
		y = y_next;
	}

	void move() {
		angle += angularVelocity;

		calculatePosition();

		for (int i = 0; i < indices_vect.size(); i++) {
			(*global_vertices_vect)[indices_vect[i] * 5] += dx;
			(*global_vertices_vect)[indices_vect[i] * 5 + 1] += dy;
		}
	}
};

float Planet::velocityFactor = 0.001f;
std::vector<GLfloat>* Planet::global_vertices_vect = NULL;

void drawCircle(GLfloat x, GLfloat y, GLfloat r) {
	static const double inc = 3.14 / 30;
	static const double max = 2 * 3.14;
	glBegin(GL_LINE_LOOP);

	for (double d = 0; d < max; d += inc) {
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

int main() {
	// Initialize GLFW
	glfwInit();



	GLFWwindow* window = glfwCreateWindow(800, 800, "Opengl-window", NULL, NULL);
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
	// 
	// Vertex Shader source code
	const char* vertexShaderSource = "#version 330 core\n"
		"in vec2 position;\n"
		"in vec3 color;\n"
		"out vec3 Color;\n"
		"void main()\n"
		"{\n"
		" Color = color;\n"
		" gl_Position = vec4(position, 0.0, 1.0);\n"
		"}\0";
	//Fragment Shader source code
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"in vec3 Color;"
		"out vec4 outColor;\n"
		"void main()\n"
		"{\n"
		" outColor = vec4(Color, 1.0);\n"
		"}\n\0";

	// Utwórz obiekt Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Po³¹cz istniej¹cy obiekt z napisan¹ wczeœniej implementacj¹ shadera
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Skompiluj gotowy kod
	glCompileShader(vertexShader);

	//status kompilacji shadera wierzcholkow
	GLint vStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vStatus);
	if (vStatus == GL_TRUE) std::cout << "vStatus: GL_TRUE" << std::endl;

	// Powtórz operacjê dla fragment shadera
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//status kompilacji shadera fragmentow
	GLint fStatus;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fStatus);
	if (fStatus == GL_TRUE) std::cout << "fStatus: GL_TRUE" << std::endl;

	// Utwórz program
	GLuint shaderProgram = glCreateProgram();
	// Pod³¹cz shadery pod program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	// Usuñ niepotrzebne shadery
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);


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

	/*const int indicesNumber = 3 * n;
	const int verticesNumber = 5 * indicesNumber;

	GLfloat vertices[verticesNumber];
	GLuint indices[indicesNumber];

	for (int i = 0; i < indicesNumber; i++) {
		indices[i] = i;
	}*/


	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	Planet::global_vertices_vect = &vertices;

	for (int i = 0; i < Planets_vector.size(); i++) {
		Planets_vector[i].generateGeometry(&indices, 20);
		int a = Planets_vector[i].indices_vect.size();
	}

	GLuint VAO, VBO, EBO;
	// Utwórz obiekty VBO, VAO i EBO, ka¿dy posiada jeden obiekt

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Po³¹cz wierzcho³ki z bufforem wierzcho³ków
	glBindVertexArray(VAO);
	// Ustaw typ VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// za³aduj przygotowane wierzcho³ki
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

	// Skonfiguruj format buffora, typ danych i d³ugoœæ
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	// Uruchom buffor
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// 
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window))
	{
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);

		for (int i = 0; i < Planets_vector.size(); i++) {
			Planets_vector[i].move();
		}

		// Ustaw kolor t³a (RGBA, z przedzia³u <0, 1>)
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Wyczyœæ buffor I nadaj mu kolor
		glClear(GL_COLOR_BUFFER_BIT);

		drawCircleFULL(0.0, 0.0, 0.1);
		drawCircle(0.0, 0.0, 0.2);
		drawCircle(0.0, 0.0, 0.3);
		drawCircle(0.0, 0.0, 0.4);
		drawCircle(0.0, 0.0, 0.5);
		drawCircle(0.0, 0.0, 0.6);
		drawCircle(0.0, 0.0, 0.7);
		drawCircle(0.0, 0.0, 0.8);
		drawCircle(0.0, 0.0, 0.9);


		// Wybierz, który shader bêdzie u¿ywany
		glUseProgram(shaderProgram);
		//glBindVertexArray(VAO);
		// Narysuj trójk¹t
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)(0 * sizeof(GLuint)));
		// Odœwie¿ widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}