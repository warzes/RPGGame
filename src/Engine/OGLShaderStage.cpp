#include "stdafx.h"
#include "OGLShaderStage.h"
//=============================================================================
inline GLenum EnumToValue(ogl::ShaderType type) noexcept
{
	switch (type)
	{
	case ogl::ShaderType::Vertex:                 return GL_VERTEX_SHADER;
	case ogl::ShaderType::Fragment:               return GL_FRAGMENT_SHADER;
	case ogl::ShaderType::Geometry:               return GL_GEOMETRY_SHADER;
	case ogl::ShaderType::Compute:                return GL_COMPUTE_SHADER;
	case ogl::ShaderType::TessellationControl:    return GL_TESS_EVALUATION_SHADER;
	case ogl::ShaderType::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER;
	default: std::unreachable();
	}
}
//=============================================================================
ogl::ShaderStage::ShaderStage(ShaderType type)
{
	m_id = glCreateShader(EnumToValue(type));
	m_type = type;
}
//=============================================================================
ogl::ShaderStage::~ShaderStage()
{
	glDeleteShader(m_id);
}
//=============================================================================
void ogl::ShaderStage::Upload(const std::string& source) const
{
	const char* src = source.c_str();
	glShaderSource(m_id, 1, &src, nullptr);
}
//=============================================================================
ogl::ShaderCompilationResult ogl::ShaderStage::Compile() const
{
	glCompileShader(m_id);

	GLint compileStatus;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetShaderInfoLog(m_id, maxLength, &maxLength, errorLog.data());

		return {
			.success = false,
			.message = errorLog
		};
	}

	return {
		.success = true
	};
}
//=============================================================================
ogl::ShaderType ogl::ShaderStage::GetType() const
{
	return m_type;
}
//=============================================================================