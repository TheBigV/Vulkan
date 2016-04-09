#pragma once

#pragma region Include
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdint>
#include <numeric>

#include <half_float/half.hpp>

#pragma endregion

#pragma region
#pragma endregion

namespace KID
{
#pragma region Typedef
	typedef std::string										string;
	typedef std::int8_t										sint8;
	typedef std::int16_t									sint16;
	typedef std::int32_t									sint32;
	typedef std::int64_t									sint64;
	typedef std::uint8_t									uint8;
	typedef std::uint16_t									uint16;
	typedef std::uint32_t									uint32;
	typedef std::uint64_t									uint64;
	typedef half_float::half								float16;
	typedef std::float_t									float32;
	typedef std::double_t									float64;
	typedef int												unknownType;
	typedef std::string										string;
#pragma endregion

	namespace Math
	{
#pragma region Predeclaration
		class vec2i;
		class vec3i;
		class vec4i;
		class vec2u;
		class vec3u;
		class vec4u;
		class vec2f;
		class vec3f;
		class vec4f;
		class mat2f;
		class mat3f;
		class mat4f;
		class quat;

		inline vec3f operator * (const mat3f& m, const vec3f& v);
		inline vec3f operator * (const mat4f& m, const vec3f& v);
		inline vec4f operator * (const mat4f& m, const vec4f& v);
#pragma endregion
#pragma region Declaration
		class vec2f
		{
		public:
			float32 x, y;
		};
		class vec3f
		{
		public:
			float32 x, y, z;
		};
		class vec4f
		{
		public:
			float32 x, y, z, w;
		};
		class mat2f
		{
		public:
			float32 value[4];
		};
		class mat3f
		{
		public:
			float32 value[9];
		};
		class mat4f
		{
		public:
			float32 value[16];
		};
		class quat
		{
		public:
			float32 x, y, z, w;
		};
#pragma endregion
	}
}

#pragma region
#pragma endregion







