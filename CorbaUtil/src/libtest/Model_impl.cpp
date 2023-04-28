#include "Model_impl.h"
#include "OceaneModelHelper.h"
#include <TkUtil/InformationLog.h>
#include <TkUtil/ErrorLog.h>
#include <TkUtil/TraceLog.h>
#include <TkUtil/UTF8String.h>
#include "CorbaUtil/ORBFacilities.h"

#include <math.h>
#include <iostream>

using namespace std;

USING_UTIL
using namespace CorbaFacilities;

static const Charset	charset ("àéèùô");


namespace Oceane
{

Model_impl::Model_impl (const string& fileName)
	: POA_Oceane::ModelICorba ( ), PortableServer::RefCountServantBase ( ),
	  _fileName (fileName)
{
	UTF8String	msg (charset);
	msg << "Model_impl::Model_impl : " << _fileName << " chargé.";
	log (InformationLog (msg));
}	// Model_impl::Model_impl


Model_impl::Model_impl (const Model_impl&)
	: POA_Oceane::ModelICorba ( ), PortableServer::RefCountServantBase ( )
{
}	// Model_impl::Model_impl


Model_impl& Model_impl::operator = (const Model_impl&)
{
	return *this;
}	// Model_impl::operator =


Model_impl::~Model_impl ( )
{
	UTF8String	msg (charset);
	msg << "Model_impl::~Model_impl : " << _fileName << " fermé.";
	log (InformationLog (msg));
}	// Model_impl::~Model_impl


void Model_impl::projectNodes (
	const Oceane::ModelICorba::SurfaceList& surfaces, 
	const Oceane::ModelICorba::NodeSurfacesList& nodeSurfaces,
	NodeList& nodes, Oceane::ModelICorba::Distances& dists)
{
	UTF8String	msg1 (charset);
	msg1 << "Model_impl::projectNodes : projection de "
	     << (unsigned long)nodes.length ( ) << " noeuds sur "
	     << (unsigned long)surfaces.length ( ) << " surfaces ...";
	log (InformationLog (msg1));

#ifndef NO_PROJECTION
	if (nodeSurfaces.length ( ) != nodes.length ( ))
	{
		UTF8String	error (charset);
		error << "Erreur en Model_impl::projectNodes : listes nodeSurfaces et "
		      << "nodes de tailles differentes.";
		Oceane::OceaneExceptionICorba	exc;
		log (ErrorLog (error));
		exc.what	= error.iso ( ).c_str ( );
		throw exc;
	}	// if (nodeSurfaces.length ( ) != nodes.length ( ))
	UTF8String	surfacesMsg (charset);
	surfacesMsg << "Model_impl::projectNodes. Surfaces :";
	size_t	i	= 0, s	= 0;
	for (s = 0; s < surfaces.length ( ); s++)
		surfacesMsg << " " << surfaces [s];
	TRACE_LOG_1 (trace1, surfacesMsg)
	log (trace1);
	for (s = 0; s < surfaces.length ( ); s++)
	{
		UTF8String	nodesMsg (charset);
		nodesMsg << "Noeuds de la surface " << surfaces [s] << " :" << "\n";
		for (i = 0; i < nodes.length ( ); i++)
		{
			if (true == OceaneModelHelper::nodeBelongToSurface (
							nodeSurfaces [i], s))
				nodesMsg << (unsigned long)i << " ";
		}	// for (i = 0; i < nodes.length ( ); i++)
		TRACE_LOG_2 (trace2, nodesMsg)
		log (trace2);
	}	// for (s = 0; s < surfaces.length ( ); s++)
	const double	var	= 0.1;
	for (i = 0; i < nodes.length ( ); i++)
	{
		Node	node	= nodes [i];
		double	max		= fabs (node.x) >= fabs (node.y) ?
						  node.x : node.y;
		max	= fabs (max) >= fabs (node.z) ?  max : node.z;
		double	x	= node.x, y	= node.y, z	= node.z;
		if (fabs (max) == fabs (x))
			x	= max < 0. ? max - var : max + var;
		if (fabs (max) == fabs (y))
			y	= max < 0. ? max - var : max + var;
		if (fabs (max) == fabs (z))
			z	= max < 0. ? max - var : max + var;
		double	dx	= node.x - x;
		double	dy	= node.y - y;
		double	dz	= node.z - z;
		double	distance	= dx * dx + dy * dy + dz * dz;
		dists [i]	= sqrt (distance);
		nodes [i].x	= x;
		nodes [i].y	= y;
		nodes [i].z	= z;
	}	// for (size_t i = 0; i < size; i++)

#endif	// NO_PROJECTION

	log (InformationLog (UTF8String ("Model_impl::projectNodes : projection effectuée.", charset)));
}	// Model_impl::projectNodes


void Model_impl::log (const Log& log)
{
	if (0 != ORBFacilities::getLogDispatcher ( ))
		(*ORBFacilities::getLogDispatcher ( )) << log;
}	// Model_impl::log

}	// namespace Oceane

