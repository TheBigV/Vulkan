#pragma once

#pragma region Include
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdint>
#include <string>
#include <ctime>
#include <mutex>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>

#include <half_float/half.hpp>
#pragma endregion

#pragma region
#pragma endregion

namespace KID
{
#pragma region Typedef
#pragma region Basic types
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
	typedef std::string										string;
#pragma endregion
#pragma endregion
#pragma region Macro
	namespace Macro_Default
	{
#define __KID_BAN_COPY__(Type) \
	public: \
		Type(const Type&) = delete; \
		Type(Type&&) = delete; \
	public: \
		Type& operator = (const Type&) = delete; \
		Type& operator = (Type&&) = delete;
	}
	namespace Macro_Dependency
	{
#define __KID_DEPENDENCY_IMPLEMENTATION__(Dependency) \
	public: \
		class Exception_UndeletedDependentObjects: public Exception {}; \
	protected: \
		std::list<Dependency##Dependent*> dependents; \
	public: \
		inline void AttachDependent(Dependency##Dependent* dependent_) \
		{ \
			dependents.push_back(dependent_); \
		} \
		inline void DetachDependent(Dependency##Dependent* dependent_) \
		{ \
			dependents.remove(dependent_); \
		}
#define __KID_DEPENDENCY_CHECK__ \
		if(dependents.size() > 0) \
		{ \
			throw new Exception_UndeletedDependentObjects(); \
		}
#define __KID_DEPENDENT_DECLARTION__(Dependency)  class Dependency##Dependent;
#define __KID_DEPENDENT_IMPLEMENTATION__(Dependency, dependency) \
		class Dependency##Dependent: \
			public Info \
		{ \
		protected: \
			Dependency*const dependency; \
		public: \
			inline Dependency##Dependent(Dependency* dependency##_): \
			dependency(dependency##_) \
			{ \
				__KID_VULKAN_LOG_BEGIN__(Dependency##Dependent()); \
				dependency->AttachDependent(this); \
				__KID_VULKAN_LOG_END__; \
			} \
			inline ~##Dependency##Dependent() \
			{ \
				__KID_VULKAN_LOG_BEGIN__(~Dependency##Dependent()); \
				dependency->DetachDependent(this); \
				__KID_VULKAN_LOG_END__; \
			} \
		public: \
			inline Dependency* Get##Dependency() const \
			{ \
				return dependency; \
			} \
		public: \
			inline string GetInfoHeader() const \
			{ \
				return "[KID][Vulkan]["#Dependency"Dependent]"; \
			} \
			inline string GetInfo() const \
			{ \
				auto info = Info::GetInfo(); \
				return (info.size() > 0 ? (info + "\n") : "") + #Dependency + " = " + std::to_string((uint64)dependency); \
			} \
		};
	}
	namespace Macro_Readonly
	{
#define __KID_CONST_REFERENCE_READONLY_FIELD__(Type, name, Accessor) \
	protected: \
		Type name; \
	public: \
		inline const Type& Accessor() const \
		{ \
			return name; \
		}
#define __KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Type, name, Accessor) \
	protected: \
		const Type name; \
	public: \
		inline const Type& Accessor() const \
		{ \
			return name; \
		}
	}
#pragma endregion
}

#pragma region
#pragma endregion





