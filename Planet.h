#pragma once
#include<vector>
#include<GLFW/glfw3.h>
#include<cmath>

class Planet {
public:
	float x, y, size, radius, angularVelocity, angle, r, g, b;
	float dx, dy;
	static float velocityFactor;
	static std::vector<GLfloat>* global_vertices_vect;
	std::vector<int> indices_vect;

	Planet() {}

	Planet(float size, float radius, float r, float g, float b);

	void generateGeometry(std::vector < GLuint>* global_indices = NULL, int steps = 20, std::vector<GLfloat>* global_vert = NULL);


	void calculatePosition();

	void move();
};
