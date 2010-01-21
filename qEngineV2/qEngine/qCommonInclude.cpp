#include "qCommonInclude.h"

/**************************** [global function]::parseQFile ******************************
description:
   Parses configuration files. These files always have information in the form of:
   key=value
   They are used for basic screen and engine configuration, as well as other misc tasks
   such as font sizes.

@strFilename[in]:
   Name of the file to be parsed.
@mValueMap[in]:
   Map contain string keys point to string values. The value can be converted using
   boost::lexical_cast to the value type needed.

value:
   Returns true if the file was parsed successfully.

access: public
------------------------------------------------------------------------------------------
created: 21.11.02 - 19:42:52
creator: qDot
*****************************************************************************************/

bool parseQFile(string strFilename, map<string, string>* mValueMap)
{
		ifstream fConsoleFile(strFilename.c_str());
		if(!fConsoleFile)
		{
			return false;
		}	
		string strTempRead;
		string strName, strValue;
		int i = 0;
		while(!fConsoleFile.eof())
		{
			//TODO: Update Console to use tokenizer instead of hacked string parser
			fConsoleFile >> strTempRead;
			i = strTempRead.find("=");
			if(i != string::npos)
			{
				mValueMap->insert(pair<string, string>(strTempRead.substr(0, i),strTempRead.substr(i+1, strTempRead.length())));
			}
		}
		fConsoleFile.close();
		return true;
}