#include "shaders.h"


static bool compile_shader(char *shader_file_path, 
                           GLenum shader_type, 
                           GLuint *shader) {
  

  FILE *fptr;

  fptr = fopen(shader_file_path, "r");

  if (fptr == NULL) {
    fprintf(stderr, "Failed to load shader: %s\n", shader_file_path);
    exit(1);
  }

  // Moving file pointer to EOF
  fseek(fptr, 0L, SEEK_END);
  // Checking current position to find length.
  unsigned long fsize = ftell(fptr);
  // Putting fptr back the beginning to read properly later.
  rewind(fptr);
  
  char *buffer = (char *)malloc(fsize + 1);

  if (!buffer) {
    fprintf(stderr, "Couldn't allocate mem to compile shader: %s\n", shader_file_path);
    fclose(fptr);
    exit(1);
  }

  if (1 != fread(buffer, fsize, 1, fptr)) {
    fprintf(stderr, "Failed to read entire shader file: %s\n", shader_file_path);
    fclose(fptr);
    free(buffer);
    exit(1);
  }

  fclose(fptr);

  *shader = glCreateShader(shader_type);
  glShaderSource(*shader, 1, buffer, NULL);
  glCompileShader(*shader);

  int success;
  char infoLog[512];
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(*shader, 512, NULL, infoLog);
    fprintf(stderr, "Failed to compile shader %s: %s\n", shader_file_path, infoLog);
    exit(1);
  }

  return true; // use return false instead of exit(1);

}

bool reload_shader_program(ShaderProgram *shader_program) {
  
  // Compiling each shader
  for (uint i = 0; i < shader_program->num_shaders; i++) 
    compile_shader(shader_program->shader_file_paths[i], 
                   shader_program->shader_types[i],
                   shader_program->shaders + i);
   
  // Linking to make shader program
  shader_program->shader_program = glCreateProgram();

  for (uint i = 0; i < shader_program->num_shaders; i++)
    glAttachShader(shader_program->shader_program, shader_program->shaders[i]);

  glLinkProgram(shader_program->shader_program);

  int success;
  char infoLog[512];

  glGetProgramiv(shader_program->shader_program, 
                 GL_LINK_STATUS,
                 &success);

  if (!success) {
    glGetProgramInfoLog(shader_program->shader_program,
                        512,
                        NULL,
                        infoLog);
    fprintf(stderr, "Failed to link shader program: %s\n", infoLog);
  }

  return true; // Actually use this.
}

ShaderProgram *make_shader_program(
  uint num_shaders,
  char **shader_file_paths,
  GLenum *shader_types
) {
  
  ShaderProgram *shader_program = (ShaderProgram*)malloc(sizeof(ShaderProgram));

  // Copying data into shader program struct.
  shader_program->num_shaders = num_shaders;

  // Making some space for array copies.
  shader_program->shader_file_paths = (char **)malloc(sizeof(char *) * num_shaders);
  shader_program->shader_types = (GLenum *)malloc(sizeof(GLenum) * num_shaders);

  for (uint i = 0; i < num_shaders; i++) {
    shader_program->shader_file_paths[i] = 
      (char *)malloc(sizeof(char) * MAX_SHADER_PATH_LENGTH);
    strcpy(shader_program->shader_file_paths[i], shader_file_paths[i]);
    shader_program->shader_types[i] = shader_types[i];
  }
  
  reload_shader_program(shader_program);

  return shader_program;
}

void delete_shader_program(ShaderProgram *shader_program) {
  uint n = shader_program->num_shaders;

  for (uint i = 0; i < n; i++) 
    free(shader_program->shader_file_paths[i]);
  free(shader_program->shader_file_paths);
  free(shader_program->shader_types);
}

