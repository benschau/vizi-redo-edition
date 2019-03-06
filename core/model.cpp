/**
 *
 * model.cpp
 * ----------
 *  Defines all object loader utilities, loading meshes, VBO/VAO & textures.
 *  Drawing should be taken care of in main.cpp.
 *
 */

#define DEBUG 1

// system headers //
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// custom headers //
#include <model.h>
#include <gl_utils.h>

bool 
load_obj( const char *filename, GLuint *vao, uint *point_count )  
{
    const aiScene *scene = aiImportFile(filename, aiProcess_Triangulate);
    if (!scene) 
    {
        gl_log_err("ERR: invalid mesh %s\n", filename);
        return false;
    }
    
#if DEBUG
        printf("Loading model...\n");
        printf("\t%i animations\n", scene->mNumAnimations);          
        printf("\t%i cameras\n",    scene->mNumCameras);          
        printf("\t%i lights\n",     scene->mNumLights);          
        printf("\t%i materials\n",  scene->mNumMaterials);          
        printf("\t%i meshes\n",     scene->mNumMeshes);          
        printf("\t%i textures\n",   scene->mNumTextures);          
#endif
    
    gl_log("Loading model...\n");
    gl_log("\t%i animations\n", scene->mNumAnimations);          
    gl_log("\t%i cameras\n",    scene->mNumCameras);          
    gl_log("\t%i lights\n",     scene->mNumLights);          
    gl_log("\t%i materials\n",  scene->mNumMaterials);          
    gl_log("\t%i meshes\n",     scene->mNumMeshes);          
    gl_log("\t%i textures\n",   scene->mNumTextures);          
   
    const aiMesh *mesh = scene->mMeshes[0];
    
    *point_count = mesh->mNumVertices;

    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    GLfloat *points = NULL;		 // array of vertex points 
	GLfloat *normals = NULL;	 // array of vertex normals 
	GLfloat *texcoords = NULL;   // array of texture coordinates
	if (mesh->HasPositions()) 
    {
		points = (GLfloat *) malloc(*point_count * 3 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) 
        {
			const aiVector3D *vp = &(mesh->mVertices[i]);
			points[i * 3]     = (GLfloat) vp->x;
			points[i * 3 + 1] = (GLfloat) vp->y;
			points[i * 3 + 2] = (GLfloat) vp->z;
		}
	}

	if (mesh->HasNormals()) 
    {
		normals = (GLfloat *) malloc((*point_count) * 3 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) 
        {
			const aiVector3D *vn = &(mesh->mNormals[i]);
			normals[i * 3]     = (GLfloat) vn->x;
			normals[i * 3 + 1] = (GLfloat) vn->y;
			normals[i * 3 + 2] = (GLfloat) vn->z;
		}
	}

	if (mesh->HasTextureCoords(0)) 
    {
		texcoords = (GLfloat *) malloc((*point_count) * 2 * sizeof(GLfloat));
		for (int i = 0; i < *point_count; i++) 
        {
			const aiVector3D *vt = &(mesh->mTextureCoords[0][i]);
			texcoords[i * 2]     = (GLfloat) vt->x;
			texcoords[i * 2 + 1] = (GLfloat) vt->y;
		}
	}

	/* copy mesh data into VBOs */
	if (mesh->HasPositions()) 
    {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * (*point_count) * sizeof(GLfloat), points,
									GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		free(points);
	}

	if (mesh->HasNormals()) 
    {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * (*point_count) * sizeof(GLfloat), normals,
									GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);
		free(normals);
	}

	if (mesh->HasTextureCoords(0)) 
    {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 2 * (*point_count) * sizeof(GLfloat), texcoords,
									GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);
		free(texcoords);
	}

	if ( mesh->HasTangentsAndBitangents() ) {
		// NB: could store/print tangents here
	}

	aiReleaseImport(scene);

#if DEBUG
        printf("SUCCESS: finished loading mesh.\n");
#endif
    
    gl_log("SUCCESS: finished loading mesh.\n");

    return true;
}

