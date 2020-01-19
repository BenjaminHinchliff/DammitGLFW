#include "pch.h"
#include "ShaderLoader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
	std::string vertexSource;
	std::string fragmentSource;
	try
	{
		// open files
		std::ifstream vShadFile{ vertexPath }, fShadFile{ fragmentPath };
		std::stringstream vShadStream, fShadStream;
		// read file contents into streams
		vShadStream << vShadFile.rdbuf();
		fShadStream << fShadFile.rdbuf();
		vShadFile.close();
		fShadFile.close();
		// convert stream into stream
		vertexSource = vShadStream.str();
		fragmentSource = fShadStream.str();
	}
	catch (const std::ifstream::failure &e)
	{
		std::cerr << e.what() << '\n';
	}

	// compile stuff
	int success;
	char infoLog[512];
	const char *cStrVertexSource{ vertexSource.c_str() };
	const char *cStrFragmentSource{ fragmentSource.c_str() };

	// create vertex shader
	GLuint vertex{ glCreateShader(GL_VERTEX_SHADER) };
	glShaderSource(vertex, 1, &cStrVertexSource, NULL);
	glCompileShader(vertex);
	// check for errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	// create fragment shader
	GLuint fragment{ glCreateShader(GL_FRAGMENT_SHADER) };
	glShaderSource(fragment, 1, &cStrFragmentSource, NULL);
	glCompileShader(fragment);
	// check for errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
	}

	// create shader program
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);
	// check for errors
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	// clean up shaders
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(m_ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}
void Shader::setMat4fv(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const float x, const float y, const float z) const
{
	glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
}

void Shader::setActiveTexture(int texNum)
{
	glActiveTexture(GL_TEXTURE0 + texNum);
}

const GLuint& Shader::getID() const
{
	return m_ID;
}