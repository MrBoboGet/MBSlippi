#include "MBSlippiTypes.h"
#include "MBSlippiParsing.h"
#include <MBScript/MBScript.h>
namespace MBSlippi
{
	class MBS_SlippiPlayerMetadata : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		std::unordered_map<std::string, std::unique_ptr<MBScript::MBSObject>> m_Fields = {};
	public:
		MBS_SlippiPlayerMetadata() {};
		MBS_SlippiPlayerMetadata(MBScript::ObjectType TypeToUse,std::unordered_map<std::string, std::unique_ptr<MBScript::MBSObject>> FieldsToUse);
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
		std::unordered_map<std::string, std::unique_ptr<MBScript::MBSObject>> m_Fields = {};

	public:
		MBS_SlippiPlayerFrameInfo() {};
		MBS_SlippiPlayerFrameInfo(Event PostFrameUpdate);
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
		std::unordered_map<std::string, std::unique_ptr<MBScript::MBSObject>> m_Fields = {};
	public:
		MBS_SlippiFrame() {};
		MBS_SlippiFrame(std::vector<Event> PostFrameUpdates);
		virtual MBScript::ObjectType GetType() const override
		{
			return(m_Type);
		}
		virtual std::unique_ptr<MBScript::MBSObject> Copy() const override;
		virtual std::unique_ptr<MBScript::MBSObject> DotOperator(std::string const& FieldName) override;
	};
	class MBS_SlippiModule;
	class MBS_SlippiGame : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		std::unordered_map<std::string, std::unique_ptr<MBScript::MBSObject>> m_Fields = {};
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
	enum class MBSSlippiTypes
	{
		Frame,
		PlayerFrameInfo,
		Metadata,
		PlayerMetadata,
	};
	class MBS_SlippiModule : public MBScript::MBScriptModule
	{
	private:
		std::unique_ptr<MBScript::MBSObject> ActionableFrames(MBScript::ArgumentList Argumnets);
		std::unique_ptr<MBScript::MBSObject> InShieldStun(MBScript::ArgumentList Argumnets);
		std::unique_ptr<MBScript::MBSObject> LoadGame(MBScript::ArgumentList Argumnets);
		
		std::unordered_map<MBSSlippiTypes, MBScript::ObjectType> m_TypeMap = {};
		
		std::unordered_map<std::string, std::unique_ptr<MBScript::MBSObject>(MBS_SlippiModule::*)(MBScript::ArgumentList)> m_StaticFunctions =
		{
			{"ActionableFrames",&MBS_SlippiModule::ActionableFrames},
			{"InShieldStun",&MBS_SlippiModule::InShieldStun},
			{"LoadGame",&MBS_SlippiModule::LoadGame},

		};
	public:
		MBScript::ObjectType GetTypeConversion(MBSSlippiTypes TypeToConvert);

		std::vector<std::string> GetExportedFunctions() const override;
		std::unique_ptr<MBScript::MBSObject> ExecuteFunction(std::string const& FunctionName, MBScript::ArgumentList Arguments) override;
		void OnCreate(MBScript::ExecutionEnvironment& AssociatedEnvironment) override;
	};
}