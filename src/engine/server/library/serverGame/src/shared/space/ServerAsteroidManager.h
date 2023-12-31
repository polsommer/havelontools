// ============================================================================
//
// ServerAsteroidManager.h
// Copyright 2004 Sony Online Entertainment, Inc.
//
// ============================================================================

#ifndef INCLUDED_ServerAsteroidManager_H
#define INCLUDED_ServerAsteroidManager_H

// ============================================================================

#include "sharedGame/AsteroidGenerationManager.h"

class ConstCharCrcString;
class NetworkId;
class Sphere;
class Vector;

// ============================================================================

class ServerAsteroidManager
{
public:
	typedef int FieldHandle;

	static void install();
	static FieldHandle generateField(AsteroidGenerationManager::AsteroidFieldData const & fieldData);
	static FieldHandle getBadHandle();
	static void setupStaticFields(std::string const & sceneName);
	static void update(float elapsedTime);
	static void listenforServerAsteroidDebugData(NetworkId const & player);
	static void endListenforServerAsteroidDebugData(NetworkId const & player);
	static float getExtentRadius(uint32 sharedTemplateCrc);

private:
	static void remove();
	static void getServerAsteroidData(stdvector<Sphere>::fwd & /*OUT*/ spheres);
	static void sendServerAsteroidDataToPlayer(NetworkId const & player, stdvector<Sphere>::fwd const & spheres);

private:
	static FieldHandle const BAD_HANDLE;
	static FieldHandle s_nextHandle;
	static stdset<NetworkId>::fwd ms_playersListeningForServerAsteroidDebugData;
	static stdvector<NetworkId>::fwd ms_asteroids;

private:
	//disabled
	ServerAsteroidManager();
	~ServerAsteroidManager();
	ServerAsteroidManager(ServerAsteroidManager const &);
	ServerAsteroidManager &operator =(ServerAsteroidManager const &);
};

// ============================================================================

#endif // INCLUDED_ServerAsteroidManager_H
