#include "SlippiSpecLSP.h"


using namespace std::chrono_literals;
int main(int argc,char** argv)
{
    std::unique_ptr<MBLSP::LSP_Server> MBSpecServer = std::make_unique<MBSlippi::SlippiLSP>();
    std::unique_ptr<MBLSP::TerminalInput> InputStream = std::make_unique<MBLSP::TerminalInput>();
    std::unique_ptr<MBUtility::MBOctetOutputStream> OutStream = std::make_unique<MBUtility::TerminalOutput>();
    MBLSP::LSP_ServerHandler Handler(std::move(InputStream),std::move(OutStream),std::move(MBSpecServer));
    Handler.Run();
}
