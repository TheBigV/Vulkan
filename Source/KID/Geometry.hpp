#pragma once

#include <Math/Math.hpp>
#pragma comment(lib, "Math.lib")

namespace KID
{
	namespace Geometry
	{
		using namespace Math;
		class Mesh
		{
		public:
			enum class VertexFormat: uint32
			{
				Empty = 0,
				Float16 = sizeof(float16),
				Float32 = sizeof(float32)
			};
			enum class IndexFormat: uint32
			{
				Empty = 0,
				UInt16 = sizeof(uint16),
				Uint32 = sizeof(uint32)
			};
		public:
			uint32 vCount = 0;
			uint32 iCount = 0;
			vec3f*	vPos = nullptr;
			vec2f*	vTex = nullptr;
			vec3f*	vTan = nullptr;
			vec3f*	vBin = nullptr;
			vec3f*	vNor = nullptr;
			uint32*	vInd;
		public:
			inline uint32 GetVertexCount() const
			{
				return vCount;
			}
			inline uint32 GetIndexCount() const
			{
				return iCount;
			}
			inline uint32 GetVertexSize(VertexFormat fPos, VertexFormat fTex, VertexFormat fTan, VertexFormat fBin, VertexFormat fNor) const
			{
				return (uint32)fPos * 3 + (uint32)fTex * 2 + (uint32)fTan * 3 + (uint32)fBin * 3 + (uint32)fNor * 3;
			}
			inline uint32 GetIndexSize(IndexFormat fInd) const
			{
				return (uint32)fInd;
			}
			inline void Create(uint32 vCount_, uint32 iCount_)
			{
				vCount = vCount_;
				iCount = iCount_;
				vPos = new vec3f[vCount];
				vTex = new vec2f[vCount];
				vTan = new vec3f[vCount];
				vBin = new vec3f[vCount];
				vNor = new vec3f[vCount];
				vInd = new uint32[iCount];
			}
			inline void CreateSphere(const vec3f& size, const vec2f& tex, const vec2u& seg)
			{
				if(seg.x < 3 || seg.y < 3) return;

				Create((seg.x + 1)*(seg.y + 1), 6 * seg.x*seg.y);

				for(uint32 x = 0; x <= seg.x; ++x)
				for(uint32 y = 0; y <= seg.y; ++y)
				{
					float32 dx = (float32)x / (float32)seg.x;
					float32 dy = (float32)y / (float32)seg.y;
					uint32 id = y*(seg.x + 1) + x;
					auto rMat = mat4f::rotateZXY(vec3f{90.0f - dy*180.0f, 180.0f - dx*360.0f, 0.0f});
					auto sMat = mat4f::scale(size);
					vPos[id] = {rMat * vec3f{0.0f, 0.0f, 1.0f}};
					vTex[id] = vec2f{dx, dy};// *tex;
					vTan[id] = rMat * vec3f{-1.0f, +0.0f, +0.0f};
					vBin[id] = rMat * vec3f{+0.0f, +1.0f, +0.0f};
					vNor[id] = rMat * vec3f{+0.0f, +0.0f, +1.0f};
				}

				for(uint32 x = 0; x < seg.x; ++x)
				for(uint32 y = 0; y < seg.y; ++y)
				{
					uint32 id = 6 * (y*seg.x + x);
					vInd[id + 0] = (y + 0)*(seg.x + 1) + (x + 0);
					vInd[id + 1] = (y + 0)*(seg.x + 1) + (x + 1);
					vInd[id + 2] = (y + 1)*(seg.x + 1) + (x + 0);
					vInd[id + 3] = vInd[id + 1];
					vInd[id + 4] = (y + 1)*(seg.x + 1) + (x + 1);
					vInd[id + 5] = vInd[id + 2];
				}
			}
			inline void* PackVertex(VertexFormat fPos, VertexFormat fTex, VertexFormat fTan, VertexFormat fBin, VertexFormat fNor)
			{
				auto data = new uint8[GetVertexSize(fPos, fTex, fTan, fBin, fNor)*vCount];

				uint32 t = 0;
				for(uint32 i = 0; i < vCount; ++i)
				{
					switch(fPos)
					{
						case VertexFormat::Float16:
						{
							((float16*)(data + t))[0] = vPos[i].x;
							((float16*)(data + t))[1] = vPos[i].y;
							((float16*)(data + t))[2] = vPos[i].z;
						} break;
						case VertexFormat::Float32:
						{
							((float32*)(data + t))[0] = vPos[i].x;
							((float32*)(data + t))[1] = vPos[i].y;
							((float32*)(data + t))[2] = vPos[i].z;
						} break;
					}
					t += (uint32)fPos * 3;

					switch(fTex)
					{
						case VertexFormat::Float16:
						{
							((float16*)(data + t))[0] = vTex[i].x;
							((float16*)(data + t))[1] = vTex[i].y;
						} break;
						case VertexFormat::Float32:
						{
							((float32*)(data + t))[0] = vTex[i].x;
							((float32*)(data + t))[1] = vTex[i].y;
						} break;
					}
					t += (uint32)fTex * 2;

					switch(fTan)
					{
						case VertexFormat::Float16:
						{
							((float16*)(data + t))[0] = vTan[i].x;
							((float16*)(data + t))[1] = vTan[i].y;
							((float16*)(data + t))[2] = vTan[i].z;
						} break;
						case VertexFormat::Float32:
						{
							((float32*)(data + t))[0] = vTan[i].x;
							((float32*)(data + t))[1] = vTan[i].y;
							((float32*)(data + t))[2] = vTan[i].z;
						} break;
					}
					t += (uint32)fTan * 3;

					switch(fBin)
					{
						case VertexFormat::Float16:
						{
							((float16*)(data + t))[0] = vBin[i].x;
							((float16*)(data + t))[1] = vBin[i].y;
							((float16*)(data + t))[2] = vBin[i].z;
						} break;
						case VertexFormat::Float32:
						{
							((float32*)(data + t))[0] = vBin[i].x;
							((float32*)(data + t))[1] = vBin[i].y;
							((float32*)(data + t))[2] = vBin[i].z;
						} break;
					}
					t += (uint32)fBin * 3;

					switch(fNor)
					{
						case VertexFormat::Float16:
						{
							((float16*)(data + t))[0] = vNor[i].x;
							((float16*)(data + t))[1] = vNor[i].y;
							((float16*)(data + t))[2] = vNor[i].z;
						} break;
						case VertexFormat::Float32:
						{
							((float32*)(data + t))[0] = vNor[i].x;
							((float32*)(data + t))[1] = vNor[i].y;
							((float32*)(data + t))[2] = vNor[i].z;
						} break;
					}
					t += (uint32)fNor * 3;
				}

				return data;
			}
			inline void* PackIndex(IndexFormat fInd)
			{
				auto data = new uint8[GetIndexSize(fInd) * iCount];

				switch(fInd)
				{
					case IndexFormat::UInt16:
					{
						for(uint32 i = 0; i < iCount; ++i) ((uint16*)data)[i] = vInd[i];
					} break;
					case IndexFormat::Uint32:
					{
						for(uint32 i = 0; i < iCount; ++i) ((uint32*)data)[i] = vInd[i];
					} break;
				}

				return data;
			}
		};
	}
}