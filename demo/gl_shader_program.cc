#include "gl_shader_program.h"

#include "panic.h"

#include <array>
#include <fstream>
#include <sstream>

namespace gl {

shader::shader(GLenum type)
    : id_{glCreateShader(type)}
{ }

void shader::compile_source_code(const char *source)
{
    glShaderSource(id_, 1, &source, nullptr);
    glCompileShader(id_);

    int rv;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &rv);
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
    : id_{glCreateProgram()}
{ }

void
shader_program::add_shader(const shader& s)
{
    glAttachShader(id_, s.id());
}

void shader_program::link()
{
    glLinkProgram(id_);

    int rv;
    glGetProgramiv(id_, GL_LINK_STATUS, &rv);
    if (!rv)
        panic("failed to link shader_program");
}

void shader_program::bind()
{
    glUseProgram(id_);
}

int shader_program::uniform_location(const char *name) const
{
    return glGetUniformLocation(id_, name);
}

void shader_program::set_uniform_f(int location, GLfloat v0)
{
    glUniform1f(location, v0);
}

void shader_program::set_uniform_f(int location, GLfloat v0, GLfloat v1)
{
    glUniform2f(location, v0, v1);
}

void shader_program::set_uniform_f(int location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(location, v0, v1, v2);
}

void shader_program::set_uniform_f(int location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glUniform4f(location, v0, v1, v2, v3);
}

void shader_program::set_uniform_i(int location, GLint v0)
{
    glUniform1i(location, v0);
}

void shader_program::set_uniform_i(int location, GLint v0, GLint v1)
{
    glUniform2i(location, v0, v1);
}

void shader_program::set_uniform_i(int location, GLint v0, GLint v1, GLint v2)
{
    glUniform3i(location, v0, v1, v2);
}

void shader_program::set_uniform_i(int location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    glUniform4i(location, v0, v1, v2, v3);
}

void shader_program::set_uniform_matrix4(int location, const std::array<GLfloat, 16>& matrix4)
{
    glUniformMatrix4fv(location, 1, 1, matrix4.data());
}

void shader_program::set_uniform_f(const GLchar *name, GLfloat v0)
{
    glUniform1f(uniform_location(name), v0);
}

void shader_program::set_uniform_f(const GLchar *name, GLfloat v0, GLfloat v1)
{
    glUniform2f(uniform_location(name), v0, v1);
}

void shader_program::set_uniform_f(const GLchar *name, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(uniform_location(name), v0, v1, v2);
}

void shader_program::set_uniform_f(const GLchar *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glUniform4f(uniform_location(name), v0, v1, v2, v3);
}

void shader_program::set_uniform_i(const GLchar *name, GLint v0)
{
    glUniform1i(uniform_location(name), v0);
}

void shader_program::set_uniform_i(const GLchar *name, GLint v0, GLint v1)
{
    glUniform2i(uniform_location(name), v0, v1);
}

void shader_program::set_uniform_i(const GLchar *name, GLint v0, GLint v1, GLint v2)
{
    glUniform3i(uniform_location(name), v0, v1, v2);
}

void shader_program::set_uniform_i(const GLchar *name, GLint v0, GLint v1, GLint v2, GLint v3)
{
    glUniform4i(uniform_location(name), v0, v1, v2, v3);
}

void shader_program::set_uniform_matrix4(const GLchar *name, const std::array<GLfloat, 16>& matrix4)
{
    glUniformMatrix4fv(uniform_location(name), 1, 1, matrix4.data());
}

} // gl
