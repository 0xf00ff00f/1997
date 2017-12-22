#include "gl_shader_program.h"

#include "gl_check.h"

#include <array>
#include <fstream>
#include <sstream>

namespace gl {

shader::shader(GLenum type)
    : id_{GL_CHECK_R(glCreateShader(type))}
{ }

void shader::compile_source_code(const char *source)
{
    GL_CHECK(glShaderSource(id_, 1, &source, nullptr));
    GL_CHECK(glCompileShader(id_));

    int rv;
    GL_CHECK(glGetShaderiv(id_, GL_COMPILE_STATUS, &rv));
    if (!rv) {
        std::array<GLchar, 64*1024> buf;
        GLsizei length;
        glGetShaderInfoLog(id_, buf.size() - 1, &length, buf.data());
        panic("failed to compile shader:\n%.*s", length, buf.data());
    }
}

void shader::compile_source_file(const std::string& filename)
{
    std::ifstream file{filename};

    std::stringstream buffer;
    buffer << file.rdbuf();

    compile_source_code(buffer.str().c_str());
}

shader_program::shader_program()
    : id_{GL_CHECK_R(glCreateProgram())}
{ }

void
shader_program::add_shader(const shader& s)
{
    GL_CHECK(glAttachShader(id_, s.id()));
}

void shader_program::link()
{
    GL_CHECK(glLinkProgram(id_));

    int rv;
    GL_CHECK(glGetProgramiv(id_, GL_LINK_STATUS, &rv));
    if (!rv)
        panic("failed to link shader_program");
}

void shader_program::bind()
{
    GL_CHECK(glUseProgram(id_));
}

int shader_program::uniform_location(const char *name) const
{
    return GL_CHECK_R(glGetUniformLocation(id_, name));
}

void shader_program::set_uniform_value_f(int location, GLfloat v0)
{
    GL_CHECK(glUniform1f(location, v0));
}

void shader_program::set_uniform_value_f(int location, GLfloat v0, GLfloat v1)
{
    GL_CHECK(glUniform2f(location, v0, v1));
}

void shader_program::set_uniform_value_f(int location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    GL_CHECK(glUniform3f(location, v0, v1, v2));
}

void shader_program::set_uniform_value_f(int location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    GL_CHECK(glUniform4f(location, v0, v1, v2, v3));
}

void shader_program::set_uniform_value_i(int location, GLint v0)
{
    GL_CHECK(glUniform1i(location, v0));
}

void shader_program::set_uniform_value_i(int location, GLint v0, GLint v1)
{
    GL_CHECK(glUniform2i(location, v0, v1));
}

void shader_program::set_uniform_value_i(int location, GLint v0, GLint v1, GLint v2)
{
    GL_CHECK(glUniform3i(location, v0, v1, v2));
}

void shader_program::set_uniform_value_i(int location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    GL_CHECK(glUniform4i(location, v0, v1, v2, v3));
}

void shader_program::set_uniform_matrix4(int location, const std::array<GLfloat, 16>& matrix4)
{
    GL_CHECK(glUniformMatrix4fv(location, 1, 1, matrix4.data()));
}

void shader_program::set_uniform_value_f(const GLchar *name, GLfloat v0)
{
    GL_CHECK(glUniform1f(uniform_location(name), v0));
}

void shader_program::set_uniform_value_f(const GLchar *name, GLfloat v0, GLfloat v1)
{
    GL_CHECK(glUniform2f(uniform_location(name), v0, v1));
}

void shader_program::set_uniform_value_f(const GLchar *name, GLfloat v0, GLfloat v1, GLfloat v2)
{
    GL_CHECK(glUniform3f(uniform_location(name), v0, v1, v2));
}

void shader_program::set_uniform_value_f(const GLchar *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    GL_CHECK(glUniform4f(uniform_location(name), v0, v1, v2, v3));
}

void shader_program::set_uniform_value_i(const GLchar *name, GLint v0)
{
    GL_CHECK(glUniform1i(uniform_location(name), v0));
}

void shader_program::set_uniform_value_i(const GLchar *name, GLint v0, GLint v1)
{
    GL_CHECK(glUniform2i(uniform_location(name), v0, v1));
}

void shader_program::set_uniform_value_i(const GLchar *name, GLint v0, GLint v1, GLint v2)
{
    GL_CHECK(glUniform3i(uniform_location(name), v0, v1, v2));
}

void shader_program::set_uniform_value_i(const GLchar *name, GLint v0, GLint v1, GLint v2, GLint v3)
{
    GL_CHECK(glUniform4i(uniform_location(name), v0, v1, v2, v3));
}

void shader_program::set_uniform_matrix4(const GLchar *name, const std::array<GLfloat, 16>& matrix4)
{
    GL_CHECK(glUniformMatrix4fv(uniform_location(name), 1, 1, matrix4.data()));
}

} // gl
