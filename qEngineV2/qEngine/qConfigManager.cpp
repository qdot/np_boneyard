#include "qconfigmanager.h"

namespace qEngine
{

	//Static pointer to this object, used for static callback functions
	qConfigManager * qEngine::qConfigManager::m_pThis = NULL;

	qConfigManager::qConfigManager()
	{
		if(m_pThis == NULL)
		{
			m_pThis = this;
		}
		else
		{
			return;
		}
	}

	/*************************** ~qConfigManager::qConfigManager *****************************
	description:
	Constructor, though there's not really anything to construct.

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 20:40:41
	creator: qDot
	*****************************************************************************************/

	qConfigManager::qConfigManager(boost::shared_ptr< qConsole > spConsole) 
		: m_spConsole(spConsole)
	{
		if(m_pThis == NULL)
		{
			m_pThis = this;
		}
		else
		{
			return;
		}
	}

	/*************************** ~qConfigManager::qConfigManager *****************************
	description:
	Deconstructor. Doesn't do much since we don't store pointers.

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 20:40:15
	creator: qDot
	*****************************************************************************************/
	
	qConfigManager::~qConfigManager(void)
	{
		m_pThis = NULL;
	}
	
	/************************** qConfigManager::loadValuesFromFile ***************************
	description:
	Load values in from an external file.

	@strFilename[in]:
	Name of the file to be loaded.

	value:
	Returns true if file is loaded successfully, false if file cannot be found or
	loaded.

	access: public
	------------------------------------------------------------------------------------------
	created: 13.10.02 - 02:58:43
	creator: qDot
	*****************************************************************************************/

	bool qConfigManager::loadValuesFromFile(string strFilename)
	{
		if(!parseQFile(strFilename, &m_mValueHash))
		{
			return false;
		}
		return true;
	}

	/******************************* qConfigManager::getValue ********************************
	description:
	Returns a string that relates to the key passed in. Since these values are loaded from
	a text file, all values are initially loaded as strings. It is up to the object calling
	this function to cast the data otherwise, most likely using boost::lexical_cast.

	@strName[in]:
	The name of the key to look for in the value hash.

	@strValue[in]:
	A pointer to the string in which the resulting value should be stored. If a null string
	is passed back (""), it is assumed that the value cannot be found.

	value:

	access: public
	------------------------------------------------------------------------------------------
	created: 13.10.02 - 03:01:01
	creator: qDot
	*****************************************************************************************/

	string qConfigManager::getValue(string strName)
	{
		map<string, string>::iterator itr;
		if((itr = m_mValueHash.find(strName)) != m_mValueHash.end())
		{
			return itr->second;
		}
		return "";
	}

	/******************************* qConfigManager::hasValue ********************************
	description:
	Checks to see if a value exists in the value hash.

	@strName[in]:
	Name of the key to look for.

	value:
	Returns true if value exists, false if it does not.

	access: public
	------------------------------------------------------------------------------------------
	created: 13.10.02 - 03:08:11
	creator: qDot
	*****************************************************************************************/

	bool qConfigManager::hasValue(string strName)
	{
		if(m_mValueHash.find(strName) != m_mValueHash.end()) return true;
		return false;
	}

	/************************** qConfigManager::writeValuesToFile ****************************
	description:
	Write values out to a file.

	@strFilename[in]:
	Name of the file to write the information out to.

	value:
	Returns true if file write is successful, false if it is not.

	access: public
	------------------------------------------------------------------------------------------
	created: 13.10.02 - 03:08:56
	creator: qDot
	*****************************************************************************************/

	bool qConfigManager::writeValuesToFile(string strFilename)
	{
		ofstream fOutputFile(strFilename.c_str());
		if(!fOutputFile)
		{
			return false;
		}
		fOutputFile << "qConsole Console Output (Commands):" << endl;
		for(map<string, string>::const_iterator itr = m_mValueHash.begin(); itr != m_mValueHash.end(); ++itr)
		{
			fOutputFile << itr->first << "=" << itr->second << endl;
		}
		fOutputFile.close();
		return true;
	}

	/******************************* qConfigManager::setValue ********************************
	description:
	Sets the value of a certain key to the string passed to the function.

	@strName[in]:
	Name of the key to set
	@strValue[in]:
	Value to set the key to

	access: public
	------------------------------------------------------------------------------------------
	created: 13.10.02 - 03:10:48
	creator: qDot
	*****************************************************************************************/

	void qConfigManager::setValue(string strName, string strValue)
	{
		m_mValueHash[strName] = strValue;
	}

	/***************************** qConfigManager::clearValues *******************************
	description:
	Clears out all data in the value hash

	access: public
	------------------------------------------------------------------------------------------
	created: 13.10.02 - 03:11:53
	creator: qDot
	*****************************************************************************************/

	void qConfigManager::clearValues()
	{
		m_mValueHash.clear();
	}
}
