#include "MBSlippiTypes.h"
#include <MBScript/MBScript.h>
namespace MBSlippi
{
	class MBS_SlippiFrame : public MBScript::MBSObject
	{
	private:
		MBScript::ObjectType m_Type = MBScript::ObjectType::Null;
		
	public:
		virtual MBScript::ObjectType GetType() const override
		{
			return(m_Type);
		}

	};
	class MBS_SlippiModule : public MBScript::MBScriptModule
	{
	private:

	public:
		std::vector<std::string> GetExportedFunctions() const override;
		std::unique_ptr<MBScript::MBSObject> ExecuteFunction(std::string const& FunctionName, MBScript::ArgumentList Arguments) override;
		void OnCreate(MBScript::ExecutionEnvironment& AssociatedEnvironment) override;
	};
}