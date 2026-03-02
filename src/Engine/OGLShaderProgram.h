#pragma once

#include "OGLShaderStage.h"

template<typename T>
concept SupportedUniformType =
std::same_as<T, float> ||
std::same_as<T, int> ||
std::same_as<T, unsigned int> ||
std::same_as<T, glm::vec2> ||
std::same_as<T, glm::vec3> ||
std::same_as<T, glm::vec4> ||
std::same_as<T, glm::mat3> ||
std::same_as<T, glm::mat4>;

namespace ogl
{
	struct ShaderLinkingResult final
	{
		const bool success;
		const std::string message;
	};

	enum class UniformType : uint8_t
	{
		Bool,
		Int,
		UnsignedInt,
		Float,
		FloatVec2,
		FloatVec3,
		FloatVec4,
		FloatMat3,
		FloatMat4,
		DoubleMat4,
		Sampler2D,
		SamplerCube,
		Image2D,
		ImageCube
	};

	struct UniformInfo final
	{
		UniformType type;
		std::string name;
		std::any defaultValue;
	};

	class ShaderProgram final
	{
	public:
		ShaderProgram();
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram&&) = delete;
		ShaderProgram& operator=(ShaderProgram&&) = delete;
		~ShaderProgram();

		void Attach(const ShaderStage& shader);
		void Detach(const ShaderStage& shader);
		void DetachAll();

		ShaderLinkingResult Link();

		void Bind() const;
		void Unbind() const;

		template<SupportedUniformType T>
		void SetUniform(const std::string& name, const T& value);
		
		template<SupportedUniformType T>
		T GetUniform(const std::string& name);

		std::optional<std::reference_wrapper<const UniformInfo>> GetUniformInfo(const std::string& name) const;

		void QueryUniforms();

		const std::unordered_map<std::string, UniformInfo>& GetUniforms() const;

		GLuint GetID() const { return m_id; }
	private:
		GLuint m_id{ 0 };
		std::unordered_map<std::string, UniformInfo>           m_uniforms;
		std::unordered_map<std::string, uint32_t>              m_uniformsLocationCache;
		std::vector<std::reference_wrapper<const ShaderStage>> m_attachedShaders;
	};
} // namespace ogl

namespace ogl
{
#define DECLARE_GET_UNIFORM_FUNCTION(type, glType, func) \
	template<> \
	inline type ShaderProgram::GetUniform<type>(const std::string& name) \
	{ \
		type result{}; \
		if (m_uniformsLocationCache.contains(name)) \
		{ \
			if (const uint32_t location = m_uniformsLocationCache.at(name)) \
			{ \
				func(m_id, location, reinterpret_cast<glType*>(&result)); \
			} \
		} \
		return result; \
	}

	DECLARE_GET_UNIFORM_FUNCTION(int, GLint, glGetUniformiv);
	DECLARE_GET_UNIFORM_FUNCTION(unsigned int, GLuint, glGetUniformuiv);
	DECLARE_GET_UNIFORM_FUNCTION(float, GLfloat, glGetUniformfv);
	DECLARE_GET_UNIFORM_FUNCTION(glm::vec2, GLfloat, glGetUniformfv);
	DECLARE_GET_UNIFORM_FUNCTION(glm::vec3, GLfloat, glGetUniformfv);
	DECLARE_GET_UNIFORM_FUNCTION(glm::vec4, GLfloat, glGetUniformfv);
	DECLARE_GET_UNIFORM_FUNCTION(glm::mat3, GLfloat, glGetUniformfv);
	DECLARE_GET_UNIFORM_FUNCTION(glm::mat4, GLfloat, glGetUniformfv);

	#define DECLARE_SET_UNIFORM_FUNCTION(type, func, ...) \
		template<> \
		inline void ShaderProgram::SetUniform<type>(const std::string& name, const type& value) \
		{ \
			if (m_uniformsLocationCache.contains(name)) \
			{ \
				func(m_uniformsLocationCache.at(name), __VA_ARGS__); \
			} \
		}
	
	DECLARE_SET_UNIFORM_FUNCTION(int, glUniform1i, value);
	DECLARE_SET_UNIFORM_FUNCTION(unsigned int, glUniform1ui, value);
	DECLARE_SET_UNIFORM_FUNCTION(float, glUniform1f, value);
	DECLARE_SET_UNIFORM_FUNCTION(glm::vec2, glUniform2f, value.x, value.y);
	DECLARE_SET_UNIFORM_FUNCTION(glm::vec3, glUniform3f, value.x, value.y, value.z);
	DECLARE_SET_UNIFORM_FUNCTION(glm::vec4, glUniform4f, value.x, value.y, value.z, value.w);
	DECLARE_SET_UNIFORM_FUNCTION(glm::mat3, glUniformMatrix3fv, 1, GL_FALSE, &value[0][0]);
	DECLARE_SET_UNIFORM_FUNCTION(glm::mat4, glUniformMatrix4fv, 1, GL_FALSE, &value[0][0]);
}
//=============================================================================