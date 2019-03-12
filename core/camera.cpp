// system headers //
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// custom headers // 
#include <camera.h>
#include <gl_utils.h>

// global variables //
Cam *camera; 
glm::mat4 view;

void 
init_camera( glm::vec3 camera_pos, glm::vec3 camera_tgt ) 
{
#if DEBUG
    printf("(init_camera) camera pos: (%f,%f,%f) camera tgt: (%f,%f,%f)\n", camera_pos.x, camera_pos.y, camera_pos.z, camera_tgt.x, camera_tgt.y, camera_tgt.z);
#endif
    gl_log("(init_camera) camera pos: (%d,%d,%d) camera tgt: (%d,%d,%d)\n", camera_pos.x, camera_pos.y, camera_pos.z, camera_tgt.x, camera_tgt.y, camera_tgt.z);

    camera = new Cam(camera_pos, camera_tgt);
    view = glm::lookAt(camera_pos,
                       camera_tgt,
                       glm::vec3(0.0f, 1.0f, 0.0f)); 
}

void move_camera()
{
    
}

void
destroy_camera() 
{
    delete camera;
}
