#include "stdafx.h"
#include "CppUnitTest.h"

#include "GString.h"
#include <Windows.h>
#include <stdexcept> // might be needed?
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestEngineCore
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			OutputDebugString(TEXT("hello from test"));
#if 0
			try 
			{
				GString one("One");
				GString alsoOne("One");
				GString two("Two");

				Assert::IsTrue(one == alsoOne);
				Assert::IsTrue(one != two);



				GString one("one");
				GString two("two");
				GString alsoOne;
				alsoOne = one;

				assert(alsoOne == alsoOne);
				assert(one == alsoOne);
				assert(alsoOne == one);
				assert(one != two);

				OutputDebugString(TEXT("\nhash"));
				OutputDebugString(std::to_wstring(one.toHash()).c_str());
				OutputDebugString(TEXT("\nhash"));
				OutputDebugString(std::to_wstring(two.toHash()).c_str());





			}
			catch (std::exception& e)
			{
				OutputDebugString( GString(e.what()).toWideString().c_str() );
			}
			catch (std::exception* e)
			{
				OutputDebugString( GString(e->what()).toWideString().c_str() );
				delete e;
			}
#endif
		}

	};
}