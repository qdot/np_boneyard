#include "qconsole.h"

namespace qEngine
{

	//Static pointer to this object, used for static callback functions
	qConsole* qEngine::qConsole::m_pThis = NULL;

	/********************************** qConsole::qConsole ***********************************
	description:
	Constructor. Runs the initialization function.

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:21:29
	creator: qDot
	*****************************************************************************************/

	qConsole::qConsole(void)
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

	/********************************* qConsole::~qConsole ***********************************
	description:
	Destructor. Runs the shutdown function.

	access: public, state: virtual
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:21:46
	creator: qDot
	*****************************************************************************************/

	qConsole::~qConsole(void)
	{
		m_pThis = NULL;
		shutdown();
	}

	/************************************ qConsole::init *************************************
	description:
	Initializes internal class object pointer and vectors.

	value:
	Returns true if initialization is successful.

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:22:52
	creator: qDot
	*****************************************************************************************/

	bool qConsole::init()
	{
		m_vMessageBuffer.reserve(BUFFER_VECTOR_SIZE);
		m_vCommandBuffer.reserve(BUFFER_VECTOR_SIZE);
		m_pstrMessageTypes[MSG_ERROR] = "ERROR";
		m_pstrMessageTypes[MSG_COMMON] = "COMMON";
		m_pstrMessageTypes[MSG_INIT] = "INIT";
		m_pstrMessageTypes[MSG_SHUTDOWN] = "SHUTDOWN";
		m_pstrMessageTypes[MSG_TEST] = "TEST";
		setMessage(MSG_INIT, "qConsole", "init : Console initialized.");
		return true;
	}

	/********************************** qConsole::shutdown ***********************************
	description:
	Clears out console vectors and maps, and deletes the internal pointer.

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:23:30
	creator: qDot
	*****************************************************************************************/

	void qConsole::shutdown()
	{
		clearConsoleBuffers();
		clearFunctionMap();
	}

	/**************************** qConsole::writeMessagesToFile ******************************
	description:
	Outputs all messages that have been sent through the console to a file.

	@strFilename[in]:
	Name of the file to write values to.

	value:
	Returns true if file is written successfully

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:25:29
	creator: qDot
	*****************************************************************************************/

	bool qConsole::writeMessagesToFile(string strFilename)
	{
		ofstream fOutputFile(strFilename.c_str(), ios::app);
		if(!fOutputFile)
		{
			setMessage(MSG_ERROR, "qConsole", "writeMessagesToFile : Cannot open file '" + strFilename + "' for console output");
			return false;
		}

		/* _ftime stuff ripped out of MSDN entry for time -> getting current */

  		struct __timeb64 timebuffer;
		char *timeline;
		char *test = (char*)malloc(sizeof(char) * 100);
		_ftime64( &timebuffer );
		timeline = _ctime64( & ( timebuffer.time ) );

		sprintf(test, "%.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20] );

		fOutputFile << "qConsole Console Output (Messages/Errors) for run at " << test;
		for(unsigned int i = 0; i < m_vMessageBuffer.size(); ++i)
		{
			fOutputFile << m_vMessageBuffer[i] << endl;
		}
		fOutputFile <<"--------------------------------------------"<<endl;
		free(test);
		fOutputFile.close();
		return true;
	}

	/********************************* qConsole::setMessage **********************************
	description:
	Adds a message to the message buffer.

	@iMessageType[in]:
	Relates to a constant set in the console header. Specifies the different types of
	messages that can be sent.
	@strMessage[in]:
	The message to be added.

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:28:51
	creator: qDot
	*****************************************************************************************/

	void qConsole::setMessage(int iMessageType, string strMessage)
	{
		m_vMessageBuffer.push_back(m_pstrMessageTypes[iMessageType] + " : " + strMessage);
	}

	/********************************* qConsole::setMessage **********************************
	description:
	Same idea as setMessage, except this also takes the name of an object to add to the
	message

	@iMessageType[in]:
	Relates to a constant set in the console header. Specifies the different types of
		messages that can be sent.
	@strObject[in]:
	Name of the object that sent the message
	@strMessage[in]:
	The message to be added.

	access: public, state: static
	------------------------------------------------------------------------------------------
	created: 15.10.02 - 00:05:16
	creator: qDot
	*****************************************************************************************/

	void qConsole::setMessage(int iMessageType, string strObject, string strMessage)
	{
		m_vMessageBuffer.push_back(m_pstrMessageTypes[iMessageType] + " : " + strObject + " : " + strMessage);
	}

	/********************************* qConsole::setCommand **********************************
	description:
	Sets the command string to be added to the command buffer.

	@strCommand[in]:
	Command and parameters

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:29:53
	creator: qDot
	*****************************************************************************************/

	void qConsole::setCommand(qTConsoleCommand qTCCommand)
	{
		m_qCommandQueue.push(qTCCommand);
	}

	/***************************** qConsole::clearMessageBuffer ******************************
	description:
	Clears the message vector

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:30:09
	creator: qDot
	*****************************************************************************************/

	void qConsole::clearMessageBuffer()
	{
		m_vMessageBuffer.clear();
	}

	/***************************** qConsole::clearCommandBuffer ******************************
	description:
	Clears out the command buffer

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:30:28
	creator: qDot
	*****************************************************************************************/

	void qConsole::clearCommandBuffer()
	{
		m_vCommandBuffer.clear();
	}

	/****************************** qConsole::clearFunctionmap *******************************
	description:
	Clears out the function map

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:30:43
	creator: qDot
	*****************************************************************************************/

	void qConsole::clearFunctionMap()
	{
		m_mFunctionMap.clear();
	}

	/**************************** qConsole::clearConsoleBuffers ******************************
	description:
	Clears out all buffers in the console

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:30:56
	creator: qDot
	*****************************************************************************************/

	void qConsole::clearConsoleBuffers()
	{
		clearMessageBuffer();
		clearCommandBuffer();
	}

	/********************************* qConsole::runCommand **********************************
	description:
	Runs the command farthest back in the command vector that hasn't been run. This uses
	the vector as a queue data structure.

	value:
	Returns true if the command is run successfully, false if it is invalid or else throws
	an error.

	access: public
	------------------------------------------------------------------------------------------
	created: 14.10.02 - 23:42:45
	creator: qDot
	*****************************************************************************************/

	void qConsole::runCommands()
	{
		/*
		qTConsoleCommand _Command;
		while((_Command = m_qCommandQueue.front()) != NULL)
		{
			for(map<string, qTConsoleFunc>::iterator itr = m_mFunctionMap.begin(); itr != m_mFunctionMap.end(); ++itr)
			{
				itr->second(_Command);
			}
			m_qCommandQueue.pop();
			m_vCommandBuffer.push_back(_Command);
		}
		*/
	}

	/******************************** qConsole::addFunction **********************************
	description:
	Adds a function to the handler map

	@strFunctionName[in]:
	Reference name of the function. No idea how I'm actually gonna use this yet, but, well,
	I think I have an idea.
	@fpFunctionHandler[in]:
	A pointer to the function. All functions passed to this take a qTConsoleCommand, and
	return a boolean value, which denotes whether or not the function was at least
	processed. Most members of the map will be static member functions from other objects
	in the engine.

	access: public
	------------------------------------------------------------------------------------------
	created: 16.10.02 - 23:42:00
	creator: qDot
	*****************************************************************************************/

	void qConsole::addFunction(string strFunctionName, qTConsoleFunc fpFunctionHandler)
	{
		m_mFunctionMap[strFunctionName] = fpFunctionHandler;
		setMessage(MSG_COMMON, "qConsole", "Function " + strFunctionName + " added to console handler vector");
	}

	/******************************* qConsole::removeFunction ********************************
	description:
	Remove a function from the function map.

	@strFunctionName[in]:
	Name of the function to remove

	access: public
	------------------------------------------------------------------------------------------
	created: 16.10.02 - 23:38:15
	creator: qDot
	*****************************************************************************************/

	void qConsole::removeFunction(string strFunctionName)
	{
		m_mFunctionMap.erase(strFunctionName);
		setMessage(MSG_COMMON, "qConsole", "Function " + strFunctionName + " removed to console handler vector");
	}

	/******************************** qConsole::getMessages **********************************
	description:
	Returns a specified number of messages from the console.

	@iNumMsgs[in]:
	Number of messages to put into the vector
	@vMsgBuf[in]:
	Pointer to a vector which will hold the messages.

	access: public
	------------------------------------------------------------------------------------------
	created: 26.11.02 - 02:09:46
	creator: qDot
	*****************************************************************************************/

	void qConsole::getMessages(int iNumMsgs, vector<string>* vMsgBuf)
	{
		if(iNumMsgs > m_vMessageBuffer.size())
		{
			for(int j = 0; j < m_vMessageBuffer.size() && j < iNumMsgs; ++j)
			{
				vMsgBuf->push_back(m_vMessageBuffer[j]);
			}
		}
		else
		{
			for(int j = m_vMessageBuffer.size() - iNumMsgs; j < m_vMessageBuffer.size(); ++j)
			{
				vMsgBuf->push_back(m_vMessageBuffer[j]);
			}
		}
	}
}
