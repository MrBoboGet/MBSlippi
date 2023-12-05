#include "MBSlippi_CLI.h"
#include <filesystem>
#include "MBSlippiTest.h"
#include "MQLParser.h"
#include <MBUtility/MBFiles.h>
int main(int argc, const char** argv)
{
    //try
    //{
    //    std::string TestText = "WITH Player1 DATE>\"asdasd\" SELECT DATE>\"asdasds\" FILTER Hej | Hej2 RECORD \"TestOut.mp4\"";
    //    auto Tokenizer = GetTokenizer();
    //    Tokenizer.SetText(TestText);
    //    SlippiSpec Result = ParseSlippiSpec(Tokenizer);        
    //}
    //catch(std::exception& e)
    //{
    //    std::cout<<"Error parsing string: "<<e.what()<<std::endl;
    //}
    //catch(...)
    //{
    //       
    //}
    //return 0;
    MBSlippi::MBSlippiCLIHandler HandlerToUse;
	//MBSlippi::RunTests();
	
	//const char* NewArgv[]= {"MBSlippi", "update-index" };
	//argc = sizeof(NewArgv) / sizeof(const char*);

	//std::filesystem::current_path("C:\\Users\\emanu\\Desktop\\Program\\C++\\MBMelee\\Analysis\\Sharp\\2023-05-09");
	//std::filesystem::current_path("../../");
	//const char* NewArgv[] = { "MBSlippi", "execute","ModuleTest.slpspec"};
	//argc = sizeof(NewArgv) / sizeof(const char*);
	//argv = NewArgv;
    return(HandlerToUse.Run(argc, argv));
}
