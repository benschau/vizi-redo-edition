/**
 *
 * model.h
 * ----------
 *  Defines all object loader utilities, loading meshes, VBO/VAO & textures.
 *  Drawing should be taken care of in main.cpp.
 *
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <GL/glew.h>

bool load_obj( const char *filename, GLuint *vao, uint *point_count );

#endif
