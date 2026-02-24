//=============================================================================
// Implementation
//=============================================================================

inline GLenum EnumToValue(DataType type) noexcept
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

inline GLenum EnumToValue(RasterizationMode mode) noexcept
{
	switch (mode) {
	case RasterizationMode::Point: return GL_POINT;
	case RasterizationMode::Line:  return GL_LINE;
	case RasterizationMode::Fill:  return GL_FILL;
	default: std::unreachable();
	}
}

inline GLenum EnumToValue(ComparisonFunc func) noexcept
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

inline GLenum EnumToValue(Operation op) noexcept
{
	switch (op)
	{
	case Operation::Zero: return GL_ZERO;
	case Operation::Keep: return GL_KEEP;
	case Operation::Replace: return GL_REPLACE;
	case Operation::Increment: return GL_INCR;
	case Operation::IncrementWrap: return GL_INCR_WRAP;
	case Operation::Decrement: return GL_DECR;
	case Operation::DecrementWrap: return GL_DECR_WRAP;
	case Operation::Invert: return GL_INVERT;
	default: std::unreachable();
	}
}

inline GLenum EnumToValue(BlendFactor factor) noexcept
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

inline GLenum EnumToValue(BlendEquation eq) noexcept
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

inline GLenum EnumToValue(CullFace cull) noexcept
{
	switch (cull) {
	case CullFace::Front:        return GL_FRONT;
	case CullFace::Back:         return GL_BACK;
	case CullFace::FrontAndBack: return GL_FRONT_AND_BACK;
	default: std::unreachable();
	}
}

inline GLenum EnumToValue(PrimitiveMode mode) noexcept
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
	default: std::unreachable();
	}
}