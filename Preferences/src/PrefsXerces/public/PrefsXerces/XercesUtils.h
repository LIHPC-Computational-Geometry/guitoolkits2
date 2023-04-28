/**
 * Classes utilitaires pour utiliser Xerces.
 */

#ifndef XERCES_UTILS_H
#define XERCES_UTILS_H

#include <PrefsCore/prefs_config.h>

#include <xercesc/util/TransService.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>

#include <iostream>

#define XERCES_CPP_NAMESPACE_QUALIFIER XERCES_CPP_NAMESPACE::
#define XERCES_CPP_NAMESPACE_USE using namespace XERCES_CPP_NAMESPACE;


/*!
 * \mainpage    Page principale de la bibliothèque PrefsXerces.
 *
 * \section presentation	Présentation
 * <P>La bibliothèque <I>prefsxerces</I> permet de gérer la persistance
 * (sauvegarde/chargement) de préférences utilisateurs
 * dans un fichier <B>html</B>. La classe <I>XMLLoader</I> est le point
 * d'entrée de cette bibliothèque.</P>
 * <P>La gestion de la persistance repose sur la bibliothèque <I>XercesC</I>.
 *</P>
 *
 * \section depend      Dépendances
 * Cette bibliothèque utilise la bibliothèque TkUtil.
 * Les directives de compilation et d'édition des liens de la bibliothèque
 * <I>TkUtil</I> s'appliquent également à celle-ci.
 */

BEGIN_NAMESPACE_PREFS


/**
 * Conversion chaine de caractère 'C' vers chaine de caractères Xerces.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class XStr
{
	public :

	XStr (const char* const toTranscode)
	{
		fUnicodeForm	= 
			XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode (toTranscode);
	}

	~XStr ( )
	{
		if (0 != fUnicodeForm)
			 XERCES_CPP_NAMESPACE_QUALIFIER XMLString::release (&fUnicodeForm,
				XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::fgMemoryManager);
	}

	const XMLCh* unicodeForm ( ) const
	{ return fUnicodeForm; }


	private :

	XStr (const XStr&)
		: fUnicodeForm (0)
	{ }
	XStr& operator = (const XStr&)
	{ return *this; }
	XMLCh*	fUnicodeForm;
};	// class XStr


#define X(str) XStr(str).unicodeForm()



/**
 * Conversion chaine de caractères Xerces vers chaine de caractères 'C'.
 * @warning	<B>ATTENTION, aptitude au multithreading non validée</B>
 */
class StrX
{
	public :

    StrX (const XMLCh* const toTranscode);		// v 5.6.0
/*    {
        // Call the private transcoding method
        fLocalForm = 
			XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode(toTranscode);
    } */

    ~StrX ( )
    {
		delete [] fLocalForm;		fLocalForm	= 0;	// v 5.6.0
/*		if (0 != fLocalForm)
			 XERCES_CPP_NAMESPACE_QUALIFIER XMLString::release (&fLocalForm,
				XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils::fgMemoryManager); */
    }

    const char* localForm ( ) const
    {
        return fLocalForm;
    }

	private :

	StrX (const StrX&)
		: fLocalForm (0)
	{ }
	StrX& operator = (const StrX&)
	{ return *this; }

    char*   fLocalForm;
    static XERCES_CPP_NAMESPACE_QUALIFIER	XMLTranscoder*	utf8Transcoder;				// v 5.6.0 Shared native local form -> UTF_8 transcoder
    static char												bytesNodeValue [16*1024+4];	// v 5.6.0 Shared buffer for utf8Transcoder
};	// class StrX


inline IN_STD ostream& operator<<(IN_STD ostream& target, const StrX& toDump)
{
    target << toDump.localForm();
    return target;
}


/**
 * Handler d'erreur Xerces affichant dans cout les erreurs interceptées.
 */
class ConfigErrorHandler : public XERCES_CPP_NAMESPACE_QUALIFIER DOMErrorHandler
{
	public :

	ConfigErrorHandler ( ) : DOMErrorHandler ( )
	{ }

	ConfigErrorHandler (const ConfigErrorHandler& handler)
		: DOMErrorHandler ( )
	{ }

	ConfigErrorHandler& operator = (const ConfigErrorHandler& handler)
	{
		return *this;
	}	// operator =

	virtual bool handleError (
				const XERCES_CPP_NAMESPACE_QUALIFIER DOMError& domError);
};	// class ConfigErrorHandler

END_NAMESPACE_PREFS

#endif		// XERCES_UTILS_H
