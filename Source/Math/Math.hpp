#pragma once

#include "../KID/Header.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cstdint>
#include <numeric>
#include <math.h>
#include <limits>

#include <half_float/half.hpp>

#pragma endregion

#pragma region
#pragma endregion

namespace KID
{
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

		inline float32 degrees(float32 x);
		inline float32 radians(float32 x);

		inline float32 sinDg(float32 x);
		inline float32 cosDg(float32 x);
		inline float32 tanDg(float32 x);
		inline float32 ctgDg(float32 x);

#pragma endregion
#pragma region Declaration
		class vec2i
		{
		public:
			sint32 x, y;
		};
		class vec3i
		{
		public:
			sint32 x, y, z;
		};
		class vec4i
		{
		public:
			sint32 x, y, z, w;
		};
		class vec2u
		{
		public:
			uint32 x, y;
		};
		class vec3u
		{
		public:
			uint32 x, y, z;
		};
		class vec4u
		{
		public:
			uint32 x, y, z, w;
		};
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
			static inline mat4f move(const vec3f&m);
			static inline mat4f rotateX(float32 r);
			static inline mat4f rotateY(float32 r);
			static inline mat4f rotateZ(float32 r);
			static inline mat4f rotateZXY(const vec3f& r);
			static inline mat4f scale(const vec3f& s);
			static inline mat4f perspective(float32 fov, float32 aspect, float32 znear, float32 zfar);
			static inline mat4f model(const vec3f& m, const vec3f& r, const vec3f& s);
		public:
			float32 value[16];
		public:
			inline float32 operator [] (size_t i) const
			{
				return value[i];
			}
			inline float32& operator [] (size_t i)
			{
				return value[i];
			}
		public:
			friend inline mat4f operator * (const mat4f a, const mat4f b);
		};
		class quat
		{
		public:
			float32 x, y, z, w;
		};
#pragma endregion
	}
}

inline KID::float32 KID::Math::degrees(float32 x)
{
	return x / (float32)M_PI * 180.0f;
}
inline KID::float32 KID::Math::radians(float32 x)
{
	return x * (float32)M_PI / 180.0f;
}

inline KID::float32 KID::Math::sinDg(float32 x)
{
	return sin(radians(x));
}
inline KID::float32 KID::Math::cosDg(float32 x)
{
	return cos(radians(x));
}
inline KID::float32 KID::Math::tanDg(float32 x)
{
	return tan(radians(x));
}
inline KID::float32 KID::Math::ctgDg(float32 x)
{
	return 1.0f / tan(radians(x));
}

#pragma region mat4f
inline KID::Math::mat4f KID::Math::mat4f::move(const vec3f& m)
{
	return mat4f{{
		1.0f,	0.0f,	0.0f,	m.x,
		0.0f,	1.0f,	0.0f,	m.y,
		0.0f,	0.0f,	1.0f,	m.z,
		0.0f,	0.0f,	0.0f,	1.0f
	}};
}
inline KID::Math::mat4f KID::Math::mat4f::rotateX(float32 r)
{
	float32 s = sinDg(r);
	float32 c = cosDg(r);
	return mat4f{{
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	c,		+s,		0.0f,
		0.0f,	-s,		c,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	}};
}
inline KID::Math::mat4f KID::Math::mat4f::rotateY(float32 r)
{
	float32 s = sinDg(r);
	float32 c = cosDg(r);
	return mat4f{{
		c,		0.0f,	+s,		0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		-s,		0.0f,	c,		0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	}};
}
inline KID::Math::mat4f KID::Math::mat4f::rotateZ(float32 r)
{
	float32 s = sinDg(r);
	float32 c = cosDg(r);
	return mat4f{{
		c,		-s,		0.0f,	0.0f,
		+s,		c,		0.0f,	0.0f,
		0.0f,	0.0f,	1.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	}};
}
inline KID::Math::mat4f KID::Math::mat4f::rotateZXY(const vec3f& r)
{
	return rotateZ(r.z) * rotateX(r.x) * rotateY(r.y);
}
inline KID::Math::mat4f KID::Math::mat4f::scale(const vec3f& s)
{
	return mat4f{{
		s.x,	0.0f,	0.0f,	0.0f,
		0.0f,	s.y,	0.0f,	0.0f,
		0.0f,	0.0f,	s.z,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f
	}};
}
inline KID::Math::mat4f KID::Math::mat4f::perspective(float32 fov, float32 aspect, float32 znear, float32 zfar)
{
	float32 f = ctgDg(fov*0.5f);
	float32 a = (zfar + znear) / (znear - zfar);
	float32 b = (2.0f * zfar * znear) / (znear - zfar);

	return mat4f{{
		f/aspect,	0.0f,	0.0f,	0.0f,
		0.0f,		f,		0.0f,	0.0f,
		0.0f,		0.0f,	a,		b,
		0.0f,		0.0f,	-1.0f,	0.0f
	}};

	/*val[0] = f / aspect;
	val[1] = _0f;
	val[2] = _0f;
	val[3] = _0f;

	val[4] = _0f;
	val[5] = f;
	val[6] = _0f;
	val[7] = _0f;

	val[8] = _0f;
	val[9] = _0f;
	val[10] = A;
	val[11] = B;

	val[12] = _0f;
	val[13] = _0f;
	val[14] = -_1f;
	val[15] = _0f;*/
}
inline KID::Math::mat4f KID::Math::mat4f::model(const vec3f& m, const vec3f& r, const vec3f& s)
{
	return scale(s) * rotateZXY(r) * move(m);
}
inline KID::Math::mat4f KID::Math::operator * (const mat4f a, const mat4f b)
{
	return mat4f{{
		(b[0] * a[0]	+ b[1] * a[4]	+ b[2] * a[8]	+ b[3] * a[12]),
		(b[0] * a[1]	+ b[1] * a[5]	+ b[2] * a[9]	+ b[3] * a[13]),
		(b[0] * a[2]	+ b[1] * a[6]	+ b[2] * a[10]	+ b[3] * a[14]),
		(b[0] * a[3]	+ b[1] * a[7]	+ b[2] * a[11]	+ b[3] * a[15]),

		(b[4] * a[0]	+ b[5] * a[4]	+ b[6] * a[8]	+ b[7] * a[12]),
		(b[4] * a[1]	+ b[5] * a[5]	+ b[6] * a[9]	+ b[7] * a[13]),
		(b[4] * a[2]	+ b[5] * a[6]	+ b[6] * a[10]	+ b[7] * a[14]),
		(b[4] * a[3]	+ b[5] * a[7]	+ b[6] * a[11]	+ b[7] * a[15]),

		(b[8] * a[0]	+ b[9] * a[4]	+ b[10] * a[8]	+ b[11] * a[12]),
		(b[8] * a[1]	+ b[9] * a[5]	+ b[10] * a[9]	+ b[11] * a[13]),
		(b[8] * a[2]	+ b[9] * a[6]	+ b[10] * a[10]	+ b[11] * a[14]),
		(b[8] * a[3]	+ b[9] * a[7]	+ b[10] * a[11]	+ b[11] * a[15]),

		(b[12] * a[0]	+ b[13] * a[4]	+ b[14] * a[8]	+ b[15] * a[12]),
		(b[12] * a[1]	+ b[13] * a[5]	+ b[14] * a[9]	+ b[15] * a[13]),
		(b[12] * a[2]	+ b[13] * a[6]	+ b[14] * a[10]	+ b[15] * a[14]),
		(b[12] * a[3]	+ b[13] * a[7]	+ b[14] * a[11]	+ b[15] * a[15])
	}};
}
#pragma endregion

inline KID::Math::vec3f KID::Math::operator * (const mat4f& m, const vec3f& v)
{
	return vec3f{
		m[0]*v.x	+ m[1]*v.y	+ m[2]*v.z	+ m[3],
		m[4]*v.x	+ m[5]*v.y	+ m[6]*v.z	+ m[7],
		m[8]*v.x	+ m[9]*v.y	+ m[10]*v.z	+ m[11]
	};
}

#pragma region
#pragma endregion







