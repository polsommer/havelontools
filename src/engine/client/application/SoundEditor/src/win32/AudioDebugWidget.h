// ============================================================================
//
// AudioDebugWidget.h
// Copyright Sony Online Entertainment
//
// ============================================================================

#ifndef AUDIODEBUGWIDGET_H
#define AUDIODEBUGWIDGET_H

#include "BaseAudioDebugWidget.h"

//-----------------------------------------------------------------------------
class AudioDebugWidget : public BaseAudioDebugWidget
{
	Q_OBJECT

public:

	AudioDebugWidget(QWidget *parent, char const *name, WFlags flags = 0);

public slots:

	void timerTimout();

signals:

private:

	void setText(int const row, int const column, QString const &text);
};

// ============================================================================

#endif // AUDIODEBUGWIDGET_H
