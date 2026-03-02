#include "stdafx.h"
#include "OGLShaderProgram.h"
#include "OGLTexture.h"
//=============================================================================
inline ogl::UniformType ValueToEnum(GLenum type) noexcept
{
	switch (type)
	{
	case GL_BOOL:         return ogl::UniformType::Bool;
	case GL_INT:          return ogl::UniformType::Int;
	case GL_UNSIGNED_INT: return ogl::UniformType::UnsignedInt;
	case GL_FLOAT:        return ogl::UniformType::Float;
	case GL_FLOAT_VEC2:   return ogl::UniformType::FloatVec2;
	case GL_FLOAT_VEC3:   return ogl::UniformType::FloatVec3;
	case GL_FLOAT_VEC4:   return ogl::UniformType::FloatVec4;
	case GL_FLOAT_MAT3:   return ogl::UniformType::FloatMat3;
	case GL_FLOAT_MAT4:   return ogl::UniformType::FloatMat4;
	case GL_DOUBLE_MAT4:  return ogl::UniformType::DoubleMat4;
	case GL_SAMPLER_2D:   return ogl::UniformType::Sampler2D;
	case GL_SAMPLER_CUBE: return ogl::UniformType::SamplerCube;
	case GL_IMAGE_2D:     return ogl::UniformType::Image2D;
	case GL_IMAGE_CUBE:   return ogl::UniformType::ImageCube;
	default: std::unreachable();
	}
}
//=============================================================================
ogl::ShaderProgram::ShaderProgram()
{
	m_id = glCreateProgram();
}
//=============================================================================
ogl::ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}
//=============================================================================
void ogl::ShaderProgram::Attach(const ShaderStage & shader)
{
	glAttachShader(m_id, shader.GetID());
	m_attachedShaders.push_back(std::ref(shader));
}
//=============================================================================
void ogl::ShaderProgram::Detach(const ShaderStage & shader)
{
	glDetachShader(m_id, shader.GetID());
	m_attachedShaders.erase(std::remove_if(
		m_attachedShaders.begin(),
		m_attachedShaders.end(),
		[&shader](const std::reference_wrapper<const ShaderStage> s) {
			return s.get().GetID() == shader.GetID();
		}
	));
}
//=============================================================================
void ogl::ShaderProgram::DetachAll()
{
	for (auto& shader : m_attachedShaders)
	{
		glDetachShader(m_id, shader.get().GetID());
	}
	m_attachedShaders.clear();
}
//=============================================================================
ogl::ShaderLinkingResult ogl::ShaderProgram::Link()
{
	glLinkProgram(m_id);

	GLint linkStatus;
	glGetProgramiv(m_id, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GL_FALSE)
	{
		GLint maxLength;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

		std::string errorLog(maxLength, ' ');
		glGetProgramInfoLog(m_id, maxLength, &maxLength, errorLog.data());

		return {
			.success = false,
			.message = errorLog
		};
	}

	QueryUniforms();

	return {
		.success = true
	};
}
//=============================================================================
void ogl::ShaderProgram::Bind() const
{
	glUseProgram(m_id);
}
//=============================================================================
void ogl::ShaderProgram::Unbind() const
{
	glUseProgram(0);
}
//=============================================================================
std::optional<std::reference_wrapper<const ogl::UniformInfo>> ogl::ShaderProgram::GetUniformInfo(const std::string& name) const
{
	if (m_uniforms.contains(name))
	{
		return m_uniforms.at(name);
	}

	return std::nullopt;
}
//=============================================================================
void ogl::ShaderProgram::QueryUniforms()
{
	m_uniforms.clear();

	std::array<GLchar, 256> nameBuffer;

	GLint activeUniformCount = 0;
	glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &activeUniformCount);

	for (GLint i = 0; i < activeUniformCount; ++i)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;

		glGetActiveUniform(m_id, i, static_cast<GLsizei>(nameBuffer.size()), &actualLength, &arraySize, &type, nameBuffer.data());

		const auto name = std::string{ nameBuffer.data(), static_cast<size_t>(actualLength) };
		const auto uniformType = ValueToEnum(type);
		const auto location = glGetUniformLocation(m_id, name.c_str());

		if (location == -1)
		{
			continue; // Skip uniforms that don't have a valid location (e.g. uniform buffer members)
		}

		m_uniformsLocationCache.emplace(name, static_cast<uint32_t>(location));

		const std::any uniformValue = [&]() -> std::any {
			switch (uniformType)
			{
			case ogl::UniformType::Bool: return static_cast<bool>(GetUniform<int>(name));
			case ogl::UniformType::Int: return GetUniform<int>(name);
			case ogl::UniformType::UnsignedInt: return GetUniform<unsigned int>(name);
			case ogl::UniformType::Float: return GetUniform<float>(name);
			case ogl::UniformType::FloatVec2: return GetUniform<glm::vec2>(name);
			case ogl::UniformType::FloatVec3: return GetUniform<glm::vec3>(name);
			case ogl::UniformType::FloatVec4: return GetUniform<glm::vec4>(name);
			case ogl::UniformType::FloatMat3: return GetUniform<glm::mat3>(name);
			case ogl::UniformType::FloatMat4: return GetUniform<glm::mat4>(name);
			case ogl::UniformType::Sampler2D: return std::make_any<Texture*>(nullptr);
			case ogl::UniformType::SamplerCube: return std::make_any<Texture*>(nullptr);
			default: return std::nullopt;
			}
			}();

		// Only add the uniform if it has a value (unsupported uniform types will be ignored)
		if (uniformValue.has_value())
		{
			m_uniforms.emplace(name, UniformInfo{
				.type = uniformType,
				.name = name,
				.defaultValue = uniformValue
				});
		}
	}
}
//=============================================================================
const std::unordered_map<std::string, ogl::UniformInfo>& ogl::ShaderProgram::GetUniforms() const
{
	return m_uniforms;
}
//=============================================================================