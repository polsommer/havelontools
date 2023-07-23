// ======================================================================
//
// ClientMain.cpp
// copyright 1998 Bootprint Entertainment
// copyright 2001 Sony Online Entertainment
//
// ======================================================================

#include "FirstSwgHeadlessClient.h"
#include "ClientMain.h"

#include "clientAnimation/SetupClientAnimation.h"
#include "clientAudio/Audio.h"
#include "clientAudio/SetupClientAudio.h"
#include "clientBugReporting/SetupClientBugReporting.h"
#include "clientDirectInput/DirectInput.h"
#include "clientDirectInput/SetupClientDirectInput.h"
#include "clientGame/Game.h"
#include "clientGame/SetupClientGame.h"
#include "clientGraphics/ConfigClientGraphics.h"
#include "clientGraphics/Graphics.h"
#include "clientGraphics/ScreenShotHelper.h"
#include "clientGraphics/ShaderTemplate.h"
#include "clientGraphics/SetupClientGraphics.h"
#include "clientGraphics/RenderWorld.h"
#include "clientObject/SetupClientObject.h"
#include "clientParticle/SetupClientParticle.h"
#include "clientSkeletalAnimation/SetupClientSkeletalAnimation.h"
#include "clientTerrain/SetupClientTerrain.h"
#include "clientTextureRenderer/SetupClientTextureRenderer.h"
#include "clientUserInterface/CuiChatHistory.h"
#include "clientUserInterface/CuiManager.h"
#include "clientUserInterface/CuiSettings.h"
#include "clientUserInterface/CuiWorkspace.h"
#include "clientGraphics/IndexedTriangleListAppearance.h"
#include "sharedCompression/SetupSharedCompression.h"
#include "sharedDebug/DataLint.h"
#include "sharedDebug/InstallTimer.h"
#include "sharedDebug/SetupSharedDebug.h"
#include "sharedFile/SetupSharedFile.h"
#include "sharedFile/TreeFile.h"
#include "sharedFoundation/ApplicationVersion.h"
#include "sharedFoundation/Binary.h"
#include "sharedFoundation/ConfigFile.h"
#include "sharedFoundation/CrashReportInformation.h"
#include "sharedFoundation/ExitChain.h"
#include "sharedFoundation/Os.h"
#include "sharedFoundation/Production.h"
#include "sharedFoundation/SetupSharedFoundation.h"
#include "sharedGame/SetupSharedGame.h"
#include "sharedImage/SetupSharedImage.h"
#include "sharedIoWin/SetupSharedIoWin.h"
#include "sharedLog/SetupSharedLog.h"
#include "sharedLog/LogManager.h"
#include "sharedMath/SetupSharedMath.h"
#include "sharedMath/VectorArgb.h"
#include "sharedMemoryManager/MemoryManager.h"
#include "sharedNetwork/SetupSharedNetwork.h"
#include "sharedNetworkMessages/SetupSharedNetworkMessages.h"
#include "sharedObject/CellProperty.h"
#include "sharedObject/Object.h"
#include "sharedObject/ObjectTemplate.h"
#include "sharedObject/SetupSharedObject.h"
#include "sharedPathfinding/SetupSharedPathfinding.h"
#include "sharedRandom/SetupSharedRandom.h"
#include "sharedRegex/SetupSharedRegex.h"
#include "sharedTerrain/SetupSharedTerrain.h"
#include "sharedTerrain/TerrainAppearance.h"
#include "sharedThread/SetupSharedThread.h"
#include "sharedUtility/CurrentUserOptionManager.h"
#include "sharedUtility/LocalMachineOptionManager.h"
#include "sharedUtility/SetupSharedUtility.h"
#include "sharedXml/SetupSharedXml.h"
#include "swgClientUserInterface/SetupSwgClientUserInterface.h"
#include "swgClientUserInterface/SwgCuiChatWindow.h"
#include "swgClientUserInterface/SwgCuiManager.h"
#include "swgSharedNetworkMessages/SetupSwgSharedNetworkMessages.h"

#include "Resource.h"

#include "sharedGame/PlatformFeatureBits.h"

#include <dinput.h>
#include <string>
#include <ctime>
#include <cstdio>

namespace ClientMainNamespace
{
	void installConfigFileOverride ()
	{
		AbstractFile * const abstractFile = TreeFile::open ("misc/override.cfg", AbstractFile::PriorityData, true);
		if (abstractFile)
		{
			int const length = abstractFile->length ();
			byte * const data = abstractFile->readEntireFileAndClose ();
			IGNORE_RETURN (ConfigFile::loadFromBuffer (reinterpret_cast<char const *> (data), length));
			delete [] data;
			delete abstractFile;
		}
	}
}

using namespace ClientMainNamespace;

// ======================================================================
// Entry point for the application
//
// Return Value:
//
//   Result code to return to the operating system
//
// Remarks:
//
//   This routine should set up the engine, invoke the main game loop,
//   and then tear down the engine.

int ClientMain(
	HINSTANCE hInstance,      // handle to current instance 
	HINSTANCE hPrevInstance,  // handle to previous instance 
	LPSTR     lpCmdLine,      // pointer to command line 
	int       nCmdShow        // show state of window 
	)
{
	UNREF(hPrevInstance);
	UNREF(nCmdShow);


	//-- thread
	SetupSharedThread::install();

	//-- debug
	SetupSharedDebug::install(4096);

	InstallTimer rootInstallTimer("root");

	const char * configFileName = "client.cfg";
	{
		// get the name of the executable
		char programName[Os::MAX_PATH_LENGTH];
		DWORD result = GetModuleFileName(NULL, programName, sizeof(programName));
		FATAL(result == 0, ("GetModuleFileName failed"));

		// lower case it all
		for (char *s = programName; *s; ++s)
			*s = static_cast<char>(tolower(*s));

		if (strstr(programName, "testswgclient") != 0)
			configFileName = "test.cfg";
	}

	//-- foundation
	SetupSharedFoundation::Data data(SetupSharedFoundation::Data::D_game);
	data.windowName                = "SwgClient";
	data.windowNormalIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	data.windowSmallIcon           = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	data.hInstance                 = hInstance;
	data.commandLine               = lpCmdLine;
	data.configFile                = configFileName;
	data.clockUsesSleep            = true;
	data.frameRateLimit            = 30.f;
#if PRODUCTION
	data.demoMode                  = true;
#endif
	if (ApplicationVersion::isPublishBuild() || ApplicationVersion::isBootlegBuild())
		data.writeMiniDumps          = true;
	SetupSharedFoundation::install (data);

	REPORT_LOG(true, ("ClientMain: Command Line = \"%s\"\n", lpCmdLine));
	REPORT_LOG (true, ("ClientMain: Memory size = %i MB\n", MemoryManager::getLimit()));

	// check for any config file entries
	if (ConfigFile::isEmpty())
		FATAL(true, ("Config file not specified"));

	InstallTimer::checkConfigFile();

	SetLastError(0);
	HANDLE semaphore = CreateSemaphore(NULL, 0, 1, "SwgClientInstanceRunning");
	if (GetLastError() == ERROR_ALREADY_EXISTS && !ConfigFile::getKeyBool("SwgClient", "allowMultipleInstances", PRODUCTION ? false : true))
	{
		MessageBox(NULL, "Another instance of this application is already running.  Application will now close.", NULL, MB_OK | MB_ICONSTOP);
	}
	else
	{
		{
			uint32 gameFeatures = ConfigFile::getKeyInt("Station",  "gameFeatures", 0) & ~ConfigFile::getKeyInt("ClientGame", "gameBitsToClear", 0);
			// hack to set retail if beta or preorder
			if (ConfigFile::getKeyBool("ClientGame",  "setJtlRetailIfBetaIsSet", 0))
			{
				if (gameFeatures & (ClientGameFeature::SpaceExpansionBeta | ClientGameFeature::SpaceExpansionPreOrder))
					gameFeatures |= ClientGameFeature::SpaceExpansionRetail;
			}

			//-- set ep3 retail if beta or preorder
			if (gameFeatures & (ClientGameFeature::Episode3Beta | ClientGameFeature::Episode3PreorderDownload))
				gameFeatures |= ClientGameFeature::Episode3ExpansionRetail;
			
			Game::setGameFeatureBits(gameFeatures);
			Game::setSubscriptionFeatureBits(ConfigFile::getKeyInt("Station",  "subscriptionFeatures", 0));
		}
		
		{
			SetupSharedCompression::Data data;
			data.numberOfThreadsAccessingZlib = 3;
			SetupSharedCompression::install(data);
		}

		//-- Regular expression support.
		SetupSharedRegex::install();

		//-- file
		{
			// figure out what skus we need to support in the tree file system
			uint32 skuBits = 0;
			if ((Game::getGameFeatureBits() & ClientGameFeature::Base) != 0)
				skuBits |= BINARY1(0001);
			if ((Game::getGameFeatureBits() & ClientGameFeature::SpaceExpansionRetail) != 0)
				skuBits |= BINARY1(0010);
			if ((Game::getGameFeatureBits() & ClientGameFeature::Episode3ExpansionRetail) != 0)
				skuBits |= BINARY1(0100);
			SetupSharedFile::install(true, skuBits);
		}

		installConfigFileOverride();

		//-- math
		SetupSharedMath::install();

		//-- utility
		SetupSharedUtility::Data setupUtilityData;
		SetupSharedUtility::setupGameData (setupUtilityData);
		setupUtilityData.m_allowFileCaching = true;
		SetupSharedUtility::install (setupUtilityData);

		//-- random
		SetupSharedRandom::install(static_cast<uint32>(time(NULL)));

		SetupSharedLog::install("SwgClient");

		//-- image
		SetupSharedImage::Data setupImageData;
		SetupSharedImage::setupDefaultData (setupImageData);
		SetupSharedImage::install (setupImageData);

		//-- network
		SetupSharedNetwork::SetupData  networkSetupData;
		SetupSharedNetwork::getDefaultClientSetupData(networkSetupData);
		SetupSharedNetwork::install(networkSetupData);

		SetupSharedNetworkMessages::install();
		SetupSwgSharedNetworkMessages::install();

		//-- object
		SetupSharedObject::Data setupObjectData;
		SetupSharedObject::setupDefaultGameData (setupObjectData);
		setupObjectData.useTimedAppearanceTemplates = true;
		// we want the SlotIdManager initialized, and we need the associated hardpoint names loaded.
		SetupSharedObject::addSlotIdManagerData(setupObjectData, true);
		// we want CustomizationData support on the client.
		SetupSharedObject::addCustomizationSupportData(setupObjectData);
		SetupSharedObject::install (setupObjectData);

		//-- game
		SetupSharedGame::Data setupSharedGameData;

		setupSharedGameData.setUseGameScheduler (true);
		setupSharedGameData.setUseMountValidScaleRangeTable (true);
		setupSharedGameData.m_debugBadStringsFunc = CuiManager::debugBadStringIdsFunc;
		SetupSharedGame::install (setupSharedGameData);

		//-- terrain
		SetupSharedTerrain::Data setupSharedTerrainData;
		SetupSharedTerrain::setupGameData (setupSharedTerrainData);
		SetupSharedTerrain::install (setupSharedTerrainData);

		//-- SharedXml
		SetupSharedXml::install();

		//-- pathfinding
		SetupSharedPathfinding::install();
		
		//-- setup client

		//-- audio
		SetupClientAudio::install ();

		//-- graphics
		SetupClientGraphics::Data setupGraphicsData;
		setupGraphicsData.screenWidth  = 1024;
		setupGraphicsData.screenHeight = 768;
		setupGraphicsData.alphaBufferBitDepth = 0;
		SetupClientGraphics::setupDefaultGameData (setupGraphicsData);

		ConfigClientGraphics::setHeadless();

		if (SetupClientGraphics::install (setupGraphicsData))
		{

			//-- directinput
			SetupClientDirectInput::install(hInstance, Os::getWindow(), DIK_LCONTROL, Graphics::isWindowed);
			DirectInput::setScreenShotFunction(ScreenShotHelper::screenShot);
			DirectInput::setToggleWindowedModeFunction(Graphics::toggleWindowedMode);
			DirectInput::setRequestDebugMenuFunction(Os::requestPopupDebugMenu);

			Os::setLostFocusHookFunction(DirectInput::unacquireAllDevices);

			//-- object
			SetupClientObject::Data setupClientObjectData;
			SetupClientObject::setupGameData (setupClientObjectData);
			SetupClientObject::install (setupClientObjectData);

			//-- animation and skeletal animation
			SetupClientAnimation::install ();

			SetupClientSkeletalAnimation::Data  saData;
			SetupClientSkeletalAnimation::setupGameData(saData);
			SetupClientSkeletalAnimation::install (saData);

			//-- texture renderer
			SetupClientTextureRenderer::install ();

			//-- terrain
			SetupClientTerrain::install ();

			//-- particle system
			SetupClientParticle::install ();

			//-- game
			SetupClientGame::Data data;
			SetupClientGame::setupGameData (data);
			SetupClientGame::install (data);

			CuiManager::setImplementationInstallFunctions (SwgCuiManager::install, SwgCuiManager::remove, SwgCuiManager::update);
			CuiManager::setImplementationTestFunction     (SwgCuiManager::test);

			SetupClientBugReporting::install();

			//-- iowin
			SetupSharedIoWin::install();

			//-- run game
			rootInstallTimer.manualExit();
			SetupSharedFoundation::callbackWithExceptionHandling(Game::run);

			//-- save options
			// @todo: write a flexible options load/save system, both of ours suck
			CuiWorkspace * workspace = CuiWorkspace::getGameWorkspace();
			if (workspace != NULL)
			{
				workspace->saveAllSettings();
				SwgCuiChatWindow * chatWindow = safe_cast<SwgCuiChatWindow *>(workspace->findMediatorByType(typeid(SwgCuiChatWindow)));
				if (chatWindow != NULL)
					chatWindow->saveSettings(true);
			}
			CuiSettings::save();
			CuiChatHistory::save();
			CurrentUserOptionManager::save ();
			LocalMachineOptionManager::save ();
		}
	}

	Audio::setEnabled( false );

	SetupSharedFoundation::remove ();
	SetupSharedThread::remove ();

	if (semaphore)
		CloseHandle(semaphore);

	return 0;
}

// ======================================================================
