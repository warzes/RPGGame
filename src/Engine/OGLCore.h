#pragma once

namespace ogl
{
	enum class AccessSpecifier : uint8_t
	{
		StreamDraw,
		StreamRead,
		StreamCopy,
		DynamicDraw,
		DynamicRead,
		DynamicCopy,
		StaticDraw,
		StaticRead,
		StaticCopy
	};

	enum class BufferType : uint8_t
	{
		Vertex,
		Index,
		Uniform,
		ShaderStorage,
	};

	enum class DataType : uint8_t
	{
		Byte,
		UnsignedByte,
		Short,
		UnsignedShort,
		Int,
		UnsignedInt,
		Float,
		Double
	};

	enum class PixelDataFormat : uint8_t
	{
		STENCIL_INDEX,
		DEPTH_COMPONENT,
		RED,
		GREEN,
		BLUE,
		ALPHA,
		RGB,
		BGR,
		RGBA,
		BGRA,
	};

	enum class ImageAccessSpecifier : uint8_t
	{
		ReadOnly,
		WriteOnly,
		ReadWrite
	};

	enum class InternalFormat : uint8_t
	{
		// Base Internal Formats
		DEPTH_COMPONENT,
		DEPTH_STENCIL,
		RED,
		RG,
		RGB,
		RGBA,

		// Sized Internal Formats
		R8,
		R8_SNORM,
		R16,
		R16_SNORM,
		RG8,
		RG8_SNORM,
		RG16,
		RG16_SNORM,
		R3_G3_B2,
		RGB4,
		RGB5,
		RGB8,
		RGB8_SNORM,
		RGB10,
		RGB12,
		RGB16_SNORM,
		RGBA2,
		RGBA4,
		RGB5_A1,
		RGBA8,
		RGBA8_SNORM,
		RGB10_A2,
		RGB10_A2UI,
		RGBA12,
		RGBA16,
		SRGB8,
		SRGB8_ALPHA8,
		R16F,
		RG16F,
		RGB16F,
		RGBA16F,
		R32F,
		RG32F,
		RGB32F,
		RGBA32F,
		R11F_G11F_B10F,
		RGB9_E5,
		R8I,
		R8UI,
		R16I,
		R16UI,
		R32I,
		R32UI,
		RG8I,
		RG8UI,
		RG16I,
		RG16UI,
		RG32I,
		RG32UI,
		RGB8I,
		RGB8UI,
		RGB16I,
		RGB16UI,
		RGB32I,
		RGB32UI,
		RGBA8I,
		RGBA8UI,
		RGBA16I,
		RGBA16UI,
		RGBA32I,
		RGBA32UI,

		// Compressed Internal Formats
		COMPRESSED_RED,
		COMPRESSED_RG,
		COMPRESSED_RGB,
		COMPRESSED_RGBA,
		COMPRESSED_SRGB,
		COMPRESSED_SRGB_ALPHA,
		COMPRESSED_RED_RGTC1,
		COMPRESSED_SIGNED_RED_RGTC1,
		COMPRESSED_RG_RGTC2,
		COMPRESSED_SIGNED_RG_RGTC2,
		COMPRESSED_RGBA_BPTC_UNORM,
		COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
		COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
		COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
	};

	enum class TextureType : uint8_t
	{
		Texture2D,
		TextureCube
	};

	enum class Format : uint8_t
	{
		RED,
		RG,
		RGB,
		BGR,
		RGBA,
		BGRA,
		RED_INTEGER,
		RG_INTEGER,
		RGB_INTEGER,
		BGR_INTEGER,
		RGBA_INTEGER,
		BGRA_INTEGER,
		STENCIL_INDEX,
		DEPTH_COMPONENT,
		DEPTH_STENCIL
	};

	enum class PixelDataType : uint8_t
	{
		Byte,
		UnsignedByte,
		Short,
		UnsignedShort,
		Int,
		UnsignedInt,
		Float,
		UnsignedByte_3_3_2,
		UnsignedByte_2_3_3_Rev,
		UnsignedShort_5_6_5,
		UnsignedShort_5_6_5_Rev,
		UnsignedShort_4_4_4_4,
		UnsignedShort_4_4_4_4_Rev,
		UnsignedShort_5_5_5_1,
		UnsignedShort_1_5_5_5_Rev,
		UnsignedInt_8_8_8_8,
		UnsignedInt_8_8_8_8_Rev,
		UnsignedInt_10_10_10_2,
		UnsignedInt_2_10_10_10_Rev
	};

	enum class TextureFilteringMode : uint8_t
	{
		Nearest,
		Linear,
		NearestMipmapNearest,
		LinearMipmapLinear,
		LinearMipmapNearest,
		NearestMipmapLinear
	};

	enum class TextureWrapMode : uint8_t
	{
		Repeat,
		ClampToEdge,
		ClampToBorder,
		MirroredRepeat,
		MirrorClampToEdge
	};

	enum class RenderingCapability : uint8_t
	{
		Blend,
		CullFace,
		DepthTest,
		Dither,
		PolygonOffsetFill,
		SampleAlphaToCoverage,
		SampleCoverage,
		ScissorTest,
		StencilTest,
		Multisample
	};

	enum class RasterizationMode : uint8_t
	{
		Point,
		Line,
		Fill
	};

	enum class ComparisonFunc : uint8_t
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};

	enum class Operation : uint8_t
	{
		Zero,
		Keep,
		Replace,
		Increment,
		IncrementWrap,
		Decrement,
		DecrementWrap,
		Invert
	};

	enum class BlendFactor : uint8_t
	{
		Zero = 0,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,

		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		Src1Color,
		OneMinusSrc1Color,
		Src1Alpha,
		OneMinusSrc1Alpha
	};

	enum class BlendEquation : uint8_t
	{
		FuncAdd,
		FuncSubtract,
		FuncReverseSubtract,
		Min,
		Max
	};

	enum class CullFace : uint8_t
	{
		Front = 0,
		Back,
		FrontAndBack
	};

	enum class PrimitiveMode : uint8_t
	{
		Points,
		Lines,
		LineLoop,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
		LinesAdjacency,
		LineStripAdjacency,
		TrianglesAdjacency,
		TriangleStripAdjacency,
		Patches
	};

#if GL_VERSION_4_0
	enum class MemoryBarrierFlags : uint16_t
	{
		VertexAttribArray = 0x1,
		ElementArray = 0x2,
		Uniform = 0x4,
		TextureFetch = 0x8,
		ShaderImageAccess = 0x10,
		Command = 0x20,
		PixelBuffer = 0x40,
		TextureUpdate = 0x80,
		BufferUpdate = 0x100,
		ClientMappedBuffer = 0x200,
		Framebuffer = 0x400,
		TransformFeedback = 0x800,
		AtomicCounter = 0x1000,
		ShaderStorage = 0x2000,
		QueryBuffer = 0x4000,
		All = std::numeric_limits<uint16_t>::max()
	};
#endif

	// Struct representing a range of memory in a buffer.
	struct BufferMemoryRange final
	{
		uint64_t offset{ 0 };
		uint64_t size{ 0 };
	};

	//=============================================================================
	// Function declarations
	//=============================================================================
	inline GLenum EnumToValue(RenderingCapability c) noexcept;
	inline GLenum EnumToValue(AccessSpecifier access) noexcept;
	inline GLenum EnumToValue(PixelDataFormat format) noexcept;
	inline GLenum EnumToValue(TextureType type) noexcept;
	inline GLenum EnumToValue(ImageAccessSpecifier access) noexcept;
	inline GLenum EnumToValue(InternalFormat format) noexcept;
	inline GLenum EnumToValue(Format format) noexcept;
	inline GLenum EnumToValue(PixelDataType type) noexcept;
	inline GLenum EnumToValue(TextureFilteringMode mode) noexcept;
	inline GLenum EnumToValue(TextureWrapMode mode) noexcept;
	inline GLenum EnumToValue(BufferType type) noexcept;
	inline GLenum EnumToValue(DataType type) noexcept;
	inline GLenum EnumToValue(RasterizationMode mode) noexcept;
	inline GLenum EnumToValue(ComparisonFunc func) noexcept;
	inline GLenum EnumToValue(Operation op) noexcept;
	inline GLenum EnumToValue(CullFace cull) noexcept;
	inline GLenum EnumToValue(BlendFactor factor) noexcept;
	inline GLenum EnumToValue(BlendEquation eq) noexcept;
	inline GLenum EnumToValue(PrimitiveMode mode) noexcept;
#if GL_VERSION_4_0
	inline GLbitfield EnumToValue(MemoryBarrierFlags mode) noexcept;
#endif

} // namespace ogl


//=============================================================================
// Implementation
//=============================================================================
#include "OGLCore.inl"