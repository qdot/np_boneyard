/************************************************************************
Class Name: qConsole
Date: 10/18/02
Purpose: This is the main console for the program. It is a quake-like
logging and function binding console that will take messages for 
information storage, as well as commands for changings parts of the 
engine on the fly.
************************************************************************/

#ifndef Q_CONSOLE
#define Q_CONSOLE 1

/************************************************************************/
/* Includes/Defines                                                     */
/************************************************************************/

#include <qEngine/qCommonInclude.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

#define MSG_ERROR		0
#define MSG_COMMON		1
#define MSG_INIT		2
#define MSG_SHUTDOWN	3
#define MSG_TEST		4

#define BUFFER_VECTOR_SIZE 1000

/************************************************************************/
/* Class Declarations                                                   */
/************************************************************************/

namespace qEngine
{

	struct qTConsoleCommand {
		string strCommand;
		string* pstrParameters;

		/*
		bool bHasRun;
		bool hasRun { return bHasRun; }
		void setRun { bHasRun = true; }
		*/
	};

	class qConsole
	{
	typedef bool(*qTConsoleFunc)(qTConsoleCommand);

	public:
		qConsole(void);
		~qConsole(void);
		
		bool init();
		void shutdown();
		
		bool writeMessagesToFile(string strFilename);

		/* Sets and removes function handlers from the map*/
		void addFunction(string strFunctionName, qTConsoleFunc fpFunctionHandler);
		void removeFunction(string strFunctionName);

		/* Sets commands and messages in the console */
		void setMessage(int iMessageType, string strMessage);
		void setMessage(int iMessageType, string strObject, string strMessage);
		void setCommand(qTConsoleCommand qTCCommand);
		
		void getMessage(string* strMsg);
		void getMessages(int i, vector<string>* strMsg);

		/* commands for clearing the various data structures in the console */
		void clearMessageBuffer();
		void clearCommandBuffer();
		void clearConsoleBuffers();
		void clearFunctionMap();

		void runCommands();

		static bool render(boost::shared_ptr< qConsole > spConsole);

	private:
		boost::array<string, 10> m_pstrMessageTypes;
		vector<string> m_vMessageBuffer;
		vector<qTConsoleCommand> m_vCommandBuffer;
		queue<qTConsoleCommand> m_qCommandQueue;
		map<string, qTConsoleFunc> m_mFunctionMap;
		map<int, string> m_mMessageHeaderMap;
		static qConsole* m_pThis;

	};
}

#endif