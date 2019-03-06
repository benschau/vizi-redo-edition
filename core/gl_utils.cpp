/**
 * 
 * gl_utils.cpp
 * ------------
 * Borrowed a.k.a straight up stolen from Dr. Anton Gerdelan's OpenGL4 tutorial,
 * since I love his style.
 * 	- https://github.com/capnramses/antons_opengl_tutorials_book/
 *
 */

// system headers // 
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <vector>

// custom headers // 
#include <gl_utils.h>

#define GL_LOG_FILE "gl.log"
#define GL_WINDOW_NAME "vizi-redo"
#define MAX_SHADER_LENGTH 262144
#define MAX_SHADERINFO_LENGTH 2048

/* ------------------------------------------ */
// gl logging utilities //
/* ------------------------------------------ */
    
bool 
restart_gl_log() 
{
    FILE *file = fopen(GL_LOG_FILE, "w");
    if (!file) 
    {
        fprintf(stderr, "(restart_gl_log) ERR: Could not open GL_LOG_FILE at %s for writing.\n", GL_LOG_FILE);
        return false;
    }

    time_t now = time(NULL);
    char *date = ctime(&now);
    fprintf(file, "vizi-redo:gl_log() - %s", date);
    fclose(file);

    return true;
}

bool 
gl_log( const char *msg, ... )  
{
    va_list argptr;

    FILE *file = fopen(GL_LOG_FILE, "a");
    if (!file) 
    {
        fprintf(stderr, "(gl_log) ERR: Could not open GL_LOG_FILE at %s for appending.\n", GL_LOG_FILE);
        return false;
    }
    

    va_start(argptr, msg);
    vfprintf(file, msg, argptr);
    va_end(argptr);
    fclose(file);

    return true;
}

bool 
gl_log_err( const char *msg, ... ) 
{
    va_list argptr;

    FILE *file = fopen(GL_LOG_FILE, "a");
    if (!file) 
    {
        fprintf(stderr, "(gl_log_err) ERR: Could not open GL_LOG_FILE at %s for appending.\n", GL_LOG_FILE);
        return false;
    }
    

    va_start(argptr, msg);
    vfprintf(file, msg, argptr);
    va_end(argptr);
    va_start(argptr, msg);
    vfprintf(stderr, msg, argptr);
    va_end(argptr);
    fclose(file);

    return true;
}

/* ------------------------------------------ */
// glfw3 and glew utilities //
/* ------------------------------------------ */

bool 
start_gl()  
{
    gl_log("(start_gl) launching GLFW %s", glfwGetVersionString());
    
    // setting up GLFW 
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) 
    {
        fprintf(stderr, "(start_gl) ERR: Could not start GLFW3\n");
        return false;
    }

    g_window = glfwCreateWindow(g_gl_width, g_gl_height, GL_WINDOW_NAME, NULL, NULL);

    if (!g_window) 
    {
        fprintf(stderr, "(start_gl) ERR: Could not open GLFW3 window.");
        return false;
    }
    
    // setup GLFW callbacks
    glfwSetFramebufferSizeCallback(g_window, glfw_framebuffer_size_callback);
    glfwSetKeyCallback(g_window, glfw_key_callback);
    glfwMakeContextCurrent(g_window);

    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // setting up GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);

    printf("(startgl) Renderer: %s\n", renderer);
    printf("(startgl) OpenGL version supported: %s\n", version);
    gl_log("(startgl) renderer: %s\nversion: %s\n", renderer, version);

    return true;
}

void 
glfw_error_callback( int error, const char *desc ) 
{
    fputs(desc, stderr);
    gl_log_err("%s\n", desc);
}

void 
glfw_framebuffer_size_callback( GLFWwindow *window, int width, int height ) 
{
    g_gl_width = width;
    g_gl_height = height;
    printf("width %i, height %i\n", width, height);

    // UPDATE PERSPECTIVE MATRICES HERE //
}

void 
glfw_key_callback( GLFWwindow *window, int key, int scancode, int action, int mods ) 
{
    if (action == GLFW_PRESS) {
        switch (key) 
        {
            case GLFW_KEY_ESCAPE:
                gl_log("(glfw_key_callback) TERM: suggesting nicely to GLFW to shut down...");
                glfwSetWindowShouldClose(g_window, 1);
                break;
            default:
                break;
        }

    }
}

void 
_update_fps_counter( GLFWwindow *window ) 
{
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    
    if (elapsed_seconds > 0.25) 
    {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "%s @ fps: %.2f", GL_WINDOW_NAME, fps);

        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }

    frame_count++;
}

/* ------------------------------------------ */
// shader utilities //
/* ------------------------------------------ */
std::string
parse_glsl( const std::string filename ) 
{
    std::ifstream in(filename, std::ifstream::in);

    if (!in)
    {
        fprintf(stderr, "(parse_glsl) ERR: Could not parse GLSL shader %s\n", filename.c_str());
        return NULL;
    }
    
    int curr_length = 0;
    std::string line, glsl;
    while (getline(in, line)) 
    {
        curr_length += line.size();
        if (curr_length > MAX_SHADER_LENGTH) 
        {
            gl_log_err("(parse_glsl) ERR: Shader length is longer than the max shader length supported, %d\n", MAX_SHADER_LENGTH);
        }
        
        glsl.append(line); 
    }

    in.close();

    return glsl;
}

void 
shader_info( GLuint shader_index ) 
{
    std::vector<char> buff(MAX_SHADERINFO_LENGTH);
    glGetShaderInfoLog(shader_index, MAX_SHADERINFO_LENGTH, NULL, buff.data());
    std::string log(begin(buff), end(buff));

#if DEBUG
    printf("(shader_info) shader info log for GL index: %i:\n\t%s\n", shader_index, log.c_str());
#endif
    gl_log("(shader_info) shader info log for GL index: %i:\n\t%s\n", shader_index, log.c_str());
}

bool 
init_shader( const char *filename, GLuint *shader_index, GLenum type ) 
{
#if DEBUG
    printf("(init_shader) creating shader from %s...\n", filename);
#endif
    gl_log("(init_shader) creating shader from %s...\n", filename);

    std::string glsl = parse_glsl(filename);
    *shader_index = glCreateShader(type);

    const GLchar *p = (const GLchar *) glsl.c_str();
    glShaderSource(*shader_index, 1, &p, NULL);
    glCompileShader(*shader_index);
    
    int params = -1;
    glGetShaderiv(*shader_index, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) 
    {
        gl_log_err("(init_shader) ERR: GL shader index %i did not compile.\n", *shader_index);
        shader_info(*shader_index);
        return false;
    }
   
#if DEBUG
    printf("(init_shader): creating shader from %s...\n", filename);
#endif
    gl_log("(init_shader): creating shader from %s...\n", filename);

    return true;
}
