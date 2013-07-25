#include "stdafx.h"
#include "CppUnitTest.h"

#include "Util.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{		
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestToString)
		{
			bool Failed=false;
			if(ToString(10)!="10")
			{
				Failed=true;
			}
			if(ToString(100)!="100")
			{
				Failed=true;
			}
			if(ToString(1)!="1")
			{
				Failed=true;
			}

			if(Failed)
			{
				throw new std::exception("Test failed.");
			}
		}

	};
}