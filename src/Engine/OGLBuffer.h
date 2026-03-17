#pragma once

#include "OGLCore.h"

namespace ogl
{
	class Buffer final
	{
	public:
		Buffer();
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;
		Buffer(Buffer&&) = delete;
		Buffer& operator=(Buffer&&) = delete;
		~Buffer();

		void Allocate(uint64_t size, AccessSpecifier usage = AccessSpecifier::StaticDraw);
		void Allocate(std::span<const std::byte> data, AccessSpecifier usage = AccessSpecifier::StaticDraw);

		void Upload(const void* data, std::optional<BufferMemoryRange> range = std::nullopt);

		void* MapWrite(uint64_t offset, uint64_t size) const;
		void Unmap() const;

		void Bind(BufferType type, std::optional<uint32_t> index = std::nullopt);
		void Unbind();

		bool IsValid() const { return m_id != 0; }
		bool IsEmpty() const { return GetSize() == 0; }

		uint64_t GetSize() const;

		GLuint GetID() const { return m_id; }

	private:
		GLuint                    m_id{ 0 };
		uint64_t                  m_allocatedBytes{ 0 };
		std::optional<BufferType> m_boundAs{ std::nullopt };
	};
} // namespace ogl