
#include "MBSlippi.h"

#include <iostream>
#include <MBParsing/MBParsing.h>
#include <filesystem>
int main(int argc, const char** argv)
{
	std::filesystem::current_path(std::filesystem::current_path().parent_path());
	MBUtility::MBFileInputStream InputStream("./Game_20200630T190553.slp");
	MBParsing::JSONObject ResultObject = MBParsing::ParseUBJSON(&InputStream, nullptr);
	std::cout << ResultObject["metadata"].ToString() << std::endl;
	std::string const& EventData = ResultObject["raw"].GetStringData();
	std::unique_ptr<MBUtility::MBOctetInputStream> RawDataStream = std::make_unique<MBUtility::MBBufferInputStream>(EventData.data(), EventData.size());
	MBSlippi::SlippiEventParser EventParser = MBSlippi::SlippiEventParser(std::move(RawDataStream));
	MBSlippi::Event CurrentEvent = MBSlippi::Event(std::make_unique<MBSlippi::Event_Null>());
	std::vector<MBSlippi::Event> AllEvents = {};
	while (true)
	{
		CurrentEvent = EventParser.GetNextEvent();
		if (CurrentEvent.GetType() == MBSlippi::EventType::Null)
		{
			break;
		}
		AllEvents.push_back(std::move(CurrentEvent));
	}
	std::cout << AllEvents.size() << std::endl;

}