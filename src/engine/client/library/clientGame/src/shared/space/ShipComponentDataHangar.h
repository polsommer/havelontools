//======================================================================
//
// ShipComponentDataHangar.h
// copyright (c) 2004 Sony Online Entertainment
//
//======================================================================

#ifndef INCLUDED_ShipComponentDataHangar_H
#define INCLUDED_ShipComponentDataHangar_H

//======================================================================

#include "clientGame/ClientShipComponentData.h"

//----------------------------------------------------------------------

class ShipComponentDataHangar : public ClientShipComponentData
{
public:

	explicit ShipComponentDataHangar (ShipComponentDescriptor const & shipComponentDescriptor);
	virtual ~ShipComponentDataHangar ();

	virtual bool readDataFromShip      (int chassisSlot, ShipObject const & ship);

	virtual void printDebugString      (Unicode::String & result, Unicode::String const & padding) const;

	virtual void getAttributes         (stdvector<std::pair<std::string, Unicode::String> >::fwd & data) const;

private:
	ShipComponentDataHangar (const ShipComponentDataHangar & rhs);
	ShipComponentDataHangar & operator= (const ShipComponentDataHangar & rhs);

public:

	//-- these members are public because this instances of this class are short-lived temporary objects,
	//-- to simplify the conversion from the ShipObject & shipt representation of components.
};

//======================================================================

#endif
