#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>

struct Cam {
    Cam( glm::vec3 camera_pos, glm::vec3 camera_tgt ) 
         : pos(camera_pos), 
           tgt(camera_tgt)
    {
        this->dir = glm::normalize(camera_pos - camera_tgt);
        
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->right = glm::normalize(glm::cross(up, this->dir));
        this->up = glm::cross(this->dir, this->right);
    }

    // camera position and look-direction //
    glm::vec3 pos;
    glm::vec3 tgt;
    glm::vec3 dir;
    
    // camera axes relative to camera->pos //
    glm::vec3 right;
    glm::vec3 up;
}; 

extern Cam *camera;
extern glm::mat4 view;

void init_camera();
void destroy_camera();

#endif 
