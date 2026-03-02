#pragma once

#include "OGLTexture.h"
#include "OGLRenderbuffer.h"

namespace ogl
{
	enum class FramebufferAttachment : uint8_t
	{
		Color,
		Depth,
		Stencil,
		DepthStencil
	};

	class Framebuffer final
	{
	public:
		template<typename T>
		static constexpr bool IsSupportedAttachmentType =
			std::same_as<T, Texture> ||
			std::same_as<T, Renderbuffer>;

		Framebuffer(std::string_view debugName = std::string_view{});
		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		Framebuffer(Framebuffer&&) = delete;
		Framebuffer& operator=(Framebuffer&&) = delete;
		~Framebuffer();

		void Bind() const;
		void Unbind() const;

		bool Validate();

		void Resize(uint16_t width, uint16_t height);

		template<class T> requires IsSupportedAttachmentType<T>
		void Attach(std::shared_ptr<T> toAttach, FramebufferAttachment attachment, uint32_t index = 0, std::optional<uint32_t> layer = std::nullopt);

		template<class T> requires IsSupportedAttachmentType<T>
		std::optional<std::reference_wrapper<T>> GetAttachment(FramebufferAttachment attachment, uint32_t index = 0) const;

		void SetTargetDrawBuffer(std::optional<uint32_t> index);
		void SetTargetReadBuffer(std::optional<uint32_t> index);

		std::pair<uint16_t, uint16_t> GetSize(FramebufferAttachment attachment = FramebufferAttachment::Color) const;

		void BlitToBackBuffer(uint16_t backBufferWidth, uint16_t backBufferHeight) const;

		void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, PixelDataFormat format, PixelDataType type, void* data) const;

		const std::string& GetDebugName() const;

		bool IsValid() const;
		GLuint GetID() const { return m_id; }
	private:
		GLuint m_id{ 0 };
		bool m_valid = false;
		std::string m_debugName;
		std::unordered_map<uint32_t, std::variant<
			std::shared_ptr<Texture>, 
			std::shared_ptr<Renderbuffer>>> m_attachments;
	};
} // namespace ogl