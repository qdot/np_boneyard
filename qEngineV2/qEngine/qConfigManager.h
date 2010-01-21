/************************************************************************
Class Name: qConfigManager
Date: 10/13/02
Purpose: This class holds the configuration values which are used by the
rest of the engine to initilize and save settings between executions.
************************************************************************/

#ifndef Q_CONFIG
#define Q_CONFIG 1

/************************************************************************/
/* Includes                                                             */
/************************************************************************/

#include <qEngine/qCommonInclude.h>
#include <qEngine/qConsole.h>

/************************************************************************/
/* Class Declarations                                                   */
/************************************************************************/
namespace qEngine
{
	class qConfigManager
	{
	public:
		qConfigManager(void);
		qConfigManager(boost::shared_ptr< qConsole > spConsole);
		~qConfigManager(void);
		bool loadValuesFromFile(string strFilename);
		string getValue(string strName);
		bool hasValue(string strName);
		bool writeValuesToFile(string strFilename);
		void setValue(string strName, string strValue);
		void clearValues();
		static void consoleFunction(string strCommand, string* pstrParameters);
	private:
		static qConfigManager* m_pThis;
		map<string, string> m_mValueHash;
		boost::shared_ptr< qConsole > m_spConsole;
	};
}

#endif