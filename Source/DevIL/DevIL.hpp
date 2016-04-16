#pragma once
#include "../KID/Header.hpp"

#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>
//#pragma comment (lib, "DevIL.lib")
//#pragma comment (lib, "ILU.lib")
//#pragma comment (lib, "ILUT.lib")


namespace KID
{
	namespace DevIL
	{
		class Initer
		{
		protected:
			bool							isInit = false;
		public:
			Initer();
		};
		static Initer						initer;

		bool								ErrorTest();

		ILinfo								Load(const string& filename);
	}
}




















