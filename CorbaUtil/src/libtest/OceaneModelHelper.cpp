#include "OceaneModelHelper.h"
#include <TkUtil/Exception.h>


#include <assert.h>
#include <string.h>
#include <string>
#include <strstream>

using namespace Oceane;
using namespace TkUtil;
using namespace std;


static const CORBA::ULongLong	nodeSurfacesSize	= 
										sizeof (ModelICorba::NodeSurfaces);
static const CORBA::ULongLong nodeSurfacesCount		= 8 * nodeSurfacesSize;
static const CORBA::ULongLong	surfacesPerField	= 
										8 * sizeof (CORBA::ULongLong);


#define CHECK_SURFACE_INDEX(method, index)                                 \
	{                                                                      \
		if ((CORBA::ULongLong)index >= nodeSurfacesCount)                  \
		{                                                                  \
			ostrstream	strIndex, strCount;                                \
			strIndex << index << ends;                                     \
			strCount << nodeSurfacesCount << ends;                         \
			string	message ("Index de surface invalide dans ");           \
			message	+= #method + string (". index = ")+ strIndex.str ( );  \
			message	+= string (" Nombre de surfaces possibles : ");        \
			message += strCount.str ( ) + string (".");                    \
			throw Exception (message);                                     \
		}	/* if (index > nodeSurfacesCount) */                           \
	}


OceaneModelHelper::OceaneModelHelper ( )
{
	assert (0 && "OceaneModelHelper::OceaneModelHelper is not allowed.");
}	// OceaneModelHelper::OceaneModelHelper


OceaneModelHelper::OceaneModelHelper (const OceaneModelHelper&)
{
	assert (0 && "OceaneModelHelper::OceaneModelHelper is not allowed.");
}	// OceaneModelHelper::OceaneModelHelper


OceaneModelHelper& OceaneModelHelper::operator = (const OceaneModelHelper&)
{
	assert (0 && "OceaneModelHelper::operator = is not allowed.");
	return *this;
}	// OceaneModelHelper::operator =


OceaneModelHelper::~OceaneModelHelper ( )
{
	assert (0 && "OceaneModelHelper::~OceaneModelHelper is not allowed.");
}	// OceaneModelHelper::~OceaneModelHelper


void OceaneModelHelper::initializeNodeSurfaces (
									ModelICorba::NodeSurfaces& nodeSurfaces)
{
	memset (&nodeSurfaces, 0, nodeSurfacesSize);
}	// OceaneModelHelper::initializeNodeSurfaces


void OceaneModelHelper::addSurfaceToNode (
					ModelICorba::NodeSurfaces& nodeSurfaces, size_t surface)
{
	CHECK_SURFACE_INDEX ("OceaneModelHelper::addSurfaceToNode", surface)
	CORBA::ULongLong*	ptr		= &nodeSurfaces.s_64;
	CORBA::ULongLong	shift	= 
		((CORBA::ULongLong)1) << (((CORBA::ULongLong)surface)%surfacesPerField);
	ptr		+= (surface / surfacesPerField);
	*ptr	|= shift;
}	// OceaneModelHelper::addSurfaceToNode


bool OceaneModelHelper::nodeBelongToSurface (
				const ModelICorba::NodeSurfaces& nodeSurfaces, size_t surface)
{
	CHECK_SURFACE_INDEX ("OceaneModelHelper::nodeBelongToSurface", surface)
	const CORBA::ULongLong*	ptr		= &nodeSurfaces.s_64;
	CORBA::ULongLong		shift	= 
		((CORBA::ULongLong)1) << (((CORBA::ULongLong)surface)%surfacesPerField);
	ptr		+= (surface / surfacesPerField);
	return (0 == (*ptr & shift) ? false : true);
}	// OceaneModelHelper::nodeBelongToSurface


