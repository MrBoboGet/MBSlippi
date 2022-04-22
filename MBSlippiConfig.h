#pragma once
#include <MBParsing/MBParsing.h>

namespace MBSlippi
{
	struct MBSlippiConfig
	{
		std::string MeleeISOPath;
		std::string ReplayDolphinDirectory;
		std::string ReplaysDirectory;
		MBSlippiConfig() {};
		MBSlippiConfig(MBParsing::JSONObject const& ObjectToParse)
		{
			MeleeISOPath = ObjectToParse.GetAttribute("MeleeISOPath").GetStringData();
			ReplayDolphinDirectory = ObjectToParse.GetAttribute("ReplayDolphinDirectory").GetStringData();
			ReplaysDirectory = ObjectToParse.GetAttribute("ReplaysDirectory").GetStringData();
		}
	};
}