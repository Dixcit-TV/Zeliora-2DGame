#include "pch.h"
#include "Core.h"
#include <ctime>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

void StartHeapControl();
void DumpMemoryLeaks();

int main( int argc, char *argv[] )
{
	srand(static_cast<unsigned int>(time(nullptr)));
	{
		StartHeapControl();

		try 
		{
			Core core{ Window{ "Project Zeliora - Vincent, Thomas - 1DAE11", 1280.f, 720.f } };
			core.Run();
		}
		catch (const std::exception& ex)
		{
			std::cerr << ex.what() << std::endl;
			std::cin.get();
		}
		catch (...)
		{
			std::cerr << "Unexpected Error has occured." << std::endl;
			std::cin.get();
		}
	}
	DumpMemoryLeaks();
	return 0;
}


void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc(15080);
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}


