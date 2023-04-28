#ifndef ELEMENT_H
#define ELEMENT_H

#include <PrefsCore/prefs_config.h>

#include <TkUtil/UTF8String.h>

#include <string>
#include <ostream>
#include <memory>


BEGIN_NAMESPACE_PREFS

class Section;



/**
 * <P>Classe abstraite représentant un élément de configuration.</P>
 * <P>Un élément de configuration est caractérisé par :<BR>
 * <UL>
 * <LI>Son nom,
 * <LI>Son type,
 * <LI>Son éventuel parent,
 * <LI>Son éventuel commentaire associé,
 * <LI>Des éventuelles informations associées à l'origine de l'élément, propres au moyen de persistance utilisé ou à l'application,
 * <LI>Son aptitude à être surchargé (valeur modifiée),
 * <LI>Son aptitude à être enregistré (une ressource "système" partagée par l'ensemble des utilisateurs, peut ne pas être enregistrable
 * par les utilisateurs).
 * </UL>
 * <P>Cette classe permet également de caractériser tout élément par un type propre à l'application et à son environnement applicatif.
 * Ce type peut par exemple être un conteneur représenté par la classe Section.
 * </P>
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class Element 
{
	friend class Section;


	public :

	/**
	 * Destructeur : RAS.
	 */
	virtual ~Element ( )
	{ }

	/**
	 * @return		Une chaine de caractères décrivant le type de l'élément au sens de cette API ("section", "double", ...).
	 */
	virtual IN_STD string getType ( ) const = 0;

	/**
	 * @return		Une chaine de caractères décrivant le type de l'élément au sens de l'application et de son environnement applicatif.
	 *				Retourne une chaine vide par défaut.
	 * @see			setAppType
	 */
	virtual const IN_UTIL UTF8String& getAppType ( ) const;

	/**
	 * @param		Chaine de caractères décrivant le type de l'élément au sens de l'application et de son environnement applicatif.
	 */
	virtual void setAppType (const IN_UTIL UTF8String& appType);

	/**
	 * @return		true si l'élément et son éventuel parent sont surchargeables, false dans le cas contraire.
	 */
	virtual bool isOverloadable ( ) const;					// v 5.7.0
	
	/**
	 * @param		true si l'élément est surchargeable, false dans le cas contraire.
	 * @warning		<B>A usage des loaders uniquement, pour éviter de brider l'API avec le mot clé <I>friend</I></B>.
	 */
	virtual void setOverloadable (bool overloable);			// v 5.7.0
	
	/**
	 * @return		true si l'élement est surchargeable, false dans le cas contraire.
	 * @exception	Lève une exception si l'élément n'est pas surchargeable et si raise vaut true.
	 * @see			isOverloadable
	 */
	virtual bool checkForModification (bool raise = true);	// v 5.7.0

	/**
	 * @return		true si l'élément est enregistrable, false dans le cas contraire.
	 */
	virtual bool isSafeguardable ( ) const;					// v 5.7.0
	
	/**
	 * @param		true si l'élément est enregistrable, false dans le cas contraire.
	 * @warning		<B>A usage des loaders uniquement, pour éviter de brider l'API avec le mot clé <I>friend</I></B>.
	 */
	virtual void setSafeguardable (bool safeguardable);		// v 5.7.0
	
	/**
	 * @return		true si l'élement et son éventuel parent sont enregistrables, false dans le cas contraire.
	 * @exception	Lève une exception si l'élément n'est pas enregistrable et si raise vaut true.
	 * @see			isSafeguardable
	 */
	virtual bool checkForSafeguard (bool raise = true);		// v 5.7.0
	
	/**
	 * @return		Le nom de l'élément.
	 */
	virtual const IN_UTIL UTF8String& getName ( ) const
	{ return _name; }

	/**
	 * @return		true si l'élément a un parent, sinon false.
	 */
	virtual bool hasParent ( ) const
	{ return 0 == _parent ? false : true; }

	/**
	 * @return		Le parent de l'élément.
	 * @exception	Une exception est levée si l'élément n'a pas de parent.
	 */
	virtual Section& getParent ( ) const;

	/**
	 * @return		L'éventuel commentaire associé.
	 */
	virtual const IN_UTIL UTF8String& getComment ( ) const;

	/**
	 * @param		L'éventuel commentaire associé.
	 */
	virtual void setComment (const IN_UTIL UTF8String& comment);

	/**
	 * @return		true si l'élément a un commentaire associé, sinon false.
	 */
	virtual bool hasComment ( ) const;

	/**
	 * @return		true si l'élément est "imprimable" en caractères ASCII, false dans le cas contraire.
	 */
	virtual bool isAscii ( ) const;

	/**
	 * @return		true si l'élément est "imprimable" en caractères ISO 8859, false dans le cas contraire.
	 */
	virtual bool isIso ( ) const;

	/**
	 * @return		Une copie de l'instance.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual Element* clone ( ) const = 0;

	/**
	 * Affichage dans le flux reçu en argument de l'élément.
	 * @param		Flux utilisé
	 * @param		Niveau d'indentation initial
	 */
	virtual void print (IN_STD ostream& stream, size_t indent = 0) const = 0;


	/**
	 * Classe destinée à véhiculer des informations sur l'origine de 
	 * l'élément.
	 */
	class OriginInfos
	{
		public :

		/**
		 * @param		Nom du moyen de persistance, de l'application, ... à l'origine de cet élément.
		 */
		OriginInfos (const IN_UTIL UTF8String& name);

		/**
		 * Constructeur de copie et opérateur =. RAS.
		 */
		OriginInfos (const OriginInfos& toCopy);
		OriginInfos& operator = (const OriginInfos& toCopy);

		/**
		 * Destructeur. RAS.
		 */
		virtual ~OriginInfos ( );

		/**
		 * @return		Le nom du moyen de persistance, de l'application, ...
		 */
		virtual const IN_UTIL UTF8String& getName ( ) const
		{ return _name; }

		/**
		 * @param		Le nom du moyen de persistance, de l'application, ...
		 */
		virtual void setName (const IN_UTIL UTF8String& name)
		{ _name	= name; }

		/**
		 * @return		Une copie de l'instance.
		 */
		virtual OriginInfos* clone ( ) const;


		protected :

		/**
		 * @param		Informations à copier.
		 */
		void copy (const OriginInfos& toCopy);


		private :

		/** Le nom du moyen de persistance, de l'application, ... */
		IN_UTIL UTF8String			_name;
	};	// class OriginInfos


	/**
	 * @param		Informations sur le moyen de persistance ou sur l'application associées à l'instance. Ces informations
	 *				sont adoptées et de ce fait détruites par l'instance.
	 */
	virtual void setOriginInfos (OriginInfos* infos);

	/**
	 * @return		Les éventuelles informations sur le moyen de persistance ou sur l'application associées à l'instance.
	 */
	virtual OriginInfos* getOriginInfos ( );
	virtual const OriginInfos* getOriginInfos ( ) const;


	protected :

	/**
	 * Constructeur.
	 * @param		Nom de l'élément.
	 * @param		Eventuel commentaire associé.
	 * @param		Caractère surchargeable de l'élément.
	 * @param		Caractère enregistrable de l'élément.
	 */
	Element (const IN_UTIL UTF8String& name, const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	Element (const Element& element);

	/**
	 * Opérateur =. Copie le nom de l'élément transmis en argument.
	 */
	Element& operator = (const Element& element);

	/**
	 * Effectue une copie de l'element reçu en argument.
	 * @param		Element à copier.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	void copy (const Element& elem);
	

	private :


	/**
	 * @param		Nouveau nom de l'élément. Ne doit pas contenir d'espaces et ne doit pas être nul.
	 */
	virtual void setName (const IN_UTIL UTF8String& name);

	/**
	 * Précondition : ne doit pas déjà avoir de parent.
	 * @param		Parent de l'élément. 
	 */
	virtual void setParent (Section* parent);

	/** Le nom de l'élément. */
	IN_UTIL UTF8String					_name;
	
	/** Caractère surchargeable de l'élément. */
	bool								_overloadable;		// v 5.7.0
	
	/** Caractère enregistrable de l'élément. */
	bool								_safeguardable;		// v 5.7.0
	
	/** Le type, dans le cadre applicatif, de l'élément. */
	IN_UTIL UTF8String					_appType;

	/** Le parent de l'élément, s'il y en a un. */
	Section*							_parent;

	/** L'éventuel commentaire associé à l'élément. */
	IN_UTIL UTF8String					_comment;

	/** Les éventuelles informations de persistance ou applicatives de
	 * l'instance. */
	IN_STD unique_ptr<OriginInfos>		_infos;
};	// class Element


IN_STD ostream& operator << (IN_STD ostream& os, const Element& elem);



END_NAMESPACE_PREFS

#endif	// ELEMENT_H
