#include "MBSlippi_CLI.h"
#include <filesystem>
#include "MBSlippiTest.h"
#include "SlippiSpecParser.h"
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
	//std::filesystem::current_path(std::filesystem::current_path().parent_path().parent_path());
	//MBSlippi::RunTests();
	
	//const char* NewArgv[]= {"MBSlippi", "update-index" };
	//argc = sizeof(NewArgv) / sizeof(const char*);

	//const char* NewArgv[] = { "MBSlippi", "execute","TestEqual.slpspec"};
	//argc = sizeof(NewArgv) / sizeof(const char*);
	//argv = NewArgv;
    return(HandlerToUse.Run(argc, argv));
}
