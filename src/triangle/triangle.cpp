#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/**
 * 顶点着色器，每个顶点有一个3D坐标，一个向量有四个分量，一个分量代表空间得坐标
 * 可以通过vec.x vec.y vec.z和vec.w获取
 */
const char *vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

/**
 * 片段着色器
 */
const char *fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\n\0";

void processInput(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
	std::cout << "Failed to create GLFW window" << std::endl;
	glfwTerminate();
	return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
	std::cout << "Failed to initialize GLAD" << std::endl;
	return -1;
  }

  //加载shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  //编译shader
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  //查看编译结果
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	std::cout << "error::shader::vertex::compilation_failed\n" << infoLog << std::endl;
  }
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  //查看编译结果
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	std::cout << "error::shader::fragment::compilation_failed\n" << infoLog << std::endl;
  }
  //链接shader
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
  if (!success) {
	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	std::cout << "error:: shader::program::linking_failed\n" << infoLog << std::endl;
  }
  //已经将着色器对象链接到程序对象后，要删除着色器对象
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  //三角形顶点，最后一个是z坐标，由于是2d图像，所以深度位0
  float vertices[] = {
		  -0.5f, -0.5f, 0.0f,//左
		  0.5f, -0.5f, 0.0f,//右
		  0.0f, 0.5f, 0.0f//上
  };
  //VAO 顶点数组对象，VBO顶点缓冲对象（在显存中存储大量顶点）
  unsigned int VAO, VBO;
  glGenVertexArrays(1, &VAO);
  //生成一个顶点缓冲对象
  glGenBuffers(1, &VBO);
  //绑定顶点数组
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //将顶点数据缓冲，并且指定这个数据是不常改变的
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
	// input
	// -----
	processInput(window);

	// render
	// ------
	//清除颜色字节缓冲
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//绘制三角形
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}