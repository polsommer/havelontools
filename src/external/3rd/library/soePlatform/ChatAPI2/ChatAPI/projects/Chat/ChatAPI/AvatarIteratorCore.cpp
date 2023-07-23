#include "AvatarIteratorCore.h"
#include "ChatAvatar.h"

using namespace std;

namespace ChatSystem 
{

// AVATAR ITERATOR CORE

AvatarIteratorCore::AvatarIteratorCore()
: m_map(NULL)
{
}

AvatarIteratorCore::AvatarIteratorCore(std::map<unsigned, ChatAvatar *> *mapIn, std::map<unsigned, ChatAvatar *>::iterator iter)
: m_map(mapIn),
  m_mapIter(iter)
{
}

AvatarIteratorCore::~AvatarIteratorCore()
{
}

AvatarIteratorCore &AvatarIteratorCore::operator=(const AvatarIteratorCore& rhs)
{
	m_map = rhs.m_map;
	m_mapIter = rhs.m_mapIter;

	return (*this);
}

ChatAvatar *AvatarIteratorCore::getCurAvatar()
{
	ChatAvatar *returnVal = NULL;
	
	if (!outOfBounds())
	{
		returnVal = (*m_mapIter).second;
	}

	return returnVal;
}

bool AvatarIteratorCore::increment()
{
	bool returnVal = false;

	m_mapIter++;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool AvatarIteratorCore::decrement()
{
	bool returnVal = false;

	m_mapIter--;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool AvatarIteratorCore::outOfBounds()
{
	bool returnVal = true;

	if (m_map)
	{
		if (m_mapIter != m_map->end())
		{
			returnVal = false;
		}
	}

	return returnVal;
}

// MODERATOR ITERATOR CORE

ModeratorIteratorCore::ModeratorIteratorCore()
: m_set(NULL)
{
}

ModeratorIteratorCore::ModeratorIteratorCore(std::set<ChatAvatar *> *setIn, std::set<ChatAvatar *>::iterator iter)
: m_set(setIn),
  m_setIter(iter)
{
}

ModeratorIteratorCore::~ModeratorIteratorCore()
{
}

ModeratorIteratorCore &ModeratorIteratorCore::operator=(const ModeratorIteratorCore& rhs)
{
	m_set = rhs.m_set;
	m_setIter = rhs.m_setIter;

	return (*this);
}

ChatAvatar *ModeratorIteratorCore::getCurAvatar()
{
	ChatAvatar *returnVal = NULL;
	
	if (!outOfBounds())
	{
		returnVal = (*m_setIter);
	}

	return returnVal;
}

bool ModeratorIteratorCore::increment()
{
	bool returnVal = false;

	m_setIter++;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool ModeratorIteratorCore::decrement()
{
	bool returnVal = false;

	m_setIter--;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool ModeratorIteratorCore::outOfBounds()
{
	bool returnVal = true;

	if (m_set)
	{
		if (m_setIter != m_set->end())
		{
			returnVal = false;
		}
	}

	return returnVal;
}

// TEMPORARY MODERATOR ITERATOR CORE

TemporaryModeratorIteratorCore::TemporaryModeratorIteratorCore()
: m_set(NULL)
{
}

TemporaryModeratorIteratorCore::TemporaryModeratorIteratorCore(std::set<ChatAvatar *> *setIn, std::set<ChatAvatar *>::iterator iter)
: m_set(setIn),
  m_setIter(iter)
{
}

TemporaryModeratorIteratorCore::~TemporaryModeratorIteratorCore()
{
}

TemporaryModeratorIteratorCore &TemporaryModeratorIteratorCore::operator=(const TemporaryModeratorIteratorCore& rhs)
{
	m_set = rhs.m_set;
	m_setIter = rhs.m_setIter;

	return (*this);
}

ChatAvatar *TemporaryModeratorIteratorCore::getCurAvatar()
{
	ChatAvatar *returnVal = NULL;
	
	if (!outOfBounds())
	{
		returnVal = (*m_setIter);
	}

	return returnVal;
}

bool TemporaryModeratorIteratorCore::increment()
{
	bool returnVal = false;

	m_setIter++;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool TemporaryModeratorIteratorCore::decrement()
{
	bool returnVal = false;

	m_setIter--;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool TemporaryModeratorIteratorCore::outOfBounds()
{
	bool returnVal = true;

	if (m_set)
	{
		if (m_setIter != m_set->end())
		{
			returnVal = false;
		}
	}

	return returnVal;
}

// VOICE ITERATOR CORE

VoiceIteratorCore::VoiceIteratorCore()
: m_set(NULL)
{
}

VoiceIteratorCore::VoiceIteratorCore(std::set<ChatAvatar *> *setIn, std::set<ChatAvatar *>::iterator iter)
: m_set(setIn),
  m_setIter(iter)
{
}

VoiceIteratorCore::~VoiceIteratorCore()
{
}

VoiceIteratorCore &VoiceIteratorCore::operator=(const VoiceIteratorCore& rhs)
{
	m_set = rhs.m_set;
	m_setIter = rhs.m_setIter;

	return (*this);
}

ChatAvatar *VoiceIteratorCore::getCurAvatar()
{
	ChatAvatar *returnVal = NULL;
	
	if (!outOfBounds())
	{
		returnVal = (*m_setIter);
	}

	return returnVal;
}

bool VoiceIteratorCore::increment()
{
	bool returnVal = false;

	m_setIter++;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool VoiceIteratorCore::decrement()
{
	bool returnVal = false;

	m_setIter--;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool VoiceIteratorCore::outOfBounds()
{
	bool returnVal = true;

	if (m_set)
	{
		if (m_setIter != m_set->end())
		{
			returnVal = false;
		}
	}

	return returnVal;
}

// INVITE ITERATOR CORE

InviteIteratorCore::InviteIteratorCore()
: m_set(NULL)
{
}

InviteIteratorCore::InviteIteratorCore(std::set<ChatAvatar *> *setIn, std::set<ChatAvatar *>::iterator iter)
: m_set(setIn),
  m_setIter(iter)
{
}

InviteIteratorCore::~InviteIteratorCore()
{
}

InviteIteratorCore &InviteIteratorCore::operator=(const InviteIteratorCore& rhs)
{
	m_set = rhs.m_set;
	m_setIter = rhs.m_setIter;

	return (*this);
}

ChatAvatar *InviteIteratorCore::getCurAvatar()
{
	ChatAvatar *returnVal = NULL;
	
	if (!outOfBounds())
	{
		returnVal = (*m_setIter);
	}

	return returnVal;
}

bool InviteIteratorCore::increment()
{
	bool returnVal = false;

	m_setIter++;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool InviteIteratorCore::decrement()
{
	bool returnVal = false;

	m_setIter--;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool InviteIteratorCore::outOfBounds()
{
	bool returnVal = true;

	if (m_set)
	{
		if (m_setIter != m_set->end())
		{
			returnVal = false;
		}
	}

	return returnVal;
}

// BAN ITERATOR CORE

BanIteratorCore::BanIteratorCore()
: m_set(NULL)
{
}

BanIteratorCore::BanIteratorCore(std::set<ChatAvatar *> *setIn, std::set<ChatAvatar *>::iterator iter)
: m_set(setIn),
  m_setIter(iter)
{
}

BanIteratorCore::~BanIteratorCore()
{
}

BanIteratorCore &BanIteratorCore::operator=(const BanIteratorCore& rhs)
{
	m_set = rhs.m_set;
	m_setIter = rhs.m_setIter;

	return (*this);
}

ChatAvatar *BanIteratorCore::getCurAvatar()
{
	ChatAvatar *returnVal = NULL;
	
	if (!outOfBounds())
	{
		returnVal = (*m_setIter);
	}

	return returnVal;
}

bool BanIteratorCore::increment()
{
	bool returnVal = false;

	m_setIter++;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool BanIteratorCore::decrement()
{
	bool returnVal = false;

	m_setIter--;
	if (!outOfBounds())
	{
		returnVal = true;
	}

	return returnVal;
}

bool BanIteratorCore::outOfBounds()
{
	bool returnVal = true;

	if (m_set)
	{
		if (m_setIter != m_set->end())
		{
			returnVal = false;
		}
	}

	return returnVal;
}

};

