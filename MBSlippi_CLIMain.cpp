#include "MBSlippi_CLI.h"
#include <filesystem>
#include "MBSlippiTest.h"

#include <MBUtility/MBFiles.h>
int main(int argc, const char** argv)
{
	MBSlippi::MBSlippiCLIHandler HandlerToUse;
	std::filesystem::current_path(std::filesystem::current_path().parent_path());
	//MBSlippi::RunTests();
	
	//const char* NewArgv[]= {"MBSlippi", "update-index" };
	//argc = sizeof(NewArgv) / sizeof(const char*);

	const char* NewArgv[] = { "MBSlippi", "execute","TestScript3.mbs"};
	argc = sizeof(NewArgv) / sizeof(const char*);

	HandlerToUse.Run(argc, NewArgv);
}