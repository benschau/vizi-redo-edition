// system headers //
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// custom headers // 
#include <camera.h>

// global variables //
Cam *camera; 
glm::mat4 view;

void 
init_camera( glm::vec3 camera_pos, glm::vec3 camera_tgt ) 
{
    camera = new Cam(camera_pos, camera_tgt);
    view = glm::lookAt(camera_pos,
                       camera_tgt,
                       glm::vec3(0.0f, 1.0f, 0.0f)); 
}

void
destroy_camera() 
{
    delete camera;
}
