#include "Command.hpp"
#include "Engine/Core/Utils/StringUtils.hpp"
#include "../Utils/GeneralUtils.hpp"

//===============================================================================================
std::map<String, CommandRegistration*> CommandRegistration::s_callbacks;

//===============================================================================================
Command::Command(const String & commandString)
{
	m_fullCommand = commandString;
	
	Strings commandBrokenUp = BreakSentenceIntoWords(commandString);
	m_commandName = commandBrokenUp.at(0);

	for (uint i = 1; i < commandBrokenUp.size(); i++)
	{
		m_commandArguements.push_back (commandBrokenUp.at(i));
	}
}

//-----------------------------------------------------------------------------------------------
Command::~Command()
{
}

//-----------------------------------------------------------------------------------------------
String Command::GetNextString()
{
	String result = "";

	if (m_commandIndex < m_commandArguements.size())
	{
		result = m_commandArguements.at(m_commandIndex);
		m_commandIndex++;
	}

	return result;
}

//-----------------------------------------------------------------------------------------------
String Command::GetCommandParameters()
{
	std::string result;

	for (uint i = 1; i < m_commandArguements.size(); i++)
	{
		result += (m_commandArguements.at(i) + " ");
	}

	return result;
}

//===============================================================================================
// COMMAND REGISTRATION
//===============================================================================================
CommandRegistration::CommandRegistration(const String & name, const String & syntax, const String & description, 
											command_cb theCallback, bool isHidden)
{
	if (CheckForCallBack(name))
		return;

	m_callback = theCallback;
	m_name = name;
	m_syntax = syntax;
	m_description = description;
	m_isHidden = isHidden;

	s_callbacks[name] = this;
}

//-----------------------------------------------------------------------------------------------
CommandRegistration::~CommandRegistration()
{
	m_callback = nullptr;
}

//-----------------------------------------------------------------------------------------------
void CommandRegistration::DeleteAllRegisteredCommands()
{
	DeleteAndClearMap<String, CommandRegistration*>(s_callbacks);
}

//-----------------------------------------------------------------------------------------------
command_cb CommandRegistration::GetCallBack(const String & callbackName)
{
	std::map<std::string, CommandRegistration*>::iterator callBackIterator;
	callBackIterator = s_callbacks.find(callbackName);

	// return it if found
	if (callBackIterator != s_callbacks.end())
		return callBackIterator->second->m_callback;

	return nullptr;
}

//-----------------------------------------------------------------------------------------------
bool CommandRegistration::CheckForCallBack(const String & callbackName)
{
	std::map<std::string, CommandRegistration*>::iterator callBackIterator;
	callBackIterator = s_callbacks.find(callbackName);

	// We could not find one, so the iterator == the end
	if (callBackIterator == s_callbacks.end())
		return false;

	return true;
}

//-----------------------------------------------------------------------------------------------
Strings CommandRegistration::GetAllTheCommandsNames()
{
	Strings theCommandNames;

	std::map<String, CommandRegistration*>::iterator nameIterator;

	for (nameIterator = s_callbacks.begin(); nameIterator != s_callbacks.end(); nameIterator++)
	{
		if (nameIterator->second->m_isHidden == false)
		{
			std::string currentName = nameIterator->first;
			theCommandNames.push_back(currentName);
		}
	}

	return theCommandNames;
}

//-----------------------------------------------------------------------------------------------
Strings CommandRegistration::GetAllCommandsHelpText()
{
	Strings theCommandNames;

	std::map<String, CommandRegistration*>::iterator nameIterator;

	for (nameIterator = s_callbacks.begin(); nameIterator != s_callbacks.end(); nameIterator++)
	{
		if (nameIterator->second->m_isHidden == false)
		{
			String currentName = nameIterator->second->m_description;
			theCommandNames.push_back(currentName);
		}
	}

	return theCommandNames;
}

//-----------------------------------------------------------------------------------------------
Strings CommandRegistration::GetAllCommandsAndTheirHelp()
{
	Strings allTheText;

	std::map<String, CommandRegistration*>::iterator theIterator;

	for (theIterator = s_callbacks.begin(); theIterator != s_callbacks.end(); theIterator++)
	{
		CommandRegistration* currentCommand = theIterator->second;

		if (currentCommand->m_isHidden == false)
		{
			std::string currentText = std::string(currentCommand->m_name
				+ " | " + currentCommand->m_description);

			allTheText.push_back(currentText);
		}
	}

	return allTheText;
}

//===============================================================================================
// C functions
//===============================================================================================
void CommandRegister(const String & name, const String & syntax, const String & description, command_cb theCallback, bool isHidden)
{
	// check for duplicates in construction
	new CommandRegistration(name, syntax, description, theCallback, isHidden);
}

//-----------------------------------------------------------------------------------------------
bool CommandRun(const String & command)
{
	// construct a command
	Command* newCommand = new Command(command);

	// see if we have it
	if (CommandRegistration::CheckForCallBack(newCommand->GetName()))
	{
		// We found it, so lets get the callback that is stored
		command_cb commandToRun = CommandRegistration::GetCallBack(newCommand->GetName());

		// This calls the function..? so we pass the command
		commandToRun(*newCommand);

		delete newCommand;

		return true;
	}

	delete newCommand;

	return false;
}
