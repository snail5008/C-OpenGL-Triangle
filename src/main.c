#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int OPENGL_VERSION[2] = { 4, 6 };
float BACKGROUND_COLOUR[4] = {0.1f, 0.1f, 0.1f, 1.0f};

const float triangle_vertices[] = {
     //   positions            colours   //
	 0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
};

// TODO: reallocate some more bytes instead of printing "file too large"
const char* read_file(const char* filename, int max_file_size) {
	char* contents;
	contents = (char*)malloc(max_file_size);
	if (contents == NULL) {
		printf("{ERROR} - failed to allocate %d bytes", max_file_size);
		exit(-1);
	}
	int ptr = 0;
	FILE* f;
	fopen_s(&f, filename, "r");
	if (!f) {
		printf("{ERROR} - could not read file '%s'\n", filename);
		exit(-1);
	}
	char c = fgetc(f);
	while (c != EOF) {
		if (ptr > max_file_size) {
			printf("{ERROR} - file too large");
			exit(-1);
		}
		//printf("%c", c);
		*(contents + ptr) = c;
		ptr++;
		c = fgetc(f);
	}
	*(contents + ptr) = '\0';
	fclose(f);
	return contents;
}

int main() {
	/* setup */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION[0]);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION[1]);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL triangle in C!", NULL, NULL);
	if (!window) {
		printf("{ERROR} - window could not be created\n");
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL()) {
		printf("{ERROR} - could not load OpenGL\n");
		exit(-1);
	}

	/* main program */
	printf("{DEBUG} - OpenGL Version: %s\n", glGetString(GL_VERSION));

	uint32_t vao, vbo, prog, vs, fs;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof triangle_vertices, triangle_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(3 * sizeof(float)));


	prog = glCreateProgram();
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vsource = read_file("res/shaders/default_vert.glsl", 512);
	const char* fsource = read_file("res/shaders/default_frag.glsl", 512);
	glShaderSource(vs, 1, &vsource, NULL);
	glShaderSource(fs, 1, &fsource, NULL);
	
	glCompileShader(vs);
	glCompileShader(fs);

	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

	glLinkProgram(prog);
	glUseProgram(prog);

	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(
			BACKGROUND_COLOUR[0],
			BACKGROUND_COLOUR[1],
			BACKGROUND_COLOUR[2],
			BACKGROUND_COLOUR[3]
		);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* cleanup */
	glDetachShader(prog, vs);
	glDetachShader(prog, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	glDeleteProgram(prog);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	free(fsource);
	free(vsource);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
