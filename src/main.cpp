#include "config.h"
#include "triangle_mesh.h"
#include "material.h"
//I am following along with GetIntoGameDev, never touched openGL before.
//Using training wheels, then am gonna make my own program!!
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
unsigned int make_module(const std::string& filepath, unsigned int module_type);
int main(void)
{
    GLFWwindow* window;
    if (!glfwInit())
    {
        printf("Program cannot start.");
        return -1;
    }
    window = glfwCreateWindow(640, 480, "Hello, Window!", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    int w,h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0,0,w,h);

    //lets put a triangle on this hoe
    TriangleMesh* triangle = new TriangleMesh();

    Material* material = new Material("../img/linus.jpg");
    Material* mask = new Material("../img/mask.jpg");
    unsigned int shader = make_shader(
        "../src/shaders/vertex.txt",
        "../src/shaders/fragment.txt"
    );

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "material"), 0);
    glUniform1i(glGetUniformLocation(shader, "mask"), 1);
                        // a      b      c   - is left and down, + is up & r
    glm::vec3 quad_position = {-0.2f, 0.4f, 0.0f};
    glm::vec3 camera_pos = {-5.0f, 0.0f, 3.0f};
    glm::vec3 camera_target = {0.0f, 0.0f, 0.0f};
    glm::vec3 up = {0.0f, 0.0f, 1.0f};
    unsigned int model_location = glGetUniformLocation(shader, "model");
    unsigned int view_location = glGetUniformLocation(shader, "view");
    unsigned int proj_location = glGetUniformLocation(shader, "projection");

    glm::mat4 view = glm::lookAt(camera_pos, camera_target, up);
    glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::perspective(45.0f, 640.0f/480.0f, 0.1f, 10.0f);
    glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, quad_position);
        model = glm::rotate(model, (float)glfwGetTime(), {0.0f, 0.0f, 1.0f});
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        material->use(0);
        mask->use(1);
        triangle->draw();
        glfwSwapBuffers(window);
    }
    glDeleteProgram(shader);
    delete triangle;
    delete material;
    delete mask;
    glfwTerminate();
    return 0;
}

unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath)
{
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule: modules)
    {
        glAttachShader(shader, shaderModule);
    }    
    glLinkProgram(shader);

    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "Shader linkage error:\n" << errorLog << std::endl;
        return 0;
    }
    for (unsigned int shaderModule: modules)
    {
        glDeleteShader(shaderModule);
    }
    return shader;
}

//this takes in a file, and makes a shader module.
unsigned int make_module(const std::string& filepath, unsigned int module_type)
{
   std::ifstream file;
   std::stringstream bufferedLines;
   std::string line;

   file.open(filepath);
   while(std::getline(file, line))
   {
    bufferedLines << line << "\n";
   }
   //need to go from C++ string to C string
   std::string shaderSource = bufferedLines.str();
   const char* shaderSrc = shaderSource.c_str();
   bufferedLines.str("");
   file.close();

   unsigned int shaderModule = glCreateShader(module_type);
   glShaderSource(shaderModule, 1, &shaderSrc, NULL);
   glCompileShader(shaderModule);

   int success;
   glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
   if (!success)
   {
    char errorLog[1024];
    glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
    std::cout << "Shader compilation error:\n" << errorLog << std::endl;
    return 0;
   }
   return shaderModule;
}