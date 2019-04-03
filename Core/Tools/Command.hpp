#pragma once
#include "Engine/Core/General/EngineCommon.hpp"
#include <map>

//====================================================================================
// Forward Declare
//====================================================================================
class Command;

//====================================================================================
// Type Defs + Defines
//====================================================================================
typedef void(*command_cb)(Command &cmd);


//====================================================================================
// ENUMS
//====================================================================================


//====================================================================================
// Structs
//====================================================================================


//====================================================================================
// Classes
//====================================================================================
class Command
{
public:
	Command(const String& commandString);
	~Command();


public:
	String GetNextString();
	String GetCommandParameters();
	String GetName() { return m_commandName; }

public:
	String	m_fullCommand;
	String	m_commandName;
	Strings m_commandArguements;
	
	uint	m_commandIndex = 0U;
};


//===============================================================================================
class CommandRegistration
{
public:
	CommandRegistration(const String& name, const String& syntax, const String& description, command_cb theCallback, bool isHidden = false);
	~CommandRegistration();

public:
	static void DeleteAllRegisteredCommands();

public:
	static command_cb	GetCallBack(const String& callbackName);
	static bool			CheckForCallBack(const String& callbackName);
	static Strings		GetAllTheCommandsNames();
	static Strings		GetAllCommandsHelpText();
	static Strings		GetAllCommandsAndTheirHelp();

private:
	String				m_name;
	String				m_syntax;
	String				m_description;
	command_cb			m_callback;
	bool				m_isHidden;

private:
	static std::map<String, CommandRegistration*> s_callbacks;
};

//====================================================================================
// Standalone C Functions
//====================================================================================
void CommandRegister(const String& name, const String& syntax, const String& description, command_cb theCallback, bool isHidden = false);
bool CommandRun(const String& command);

//====================================================================================
// Externs
//====================================================================================


//====================================================================================
// Written by Zachary Bracken : [4/3/2019]
//====================================================================================