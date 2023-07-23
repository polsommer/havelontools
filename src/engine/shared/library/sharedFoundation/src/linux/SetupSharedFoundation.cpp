// ======================================================================
//
// SetupSharedFoundation.cpp
// copyright 1998 Bootprint Entertainment
// copyright 2001 - 2002 Sony Online Entertainment
//
// ======================================================================

#include "sharedFoundation/FirstSharedFoundation.h"
#include "sharedFoundation/SetupSharedFoundation.h"

#include "sharedDebug/ConfigSharedDebugLinux.h"
#include "sharedDebug/DebugMonitor.h"
#include "sharedDebug/Profiler.h"
#include "sharedFoundation/Clock.h"
#include "sharedFoundation/CommandLine.h"
#include "sharedFoundation/ConfigFile.h"
#include "sharedFoundation/ConfigSharedFoundation.h"
#include "sharedFoundation/CrcLowerString.h"
#include "sharedFoundation/ExitChain.h"
#include "sharedFoundation/Os.h"
#include "sharedFoundation/PerThreadData.h"
#include "sharedFoundation/StaticCallbackEntry.h"

#include <ctime>
#include <math.h>

// ======================================================================
/**
 * Install the engine.
 *
 * The settings in the Data structure will determine which subsystems
 * get initialized.
 */

void SetupSharedFoundation::install(const Data &data)
{
	// and get the command line stuff in quick so we can make decisions based on the command line settings
	CommandLine::install();
	ConfigFile::install();

	if (data.lpCmdLine)
		CommandLine::absorbString(data.lpCmdLine);
	if (data.argc)
		CommandLine::absorbStrings(const_cast<const char**>(data.argv+1), data.argc-1);

#if 0
	//currently there's a problem that we cannot override the defaults here.
	if (data.configFile)
		IGNORE_RETURN(ConfigFile::loadFile(data.configFile));
#endif

	// get the post command-line text for the ConfigFile (key-value pairs)
	const char *configString = CommandLine::getPostCommandLineString();
	if (configString)
		ConfigFile::loadFromCommandLine(configString);

	//@todo there is a lot of stuff in win32 setup not here...like exitchain
	Profiler::registerDebugFlags();
#if _DEBUG
	MemoryManager::registerDebugFlags();
#endif//_DEBUG

	// Setup Linux DebugMonitor support.
	// @todo fix this dependency: DebugMonitor really should be moved into Foundation the way things currently are.  TRF is following the existing win32 setup.
#ifdef _DEBUG
	ConfigSharedDebugLinux::install();
	DebugMonitor::install();
#endif

	// setup the engine configuration
	ConfigSharedFoundation::Defaults defaults;
	defaults.frameRateLimit = data.frameRateLimit;
	ConfigSharedFoundation::install(defaults);
	SetWarningStrictFatal(ConfigFile::getKeyBool("SharedDebug", "strict", false));
	Report::install();
	Clock::install(data.runInBackground, false);

	PersistentCrcString::install();
	CrcLowerString::install();
	StaticCallbackEntry::install();
}

// ----------------------------------------------------------------------
// Call a function with appropriate exception handling (not)
//
// Remarks:
//
//   this is stubbed and exception handling is ignored currently

void SetupSharedFoundation::callbackWithExceptionHandling(
	void (*callback)(void)   // Routine to call with exception handling
	)
{
	if (ConfigSharedFoundation::getNoExceptionHandling())
	{
		callback();
	}
	else
	{
#if 0
		try
		{
			callback();
		}
		catch (__exception * mathException)
		{
			FATAL(true, ("Math Exception: %s\n", mathException->name));
		}
		catch (const char* message)
		{
			FATAL(true, ("Character Exception: %s\n", message));
		}
		catch(std::exception & m)
		{
			const char * c = m.what();
			FATAL(true, ("Std::exception: %s\n", c));
		}
		catch(...)
		{
			FATAL(true, ("Unknown exception\n"));
		}
#else
		callback();
#endif
	}
}

// ----------------------------------------------------------------------
/**
 * Uninstall the engine.
 *
 * This routine will properly uninstall the engine componenets that were
 * installed by SetupSharedFoundation::install().
 */

void SetupSharedFoundation::remove(void)
{
	ExitChain::quit();

	if (GetNumberOfWarnings())
		REPORT(true, Report::RF_print | Report::RF_log | Report::RF_dialog, ("%d warnings logged", GetNumberOfWarnings()));

	// this routine can't be on the exit chain because the exit chain depends upon the PerThreadData class being around
	//PerThreadData::remove();
}

// ----------------------------------------------------------------------

SetupSharedFoundation::Data::Data(Defaults defaults)
{
	Zero(*this);

	switch (defaults)
	{
		case D_game:
			runInBackground                 = true;

			lpCmdLine                       = NULL;
			argc                            = 0;
			argv                            = NULL;

			configFile                      = NULL;

			frameRateLimit                  = CONST_REAL(0);
			break;

		case D_console:
			runInBackground                 = true;

			lpCmdLine                       = NULL;
			argc                            = 0;
			argv                            = NULL;

			configFile                      = NULL;

			frameRateLimit                  = CONST_REAL(0);
			break;

		default:
			DEBUG_FATAL(true, ("unknown case"));
	}
}

// ======================================================================
