#pragma once
#include "MBSlippiTypes.h"
#include "MBSlippiParsing.h"
#include <MBScript/MBScript.h>
#include "MBSlippiConfig.h"
namespace MBSlippi
{
	class MBS_SlippiModule;
	class MBS_SlippiPlayerMetadata : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		std::unordered_map<std::string, MBScript::MBSObjectStore> m_Fields = {};
	public:
		MBS_SlippiPlayerMetadata() {};
		MBS_SlippiPlayerMetadata(MBScript::ObjectType TypeToUse,std::unordered_map<std::string,MBScript::MBSObjectStore> FieldsToUse);
		virtual MBScript::ObjectType GetType() const override
		{
			return(m_Type);
		}
		virtual std::unique_ptr<MBScript::MBSObject> Copy() const override;
		virtual std::unique_ptr<MBScript::MBSObject> DotOperator(std::string const& FieldName) override;
	};
	class MBS_SlippiPlayerFrameInfo : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		std::unordered_map<std::string, MBScript::MBSObjectStore> m_Fields = {};

	public:
		MBS_SlippiPlayerFrameInfo() {};
		MBS_SlippiPlayerFrameInfo(Event PostFrameUpdate, MBS_SlippiModule& AssociatedModule);
		virtual MBScript::ObjectType GetType() const override
		{
			return(m_Type);
		}
		virtual std::unique_ptr<MBScript::MBSObject> Copy() const override;
		virtual std::unique_ptr<MBScript::MBSObject> DotOperator(std::string const& FieldName) override;
	};
	class MBS_SlippiFrame : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		std::unordered_map<std::string, MBScript::MBSObjectStore> m_Fields = {};
	public:
		MBS_SlippiFrame() {};
		MBS_SlippiFrame(std::vector<Event> FrameEvents,MBS_SlippiModule& AssociatedModule);
		virtual MBScript::ObjectType GetType() const override
		{
			return(m_Type);
		}
		virtual std::unique_ptr<MBScript::MBSObject> Copy() const override;
		virtual std::unique_ptr<MBScript::MBSObject> DotOperator(std::string const& FieldName) override;
	};
	class MBS_SlippiGame : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		std::unordered_map<std::string, MBScript::MBSObjectStore> m_Fields = {};
	public:
		MBS_SlippiGame() {};
		MBS_SlippiGame(std::string const& GamePath, MBS_SlippiModule* AssociatedModule);
		virtual MBScript::ObjectType GetType() const override
		{
			return(m_Type);
		}
		virtual std::unique_ptr<MBScript::MBSObject> Copy() const override;
		virtual std::unique_ptr<MBScript::MBSObject> DotOperator(std::string const& FieldName) override;
	};

	class MBS_SlippiReplayInfo : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		std::unordered_map<std::string, MBScript::MBSObjectStore> m_Fields = {};
	public:
		virtual MBScript::ObjectType GetType() const override
		{
			return(m_Type);
		}
		MBS_SlippiReplayInfo() {};
		MBS_SlippiReplayInfo(MBS_SlippiModule& AssociatedModule, MBScript::ArgumentList Arguments);
		virtual std::unique_ptr<MBScript::MBSObject> Copy() const override;
		virtual std::unique_ptr<MBScript::MBSObject> DotOperator(std::string const& FieldName) override;
	};

	enum class MBSSlippiTypes
	{
		Frame,
		PlayerFrameInfo,
		Metadata,
		PlayerMetadata,
		ReplayInfo
	};

	//Not knowing if order is relevant im going to assume that it is
	struct DolphinConfigOption
	{
		std::string Name;
		std::string Value;
	};
	class DolphinConfigParser
	{
	private:
		std::vector<std::pair<std::string, std::vector<DolphinConfigOption>>> m_Options;
	public:
		DolphinConfigParser() {};
		DolphinConfigParser(std::string const& FileToReadPath);
		void InsertValue(std::string const& OptionType, std::string const& OptionName, std::string const& OptionValue);
		void RemoveValue(std::string const& OptionType, std::string const& OptionName);
		void WriteValues(std::string const& Path) const;
	};
	class MBS_SlippiModule : public MBScript::MBScriptModule
	{
	private:
		std::unique_ptr<MBScript::MBSObject> ActionableFrames(MBScript::ArgumentList Argumnets);
		std::unique_ptr<MBScript::MBSObject> InShieldStun(MBScript::ArgumentList Argumnets);
		std::unique_ptr<MBScript::MBSObject> LoadGame(MBScript::ArgumentList Argumnets);
		std::unique_ptr<MBScript::MBSObject> ReplayInfo(MBScript::ArgumentList Arguments);
		std::unique_ptr<MBScript::MBSObject> WriteReplayInfo(MBScript::ArgumentList Arguments);
		std::unique_ptr<MBScript::MBSObject> GetGameQuery(MBScript::ArgumentList Arguments);
		
		std::unique_ptr<MBScript::MBSObject> RecordReplay(MBScript::ArgumentList Arguments);

		//returns the relative path for the dump directory
		std::string p_UpdateDolphinConfigs(DolphinConfigParser* OriginalDolphinINI,DolphinConfigParser* OriginalDolphinGFX);
		void p_RestoreDolphinConfigs(std::string const& DumpPath,DolphinConfigParser const& DolphinINI,DolphinConfigParser const& DolphinGFX);
		
		MBSlippiConfig m_Config;

		std::unordered_map<MBSSlippiTypes, MBScript::ObjectType> m_TypeMap = {};
		
		std::unordered_map<std::string, std::unique_ptr<MBScript::MBSObject>(MBS_SlippiModule::*)(MBScript::ArgumentList)> m_StaticFunctions =
		{
			{"ActionableFrames",&MBS_SlippiModule::ActionableFrames},
			{"InShieldStun",&MBS_SlippiModule::InShieldStun},

			{"LoadGame",&MBS_SlippiModule::LoadGame},
			{"ReplayInfo",&MBS_SlippiModule::ReplayInfo},
			{"WriteReplayInfo",&MBS_SlippiModule::WriteReplayInfo},
			{"RecordReplay",&MBS_SlippiModule::RecordReplay},
			{"GetGameQuery",&MBS_SlippiModule::GetGameQuery},

		};
	public:
		MBS_SlippiModule(MBSlippiConfig ConfigToUse);

		MBScript::ObjectType GetTypeConversion(MBSSlippiTypes TypeToConvert);
		std::vector<std::string> GetExportedFunctions() const override;
		std::unique_ptr<MBScript::MBSObject> ExecuteFunction(std::string const& FunctionName, MBScript::ArgumentList Arguments) override;
		void OnCreate(MBScript::ExecutionEnvironment& AssociatedEnvironment) override;
	};
}