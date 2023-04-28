#include "TkUtil/DefaultLogFactory.h"
#include "TkUtil/FileLogOutputStream.h"
#include "TkUtil/OstreamLogOutputStream.h"
#include "TkUtil/File.h"
#include "TkUtil/Exception.h"
#include "TkUtil/MemoryError.h"

#include <memory>


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


DefaultLogFactory::DefaultLogFactory ( )
{
}	// DefaultLogFactory::DefaultLogFactory ( )


DefaultLogFactory::DefaultLogFactory (const DefaultLogFactory&)
{
}	// DefaultLogFactory::DefaultLogFactory (const DefaultLogFactory&)


DefaultLogFactory& DefaultLogFactory::operator = (const DefaultLogFactory&)
{
	return *this;
}	// DefaultLogFactory::operator =


DefaultLogFactory::~DefaultLogFactory ( )
{
}	// DefaultLogFactory::~DefaultLogFactory


LogOutputStream* DefaultLogFactory::createOutputStream (
			const UTF8String& name, Log::TYPE mask, bool truncate,
			bool enableDate, bool enableTime)
{
	unique_ptr<LogOutputStream>	stream;

	if (name.ascii ( ) == "stdout")
		stream.reset (new OstreamLogOutputStream (
								cout, mask, enableDate, enableTime));
	else if (name.ascii ( ) == "stderr")
		stream.reset (new OstreamLogOutputStream (
								cerr, mask, enableDate, enableTime));
	else
		stream.reset (new FileLogOutputStream (
								name, truncate, mask, enableDate, enableTime));

	CHECK_NULL_PTR_ERROR (stream.get ( ))

	if (false == stream->isEnabled ( ))
	{
		UTF8String	error (charset);
		error << "Flux de messages " << name << " invalide";

		if (0 != dynamic_cast<FileLogOutputStream*>(stream.get ( )))
		{
			File	file (name);
			if (false == file.isWritable ( ))
				error << " absence de droits en écriture sur ce fichier.";
		}	// if (0 != dynamic_cast<FileLogOutputStream*>(stream.get ( )))

		throw Exception (error);
	}	// if (false == stream->isEnabled ( ))

	return stream.release ( );
}	// DefaultLogFactory::createOutputStream



END_NAMESPACE_UTIL
