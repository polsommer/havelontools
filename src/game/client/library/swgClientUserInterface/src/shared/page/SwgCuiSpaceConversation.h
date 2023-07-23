//======================================================================
//
// SwgCuiSpaceConversation.h
// copyright(c) 2004 Sony Online Entertainment
//
//======================================================================

#ifndef INCLUDED_SwgCuiSpaceConversation_H
#define INCLUDED_SwgCuiSpaceConversation_H

//======================================================================

#include "swgClientUserInterface/SwgCuiLockableMediator.h"

//----------------------------------------------------------------------

class CuiWidget3dObjectListViewer;
class NetworkId;
class Object;
class UIPage;
class UIPopupMenu;
class UIText;
class UIWidget;

//----------------------------------------------------------------------

namespace MessageDispatch
{
	class Callback;
}

//----------------------------------------------------------------------

class SwgCuiSpaceConversation :
public SwgCuiLockableMediator
{
public:
	typedef std::vector<Unicode::String> StringVector;
	
	static void install();
	static void remove();

	explicit SwgCuiSpaceConversation(UIPage & page);
	virtual ~SwgCuiSpaceConversation();
	
	void update(float deltaTimeSecs);
	bool setTauntInfo(NetworkId const & taunter, Unicode::String const & text, uint32 appearanceOverloadSharedTemplateCrc, std::string const & soundEffect, float duration);
	
protected:
	virtual void performActivate();
	virtual void performDeactivate();
	
private:
	void onTargetChanged(bool const & b);
	void onResponsesChanged(bool const & b);
	void onConversationEnded(bool const & b);
	void OnPopupMenuSelection(UIWidget * context);
	bool setViewerTarget(NetworkId const & target, uint32 appearanceOverrideTemplateCrc);
	void updateResponses(StringVector const & strings);
	Unicode::String const & getText() const;
	void setText(Unicode::String const & text);
	void clearResponses();
	
private:
	//disabled
	SwgCuiSpaceConversation();
	SwgCuiSpaceConversation(const SwgCuiSpaceConversation &);
	SwgCuiSpaceConversation & operator=(const SwgCuiSpaceConversation &);
	
private:
	MessageDispatch::Callback * m_callback;
	CuiWidget3dObjectListViewer * m_targetViewer;
	UIText * m_text;
	UIPopupMenu * m_popup;
	UIPage * m_viewerPage;
	UIPage * m_textPage;
	StringVector m_currentResponses;
	Object * m_createdViewerObject;
	uint32 m_currentNodeId;
};

//======================================================================

#endif
