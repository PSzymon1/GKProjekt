#include"Planet.h"

Planet::Planet(float size, float radius, float r, float g, float b) {
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

void Planet::generateGeometry(std::vector < GLuint>* global_indices = NULL, int steps = 20, std::vector<GLfloat>* global_vert = NULL) {
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


void Planet::calculatePosition() {
	float x_next = radius * cos(angle),
		y_next = radius * sin(angle);

	dx = x_next - x;
	dy = y_next - y;

	x = x_next;
	y = y_next;
}

void Planet::move() {
	angle += angularVelocity;

	calculatePosition();

	for (int i = 0; i < indices_vect.size(); i++) {
		(*global_vertices_vect)[indices_vect[i] * 5] += dx;
		(*global_vertices_vect)[indices_vect[i] * 5 + 1] += dy;
	}
}
