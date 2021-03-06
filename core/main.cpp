/**
 *
 * main.cpp
 * --------
 * entry point for the main program. 
 *  - adapted from anton's opengl tutorials, see https://github.com/capnramses/antons_opengl_tutorials_book/
 *
 * author: benschau
 *
 */

// system headers // 
#include <stdio.h>
#include <glm/glm.hpp>

// custom headers //
#include <gl_utils.h>
#include <model.h>
#include <camera.h>

// GL window variables //
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow *g_window = NULL;

int 
main( int argc, char **argv ) 
{
    // TODO: Support more/better argument parsing. //
    if (argc != 2) {
        printf("ERR: specify object file location\n");
        return -1;
    } 

    restart_gl_log();
    start_gl();
        
    // initialize meshes, shaders, camera //
    GLuint vao;
    uint point_count;
    
    if (!load_obj(argv[1], &vao, &point_count)) 
    {
        printf("ERR: failed to load object file from %s. Giving up...\n", argv[1]);
        return -1;
    }
    
    // TODO: create shaders //
     

    // TODO: init camera //
    glm::vec3 camera_pos = glm::vec3(0.0, 0.0, 3.0); 
    glm::vec3 camera_tgt = glm::vec3(0.0, 0.0, 1.0);
    init_camera(camera_pos, camera_tgt); 

    // enable GL functionality for model faces & depth testing //
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE); /* cull faces for better performance */
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    // clear GL screen with grey //
    glClearColor(0.2, 0.2, 0.2, 1.0);

    // set GL viewport to fit the entire window //
    glViewport(0, 0, g_gl_width, g_gl_height);

    while (!glfwWindowShouldClose(g_window)) 
    {
        _update_fps_counter(g_window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, g_gl_width, g_gl_height);
        
        // draw object from vao buffer
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, point_count);
        
        // poll input devices
        glfwPollEvents();
        
        glfwSwapBuffers(g_window);
    }

    glfwTerminate();
	return 0;
}
