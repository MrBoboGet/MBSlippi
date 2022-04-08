
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
	
	std::string TotalRawData = "";
	uint32_t TotalEvents[11];
	std::memset(TotalEvents, 0, sizeof(TotalEvents));
	
	for (size_t i = 0; i < AllEvents.size(); i++)
	{
		size_t EventIndex = size_t(AllEvents[i].GetType());
		if (EventIndex == 0x10)
		{
			EventIndex -= 0x10;
		}
		else
		{
			EventIndex -= 0x34;
		}
		TotalEvents[EventIndex] += 1;
		TotalRawData += AllEvents[i].Serialize(MBSlippi::ParseVersion{ 3,6,0 });
	}
	assert(TotalRawData.size() == EventData.size());
	assert(std::memcmp(TotalRawData.data(), EventData.data(), 29) == 0);
	for (size_t i = 0; i < TotalRawData.size(); i++)
	{
		if (TotalRawData[i] != EventData[i])
		{
			size_t DebugPos = i - 29;
			uint8_t* BytePointer =(uint8_t*)  TotalRawData.data();
			std::cout << "First differing byte position: " << i<<std::endl;
			break;
		}
	}
	std::vector<std::vector<MBSlippi::Event>> TotalFrames = {};
	MBSlippi::FrameParser FrameParser;
	FrameParser.SetVersion(EventParser.GetVersion());
	for (size_t i = 0; i < AllEvents.size(); i++)
	{
		FrameParser.InsertEvent(AllEvents[i]);
		if (FrameParser.AvailableFrames() > 0)
		{
			TotalFrames.push_back(FrameParser.ExtractFrame());
		}
	}
	while (FrameParser.AvailableFrames() > 0)
	{
		TotalFrames.push_back(FrameParser.ExtractFrame());
	}
	std::cout << "Total number of frames: " << TotalFrames.size() << std::endl;
	
	MBParsing::JSONObject TestCopy = ResultObject;
	TestCopy.GetAttribute("raw") = MBParsing::JSONObject(TotalRawData);
	std::ofstream FileOutStream("ReplayVariable.slp", std::ios::out | std::ios::binary);
	MBUtility::MBFileOutputStream OutStream(&FileOutStream);
	//direkt till filen test
	//OutStream.Write("\x7b\x55\x03\x72\x61\x77\x5b\x24\x55\x23\x6c", 11);
	//OutStream.Write(TotalRawData.data(),TotalRawData.size());
	//OutStream.Write("U\x08metadata",10);
	//MBParsing::SerialiseUBJSON(OutStream, TestCopy.GetAttribute("metadata"));
	MBParsing::SerialiseUBJSON(OutStream, TestCopy);

	MBUtility::MBFileInputStream CopiedInputStream("ReplayVariable.slp");
	MBParsing::JSONObject CopiedResultObject = MBParsing::ParseUBJSON(&CopiedInputStream, nullptr);
	std::cout << CopiedResultObject.GetAttribute("metadata").ToString() << std::endl;
	assert(CopiedResultObject.GetAttribute("raw").GetStringData() == TotalRawData);

	std::ofstream CorrectRawData("CorrectRawData", std::ios::out | std::ios::binary);
	CorrectRawData.write(EventData.data(),EventData.size());
	std::ofstream MyRawData("MyRawData", std::ios::out | std::ios::binary);
	MyRawData.write(TotalRawData.data(), TotalRawData.size());
	//eld testet, spela varannan frame 
	std::cout << AllEvents.size() << std::endl;

}