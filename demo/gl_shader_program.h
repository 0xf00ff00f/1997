#pragma once

#include "noncopyable.h"

#include <GL/glew.h>

#include <array>
#include <string>

namespace gl
{

class shader : private noncopyable
{
public:
    shader(GLenum type);

    void compile_source_code(const char *source);
    void compile_source_file(const std::string &filename);

    GLuint id() const { return id_; }

private:
    GLuint id_;
};

class shader_program : private noncopyable
{
public:
    shader_program();

    void add_shader(const shader& s);
    void link();

    void bind();

    int uniform_location(const char *name) const;

    void set_uniform_f(int location, GLfloat v0);
    void set_uniform_f(int location, GLfloat v0, GLfloat v1);
    void set_uniform_f(int location, GLfloat v0, GLfloat v1, GLfloat v2);
    void set_uniform_f(int location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

    void set_uniform_i(int location, GLint v0);
    void set_uniform_i(int location, GLint v0, GLint v1);
    void set_uniform_i(int location, GLint v0, GLint v1, GLint v2);
    void set_uniform_i(int location, GLint v0, GLint v1, GLint v2, GLint v3);

    void set_uniform_matrix4(int location, const std::array<GLfloat, 16>& matrix4);

    void set_uniform_f(const GLchar *name, GLfloat v0);
    void set_uniform_f(const GLchar *name, GLfloat v0, GLfloat v1);
    void set_uniform_f(const GLchar *name, GLfloat v0, GLfloat v1, GLfloat v2);
    void set_uniform_f(const GLchar *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

    void set_uniform_i(const GLchar *name, GLint v0);
    void set_uniform_i(const GLchar *name, GLint v0, GLint v1);
    void set_uniform_i(const GLchar *name, GLint v0, GLint v1, GLint v2);
    void set_uniform_i(const GLchar *name, GLint v0, GLint v1, GLint v2, GLint v3);

    void set_uniform_matrix4(const GLchar *name, const std::array<GLfloat, 16>& matrix4);

private:
    GLuint id_;
};

} // gl
