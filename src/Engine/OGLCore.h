#pragma once

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
};

//=============================================================================
// Function declarations
//=============================================================================
inline GLenum EnumToValue(DataType type) noexcept;
inline GLenum EnumToValue(RasterizationMode mode) noexcept;
inline GLenum EnumToValue(ComparisonFunc func) noexcept;
inline GLenum EnumToValue(Operation op) noexcept;
inline GLenum EnumToValue(BlendFactor factor) noexcept;
inline GLenum EnumToValue(BlendEquation eq) noexcept;
inline GLenum EnumToValue(PrimitiveMode mode) noexcept;

//=============================================================================
// Implementation
//=============================================================================
#include "OGLCore.inl"