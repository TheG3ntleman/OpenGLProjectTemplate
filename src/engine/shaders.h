#ifndef SHADERS_H
#define SHADERS_H

#include "engine_config.h"
#include "glad/include/glad/glad.h"

// We are doing it the cheap way for now.
// Meaning we will have fragmented 
// shader structres for now.

typedef struct {

  uint num_shaders;
  char ** shader_file_paths;
  GLenum *shader_types;
  GLuint *shaders;
  GLuint shader_program;

} ShaderProgram;

static bool compile_shader(char *shader_file_path, 
                           GLenum shader_type, 
                           GLuint *shader);

ShaderProgram *make_shader_program(
  uint num_shaders,
  char **shader_file_paths,
  GLenum *shader_types
);

bool reload_shader_program(ShaderProgram *shader_program); 
void delete_shader_program(ShaderProgram *shader_program);

#endif
