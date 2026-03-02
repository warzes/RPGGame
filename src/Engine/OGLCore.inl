//=============================================================================
// Implementation
//=============================================================================

inline GLenum ogl::EnumToValue(ogl::RenderingCapability c) noexcept
{
	switch (c)
	{
	case RenderingCapability::Blend:                 return GL_BLEND;
	case RenderingCapability::CullFace:              return GL_CULL_FACE;
	case RenderingCapability::DepthTest:             return GL_DEPTH_TEST;
	case RenderingCapability::Dither:                return GL_DITHER;
	case RenderingCapability::PolygonOffsetFill:     return GL_POLYGON_OFFSET_FILL;
	case RenderingCapability::SampleAlphaToCoverage: return GL_SAMPLE_ALPHA_TO_COVERAGE;
	case RenderingCapability::SampleCoverage:        return GL_SAMPLE_COVERAGE;
	case RenderingCapability::ScissorTest:           return GL_SCISSOR_TEST;
	case RenderingCapability::StencilTest:           return GL_STENCIL_TEST;
	case RenderingCapability::Multisample:           return GL_MULTISAMPLE;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::AccessSpecifier access) noexcept
{
	switch (access)
	{
	case AccessSpecifier::StreamDraw:  return GL_STREAM_DRAW;
	case AccessSpecifier::StreamRead:  return GL_STREAM_READ;
	case AccessSpecifier::StreamCopy:  return GL_STREAM_COPY;
	case AccessSpecifier::DynamicDraw: return GL_DYNAMIC_DRAW;
	case AccessSpecifier::DynamicRead: return GL_DYNAMIC_READ;
	case AccessSpecifier::DynamicCopy: return GL_DYNAMIC_COPY;
	case AccessSpecifier::StaticDraw:  return GL_STATIC_DRAW;
	case AccessSpecifier::StaticRead:  return GL_STATIC_READ;
	case AccessSpecifier::StaticCopy:  return GL_STATIC_COPY;

	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(PixelDataFormat format) noexcept
{
	switch (format)
	{
	case ogl::PixelDataFormat::STENCIL_INDEX: return GL_STENCIL_INDEX;
	case ogl::PixelDataFormat::DEPTH_COMPONENT: return GL_DEPTH_COMPONENT;
	case ogl::PixelDataFormat::RED: return GL_RED;
	case ogl::PixelDataFormat::GREEN: return GL_GREEN;
	case ogl::PixelDataFormat::BLUE: return GL_BLUE;
	case ogl::PixelDataFormat::ALPHA: return GL_ALPHA;
	case ogl::PixelDataFormat::RGB: return GL_RGB;
	case ogl::PixelDataFormat::BGR: return GL_BGR;
	case ogl::PixelDataFormat::RGBA: return GL_RGBA;
	case ogl::PixelDataFormat::BGRA: return GL_BGRA;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::BufferType type) noexcept
{
	switch (type)
	{
	case BufferType::Vertex:        return GL_ARRAY_BUFFER;
	case BufferType::Index:         return GL_ELEMENT_ARRAY_BUFFER;
	case BufferType::Uniform:       return GL_UNIFORM_BUFFER;
	case BufferType::ShaderStorage: return GL_SHADER_STORAGE_BUFFER;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::ImageAccessSpecifier access) noexcept
{
	switch (access)
	{
	case ogl::ImageAccessSpecifier::ReadOnly:  return GL_READ_ONLY;
	case ogl::ImageAccessSpecifier::WriteOnly: return GL_WRITE_ONLY;
	case ogl::ImageAccessSpecifier::ReadWrite: return GL_READ_WRITE;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::InternalFormat format) noexcept
{
	switch (format)
	{
	case ogl::InternalFormat::DEPTH_COMPONENT: return GL_DEPTH_COMPONENT;
	case ogl::InternalFormat::DEPTH_STENCIL: return GL_DEPTH_STENCIL;
	case ogl::InternalFormat::RED: return GL_RED;
	case ogl::InternalFormat::RG: return GL_RG;
	case ogl::InternalFormat::RGB: return GL_RGB;
	case ogl::InternalFormat::RGBA: return GL_RGBA;
	case ogl::InternalFormat::R8: return GL_R8;
	case ogl::InternalFormat::R8_SNORM: return GL_R8_SNORM;
	case ogl::InternalFormat::R16: return GL_R16;
	case ogl::InternalFormat::R16_SNORM: return GL_R16_SNORM;
	case ogl::InternalFormat::RG8: return GL_RG8;
	case ogl::InternalFormat::RG8_SNORM: return GL_RG8_SNORM;
	case ogl::InternalFormat::RG16: return GL_RG16;
	case ogl::InternalFormat::RG16_SNORM: return GL_RG16_SNORM;
	case ogl::InternalFormat::R3_G3_B2: return GL_R3_G3_B2;
	case ogl::InternalFormat::RGB4: return GL_RGB4;
	case ogl::InternalFormat::RGB5: return GL_RGB5;
	case ogl::InternalFormat::RGB8: return GL_RGB8;
	case ogl::InternalFormat::RGB8_SNORM: return GL_RGB8_SNORM;
	case ogl::InternalFormat::RGB10: return GL_RGB10;
	case ogl::InternalFormat::RGB12: return GL_RGB12;
	case ogl::InternalFormat::RGB16_SNORM: return GL_RGB16_SNORM;
	case ogl::InternalFormat::RGBA2: return GL_RGBA2;
	case ogl::InternalFormat::RGBA4: return GL_RGBA4;
	case ogl::InternalFormat::RGB5_A1: return GL_RGB5_A1;
	case ogl::InternalFormat::RGBA8: return GL_RGBA8;
	case ogl::InternalFormat::RGBA8_SNORM: return GL_RGBA8_SNORM;
	case ogl::InternalFormat::RGB10_A2: return GL_RGB10_A2;
	case ogl::InternalFormat::RGB10_A2UI: return GL_RGB10_A2UI;
	case ogl::InternalFormat::RGBA12: return GL_RGBA12;
	case ogl::InternalFormat::RGBA16: return GL_RGBA16;
	case ogl::InternalFormat::SRGB8: return GL_SRGB8;
	case ogl::InternalFormat::SRGB8_ALPHA8: return GL_SRGB8_ALPHA8;
	case ogl::InternalFormat::R16F: return GL_R16F;
	case ogl::InternalFormat::RG16F: return GL_RG16F;
	case ogl::InternalFormat::RGB16F: return GL_RGB16F;
	case ogl::InternalFormat::RGBA16F: return GL_RGBA16F;
	case ogl::InternalFormat::R32F: return GL_R32F;
	case ogl::InternalFormat::RG32F: return GL_RG32F;
	case ogl::InternalFormat::RGB32F: return GL_RGB32F;
	case ogl::InternalFormat::RGBA32F: return GL_RGBA32F;
	case ogl::InternalFormat::R11F_G11F_B10F: return GL_R11F_G11F_B10F;
	case ogl::InternalFormat::RGB9_E5: return GL_RGB9_E5;
	case ogl::InternalFormat::R8I: return GL_R8I;
	case ogl::InternalFormat::R8UI: return GL_R8UI;
	case ogl::InternalFormat::R16I: return GL_R16I;
	case ogl::InternalFormat::R16UI: return GL_R16UI;
	case ogl::InternalFormat::R32I: return GL_R32I;
	case ogl::InternalFormat::R32UI: return GL_R32UI;
	case ogl::InternalFormat::RG8I: return GL_RG8I;
	case ogl::InternalFormat::RG8UI: return GL_RG8UI;
	case ogl::InternalFormat::RG16I: return GL_RG16I;
	case ogl::InternalFormat::RG16UI: return GL_RG16UI;
	case ogl::InternalFormat::RG32I: return GL_RG32I;
	case ogl::InternalFormat::RG32UI: return GL_RG32UI;
	case ogl::InternalFormat::RGB8I: return GL_RGB8I;
	case ogl::InternalFormat::RGB8UI: return GL_RGB8UI;
	case ogl::InternalFormat::RGB16I: return GL_RGB16I;
	case ogl::InternalFormat::RGB16UI: return GL_RGB16UI;
	case ogl::InternalFormat::RGB32I: return GL_RGB32I;
	case ogl::InternalFormat::RGB32UI: return GL_RGB32UI;
	case ogl::InternalFormat::RGBA8I: return GL_RGBA8I;
	case ogl::InternalFormat::RGBA8UI: return GL_RGBA8UI;
	case ogl::InternalFormat::RGBA16I: return GL_RGBA16I;
	case ogl::InternalFormat::RGBA16UI: return GL_RGBA16UI;
	case ogl::InternalFormat::RGBA32I: return GL_RGBA32I;
	case ogl::InternalFormat::RGBA32UI: return GL_RGBA32UI;
	case ogl::InternalFormat::COMPRESSED_RED: return GL_COMPRESSED_RED;
	case ogl::InternalFormat::COMPRESSED_RG: return GL_COMPRESSED_RG;
	case ogl::InternalFormat::COMPRESSED_RGB: return GL_COMPRESSED_RGB;
	case ogl::InternalFormat::COMPRESSED_RGBA: return GL_COMPRESSED_RGBA;
	case ogl::InternalFormat::COMPRESSED_SRGB: return GL_COMPRESSED_SRGB;
	case ogl::InternalFormat::COMPRESSED_SRGB_ALPHA: return GL_COMPRESSED_SRGB_ALPHA;
	case ogl::InternalFormat::COMPRESSED_RED_RGTC1: return GL_COMPRESSED_RED_RGTC1;
	case ogl::InternalFormat::COMPRESSED_SIGNED_RED_RGTC1: return GL_COMPRESSED_SIGNED_RED_RGTC1;
	case ogl::InternalFormat::COMPRESSED_RG_RGTC2: return GL_COMPRESSED_RG_RGTC2;
	case ogl::InternalFormat::COMPRESSED_SIGNED_RG_RGTC2: return GL_COMPRESSED_SIGNED_RG_RGTC2;
	case ogl::InternalFormat::COMPRESSED_RGBA_BPTC_UNORM: return GL_COMPRESSED_RGBA_BPTC_UNORM;
	case ogl::InternalFormat::COMPRESSED_SRGB_ALPHA_BPTC_UNORM: return GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
	case ogl::InternalFormat::COMPRESSED_RGB_BPTC_SIGNED_FLOAT: return GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
	case ogl::InternalFormat::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT: return GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::Format format) noexcept
{
	switch (format)
	{
	case ogl::Format::RED: return GL_RED;
	case ogl::Format::RG: return GL_RG;
	case ogl::Format::RGB: return GL_RGB;
	case ogl::Format::BGR: return GL_BGR;
	case ogl::Format::RGBA: return GL_RGBA;
	case ogl::Format::BGRA: return GL_BGRA;
	case ogl::Format::RED_INTEGER: return GL_RED_INTEGER;
	case ogl::Format::RG_INTEGER: return GL_RG_INTEGER;
	case ogl::Format::RGB_INTEGER: return GL_RGB_INTEGER;
	case ogl::Format::BGR_INTEGER: return GL_BGR_INTEGER;
	case ogl::Format::RGBA_INTEGER: return GL_RGBA_INTEGER;
	case ogl::Format::BGRA_INTEGER: return GL_BGRA_INTEGER;
	case ogl::Format::STENCIL_INDEX: return GL_STENCIL_INDEX;
	case ogl::Format::DEPTH_COMPONENT: return GL_DEPTH_COMPONENT;
	case ogl::Format::DEPTH_STENCIL: return GL_DEPTH_STENCIL;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::TextureFilteringMode mode) noexcept
{
	switch (mode)
	{
	case ogl::TextureFilteringMode::Nearest: return GL_NEAREST;
	case ogl::TextureFilteringMode::Linear: return GL_LINEAR;
	case ogl::TextureFilteringMode::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
	case ogl::TextureFilteringMode::LinearMipmapLinear: return GL_LINEAR_MIPMAP_LINEAR;
	case ogl::TextureFilteringMode::LinearMipmapNearest: return GL_LINEAR_MIPMAP_NEAREST;
	case ogl::TextureFilteringMode::NearestMipmapLinear: return GL_NEAREST_MIPMAP_LINEAR;
	default: std::unreachable();
	}
}
inline GLenum ogl::EnumToValue(ogl::TextureWrapMode mode) noexcept
{
	switch (mode)
	{
	case ogl::TextureWrapMode::Repeat: return GL_REPEAT;
	case ogl::TextureWrapMode::ClampToEdge: return GL_CLAMP_TO_EDGE;
	case ogl::TextureWrapMode::ClampToBorder: return GL_CLAMP_TO_BORDER;
	case ogl::TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
	case ogl::TextureWrapMode::MirrorClampToEdge: return GL_MIRROR_CLAMP_TO_EDGE;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::PixelDataType type) noexcept
{
	switch (type)
	{
	case ogl::PixelDataType::Byte: return GL_BYTE;
	case ogl::PixelDataType::UnsignedByte: return GL_UNSIGNED_BYTE;
	case ogl::PixelDataType::Short: return GL_SHORT;
	case ogl::PixelDataType::UnsignedShort: return GL_UNSIGNED_SHORT;
	case ogl::PixelDataType::Int: return GL_INT;
	case ogl::PixelDataType::UnsignedInt: return GL_UNSIGNED_INT;
	case ogl::PixelDataType::Float: return GL_FLOAT;
	case ogl::PixelDataType::UnsignedByte_3_3_2: return GL_UNSIGNED_BYTE_3_3_2;
	case ogl::PixelDataType::UnsignedByte_2_3_3_Rev: return GL_UNSIGNED_BYTE_2_3_3_REV;
	case ogl::PixelDataType::UnsignedShort_5_6_5: return GL_UNSIGNED_SHORT_5_6_5;
	case ogl::PixelDataType::UnsignedShort_5_6_5_Rev: return GL_UNSIGNED_SHORT_5_6_5_REV;
	case ogl::PixelDataType::UnsignedShort_4_4_4_4: return GL_UNSIGNED_SHORT_4_4_4_4;
	case ogl::PixelDataType::UnsignedShort_4_4_4_4_Rev: return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	case ogl::PixelDataType::UnsignedShort_5_5_5_1: return GL_UNSIGNED_SHORT_5_5_5_1;
	case ogl::PixelDataType::UnsignedShort_1_5_5_5_Rev: return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case ogl::PixelDataType::UnsignedInt_8_8_8_8: return GL_UNSIGNED_INT_8_8_8_8;
	case ogl::PixelDataType::UnsignedInt_8_8_8_8_Rev: return GL_UNSIGNED_INT_8_8_8_8_REV;
	case ogl::PixelDataType::UnsignedInt_10_10_10_2: return GL_UNSIGNED_INT_10_10_10_2;
	case ogl::PixelDataType::UnsignedInt_2_10_10_10_Rev: return GL_UNSIGNED_INT_2_10_10_10_REV;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::TextureType type) noexcept
{
	switch (type)
	{
	case ogl::TextureType::Texture2D:   return GL_TEXTURE_2D;
	case ogl::TextureType::TextureCube: return GL_TEXTURE_CUBE_MAP;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::DataType type) noexcept
{
	switch (type) {
	case DataType::Byte:          return GL_BYTE;
	case DataType::UnsignedByte:  return GL_UNSIGNED_BYTE;
	case DataType::Short:         return GL_SHORT;
	case DataType::UnsignedShort: return GL_UNSIGNED_SHORT;
	case DataType::Int:           return GL_INT;
	case DataType::UnsignedInt:   return GL_UNSIGNED_INT;
	case DataType::Float:         return GL_FLOAT;
	case DataType::Double:        return GL_DOUBLE;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::RasterizationMode mode) noexcept
{
	switch (mode) {
	case RasterizationMode::Point: return GL_POINT;
	case RasterizationMode::Line:  return GL_LINE;
	case RasterizationMode::Fill:  return GL_FILL;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::ComparisonFunc func) noexcept
{
	switch (func) {
	case ComparisonFunc::Never:        return GL_NEVER;
	case ComparisonFunc::Less:         return GL_LESS;
	case ComparisonFunc::Equal:        return GL_EQUAL;
	case ComparisonFunc::LessEqual:    return GL_LEQUAL;
	case ComparisonFunc::Greater:      return GL_GREATER;
	case ComparisonFunc::NotEqual:     return GL_NOTEQUAL;
	case ComparisonFunc::GreaterEqual: return GL_GEQUAL;
	case ComparisonFunc::Always:       return GL_ALWAYS;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::Operation op) noexcept
{
	switch (op)
	{
	case Operation::Zero:          return GL_ZERO;
	case Operation::Keep:          return GL_KEEP;
	case Operation::Replace:       return GL_REPLACE;
	case Operation::Increment:     return GL_INCR;
	case Operation::IncrementWrap: return GL_INCR_WRAP;
	case Operation::Decrement:     return GL_DECR;
	case Operation::DecrementWrap: return GL_DECR_WRAP;
	case Operation::Invert:        return GL_INVERT;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::BlendFactor factor) noexcept
{
	switch (factor) {
	case BlendFactor::Zero:                  return GL_ZERO;
	case BlendFactor::One:                   return GL_ONE;
	case BlendFactor::SrcColor:              return GL_SRC_COLOR;
	case BlendFactor::OneMinusSrcColor:      return GL_ONE_MINUS_SRC_COLOR;
	case BlendFactor::DstColor:              return GL_DST_COLOR;
	case BlendFactor::OneMinusDstColor:      return GL_ONE_MINUS_DST_COLOR;
	case BlendFactor::SrcAlpha:              return GL_SRC_ALPHA;
	case BlendFactor::OneMinusSrcAlpha:      return GL_ONE_MINUS_SRC_ALPHA;
	case BlendFactor::DstAlpha:              return GL_DST_ALPHA;
	case BlendFactor::OneMinusDstAlpha:      return GL_ONE_MINUS_DST_ALPHA;
	case BlendFactor::ConstantColor:         return GL_CONSTANT_COLOR;
	case BlendFactor::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
	case BlendFactor::ConstantAlpha:         return GL_CONSTANT_ALPHA;
	case BlendFactor::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
	case BlendFactor::SrcAlphaSaturate:      return GL_SRC_ALPHA_SATURATE;
	case BlendFactor::Src1Color:             return GL_SRC1_COLOR;
	case BlendFactor::OneMinusSrc1Color:     return GL_ONE_MINUS_SRC1_COLOR;
	case BlendFactor::Src1Alpha:             return GL_SRC1_ALPHA;
	case BlendFactor::OneMinusSrc1Alpha:     return GL_ONE_MINUS_SRC1_ALPHA;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::BlendEquation eq) noexcept
{
	switch (eq) {
	case BlendEquation::FuncAdd:             return GL_FUNC_ADD;
	case BlendEquation::FuncSubtract:        return GL_FUNC_SUBTRACT;
	case BlendEquation::FuncReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
	case BlendEquation::Min:                 return GL_MIN;
	case BlendEquation::Max:                 return GL_MAX;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::CullFace cull) noexcept
{
	switch (cull) {
	case ogl::CullFace::Front:        return GL_FRONT;
	case ogl::CullFace::Back:         return GL_BACK;
	case ogl::CullFace::FrontAndBack: return GL_FRONT_AND_BACK;
	default: std::unreachable();
	}
}

inline GLenum ogl::EnumToValue(ogl::PrimitiveMode mode) noexcept
{
	switch (mode) {
	case PrimitiveMode::Points:                 return GL_POINTS;
	case PrimitiveMode::Lines:                  return GL_LINES;
	case PrimitiveMode::LineLoop:               return GL_LINE_LOOP;
	case PrimitiveMode::LineStrip:              return GL_LINE_STRIP;
	case PrimitiveMode::Triangles:              return GL_TRIANGLES;
	case PrimitiveMode::TriangleStrip:          return GL_TRIANGLE_STRIP;
	case PrimitiveMode::TriangleFan:            return GL_TRIANGLE_FAN;
	case PrimitiveMode::LinesAdjacency:         return GL_LINES_ADJACENCY;
	case PrimitiveMode::LineStripAdjacency:     return GL_LINE_STRIP_ADJACENCY;
	case PrimitiveMode::TrianglesAdjacency:     return GL_TRIANGLES_ADJACENCY;
	case PrimitiveMode::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
	case PrimitiveMode::Patches:                return GL_PATCHES;
	default: std::unreachable();
	}
}
#if GL_VERSION_4_0
inline GLbitfield ogl::EnumToValue(ogl::MemoryBarrierFlags mode) noexcept
{
	// TODO: проверить
	GLbitfield result = 0;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::VertexAttribArray))  result |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::ElementArray))       result |= GL_ELEMENT_ARRAY_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::Uniform))            result |= GL_UNIFORM_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::TextureFetch))       result |= GL_TEXTURE_FETCH_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::ShaderImageAccess))  result |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::Command))            result |= GL_COMMAND_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::PixelBuffer))        result |= GL_PIXEL_BUFFER_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::TextureUpdate))      result |= GL_TEXTURE_UPDATE_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::BufferUpdate))       result |= GL_BUFFER_UPDATE_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::ClientMappedBuffer)) result |= GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::Framebuffer))        result |= GL_FRAMEBUFFER_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::TransformFeedback))  result |= GL_TRANSFORM_FEEDBACK_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::AtomicCounter))      result |= GL_ATOMIC_COUNTER_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::ShaderStorage))      result |= GL_SHADER_STORAGE_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::QueryBuffer))        result |= GL_QUERY_BUFFER_BARRIER_BIT;
	if (((uint16_t)mode & (uint16_t)MemoryBarrierFlags::All))                result |= GL_ALL_BARRIER_BITS;
	return result;
}
#endif