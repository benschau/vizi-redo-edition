/**
 *
 * gl_utils.h
 * ----------
 * Borrowed a.k.a straight up stolen from Dr. Anton Gerdelan's OpenGL4 tutorial,
 * since I love his style.
 * 	- https://github.com/capnramses/antons_opengl_tutorials_book/
 *
 */


#ifndef _GL_UTILS_H_
#define _GL_UTILS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdarg.h>
#include <string>

// global variables //
extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;

// gl logging utilities //
bool restart_gl_log();
bool gl_log( const char *msg, ... );
bool gl_log_err( const char *msg, ... ); /* same as gl_log, but to stderr */

// glfw3 and glew utilities //
bool start_gl();
void glfw_key_callback( GLFWwindow *window, int key, int scancode, int action, int mods );
void glfw_error_callback( int error, const char *desc );
void glfw_framebuffer_size_callback( GLFWwindow *window, int width, int height );
void _update_fps_counter( GLFWwindow *window );

// GLSL shader utilities //
std::string parse_glsl( const std::string filename );
void shader_info( GLuint shader_index );
bool init_shader( const char *filename, GLuint *shader_index, GLenum type );

// GL program utilities //
GLuint parse_glprogram( const std::string filename );
bool glprogram_info( GLuint prog );
bool init_glprogram( GLuint vert, GLuint frag, GLuint *program );

#endif
