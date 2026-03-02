#pragma once

namespace ogl
{
	enum class ShaderType : uint8_t
	{
		Vertex,
		Fragment,
		Geometry,
		Compute,
		TessellationControl,
		TessellationEvaluation
	};

	struct ShaderCompilationResult final
	{
		bool success{ false };
		std::string message{};
	};

	class ShaderStage final
	{
	public:
		ShaderStage(ShaderType type);
		ShaderStage(const ShaderStage&) = delete;
		ShaderStage& operator=(const ShaderStage&) = delete;
		ShaderStage(ShaderStage&&) = delete;
		ShaderStage& operator=(ShaderStage&&) = delete;
		~ShaderStage();

		void Upload(const std::string& source) const;
		ShaderCompilationResult Compile() const;

		ShaderType GetType() const;

		GLuint GetID() const { return m_id; }
	private:
		GLuint     m_id{ 0 };
		ShaderType m_type;
	};
} // namespace ogl