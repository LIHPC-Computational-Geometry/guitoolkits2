#ifndef XML_LOADER_H
#define XML_LOADER_H

#include <PrefsCore/prefs_config.h>
#include <PrefsCore/Section.h>
#include <TkUtil/Version.h>
#include <PrefsXerces/XercesUtils.h>
#include <xercesc/dom/DOMElement.hpp>

#include <string>
#include <memory>


BEGIN_NAMESPACE_PREFS


/** <P>Classe utilitaire de configuration permettant de :
 * <UL>
 * <LI>- Charger une configuration depuis un fichier XML,</LI>
 * <LI>- D'enregistrer une configuration dans un fichier XML.</LI>
 * </UL>
 * </P>
 * <P>Le fichier utilise une notation XML schema sans schema. Il est autonome. L'encodage peut être fait au format UTF_8, ISO 8859, ou UTF_16. Dans le 
 * premier cas, seuls les caractères ASCII seront pris en compte.
 * <P>
 * <P>Le format reprend la notion de <B>types de données simples et complexes </B> de XML schema. Types de <B>données simples</B> :<BR>
 * <UL>
 * <LI><I>string</I>,</LI>
 * <LI><I>boolean</I>,</LI>
 * <LI><I>long</I>,</LI>
 * <LI><I>unsignedLong</I>,</LI>
 * <LI><I>double</I>.</LI>
 * </UL>
 * <P>Ces données simples sont néanmoins stockées dans des données XML schema complexes afin de pouvoir y adjoindre des commentaires et des attributs 
 * renseignant le nom et le type de la donnée.<P>
 * <P>Types de <B>données complexes</B> (et spécifiques à ce module) :<BR>
 * <UL>
 * <LI><I>color</I>, utilisé pour représenter des couleurs à l'aide des composantes RGB codées sous forme de double compris entre 0 et 1,
 * <LI><I>doubleTriplet</I>, utilisé, par exemple, pour des coordonnées dans un espace tri-dimensionnel.
 * </UL><P>
 * <P>Les <B>sections</B> doivent avoir quant à elle pour <B>type</B> <I>container</I>.
 * </P>
 * <P>Chaque élément, quelque soit son type, est identifié dans le fichier XML par ses attributs <I>type</I> et <I>name</I>. Ensuite, selon son type,
 * d'autres informations pourront être nécessaires. Les types simples requièrent par exemple la présence d'un sous-élément XML simple <I>value</I>
 * contenant la valeur de l'élément. Ce sous-élément ne doit pas contenir lui même d'autres sous-éléments, au risque de ne pas pouvoir lire correctement
 * la valeur.<BR>
 * La syntaxe des fichiers est comme suit :<BR>
 * <CODE>
 * <?xml version="1.0" encoding="UTF-8" standalone="yes" ?><BR>
 * <NomSectionPrincipale XMLLoader_version="0.2.0"><BR>
 *		<Section type="container" name="NomSection1"><BR>
 *			<Section type="container" name="NomSection1_1"><BR>
 *				<element type="boolean" name="NomValeurSimple1"><BR>
 *					<value>false</value><BR>
 *				</element><BR>
 *				<element type="double" name="NomValeurSimple2"><BR>
 *					<value>1.2E3</value><BR>
 *				</element><BR>
 *				<element type="string" name="NomValeurSimple3"><BR>
 *					<value>Preferences XML</value><BR>
 *				</element><BR>
 *			</Section><BR>
 *			<Section type="container" name="NomSection1_2"><BR>
 *				<element type="unsignedLong" name="NomValeurSimple1"><BR>
 *					<value>18</value><BR>
 *				</element><BR>
 *				<element type="color" name="NomCouleur"><BR>
 *					<red>1</red><BR>
 *					<green>1</green><BR>
 *					<blue>1</blue><BR>
 *				</element><BR>
 *				<element type="doubleTriplet" name="NomTripletDouble"><BR>
 *					<x>2</x><BR>
 *					<y>.3</y><BR>
 *					<z>4</z><BR>
 *				</element><BR>
 *			</Section><BR>
 *			<element type="long" name="NomValeurSimple1"><BR>
 *				<value>-13</value><BR>
 *			</element><BR>
 *		</Section><BR>
 * </NomSectionPrincipale><BR>
 * </CODE>
 * <P>Une section peut contenir des sous-sections et/ou des valeurs, et chaque entité peut avoir un commentaire textuel dans rubrique <I>documentation</I>
 * de la rubrique <I>annotation</I> (cf. XML schema). Exemple :<BR>
 * </P>
 * <CODE>
 * <element type="doubleTriplet" name="vitesse"><BR>
 *		<x>10.5</x><BR>
 *		<y>0.</y><BR>
 *		<z>0.</z><BR>
 *		<annotation><BR>
 *			<documentation>Composantes de la vitesse de déplacement</documentation><BR>
 *		</annotation><BR>
 * </element><BR>
 * </CODE>
 * <P>Les <B>informations des fichiers XML utiles à ce module </B>sont les suivantes :<BR>
 * <UL>
 * <LI>Les attributs <I>type</I>, conformément à ceux décrits ci-dessus,</LI>
 * <LI>Les attributs <I>name</I>,</LI>
 * <LI>Les sous-éléments <I>value</I> pour les éléments simples,</LI>
 * <LI>Les sous-éléments <I>x</I>, <I>y</I> et <I>z</I> pour les éléments de type triplet de doubles,</LI>
 * <LI>Les sous-éléments <I>red</I>, <I>green</I> et <I>blue</I> pour les éléments de type couleur,</LI>
 * <LI>Optionnellement les sous-éléments <I>annotation</I> avec leur sous-élément <I>documentation</I> pour d'éventuels commentaires.</LI>
 * </UL>
 * Cela signifie donc que les noms XML des éléments ne sont pas utilisés par ce module. Les noms <I>Section</I> et <I>element</I> sont proposés par
 * défaut, mais ils peuvent être remplacés par d'autres noms, par exemple à des fins de couplage à un schéma et de vérification du fichier XML par un outil 
 * tiers. Ce module propose les services nécessaires à la conservation de cette information de manière persistante (notion de <B>type applicatif</B>).
 * </P>
 * <P>A partir de la version 5.7.0 de cette bibliothèque (version 0.3.0 du format de fichier), les éléments peuvent être <I>surchargeables</I>,
 * c.a.d. modifiables, via une IHM ou via le chargement d'un autre fichier par exemple. La nouveauté ici réside dans le fait que via cette propriété les éléments
 * peuvent être en fait non surchargeables. Cela inéresse par exemple un fichier de ressources d'une application dont on veut imposer certaines valeurs
 * à certaines ressources, en interdire la modification aux utilisateurs de l'application. Pour ce l'attribut <I>overloable</I> de l'élément doit recevoir la valeur
 * <I>"false"</I>. En l'absence de cet attribut l'élément est considéré comme étant surchargeable (comportement par défaut).
 * </P>
 * <P>Toujours à partir de la version 5.7.0 de cette bibliothèque les éléments peuvent être <I>sauvegardables</I>,
 * c.a.d. enregistrables dans un fichier. La nouveauté ici réside dans le fait que via cette propriété les éléments peuvent être en fait non enregistrables.
 * Cela inéresse également, par exemple, un fichier de ressources d'une application dont on veut imposer certaines valeurs à certaines ressources.
 * Dès lors une sauvegarde dans un fichier utilisateur ne revêt aucun intérêt. Pour ce l'attribut <I>safeguardable</I> de l'élément doit recevoir la valeur
 * <I>"false"</I>. En l'absence de cet attribut l'élément est considéré comme étant enregistrable (comportement par défaut).
 * </P>
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class XMLLoader 
{
	public :


	/**
	 * L'encodage à utiliser.
	 */
	enum ENCODING { UTF_8, ISO_8859, UTF_16 };

	/**
	 * Charge une configuration depuis un fichier XML dont le nom est transmis
	 * en argument.
	 * @param		Chemin d'accès complet au fichier à lire.
	 * @param		Section initiale. En cas de lecture d'éléments déjà présents, ceux-ci sont modifiésconformément à ce qui est lu.
	 * @return		Configuration chargée. Sa destruction est à la charge de l'appelant.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static Section* load (const IN_STD string& fileName, Section* initialSection = 0);

	/**
	 * Enregistre la section transmise en arguement dans le fichier dont le nom est donné en second argument.
	 * @param		Configuration à enregistrer.
	 * @param		Chemin d'accès complet au fichier à écrire.
	 * @param		Encodage à utiliser. UTF_8 par défaut (caractères ASCII uniquement).
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static void save (const Section& section, const IN_STD string& fileName, ENCODING encoding = UTF_8);

	/**
	 * Conversion UnicodeString -> XMLChar*.
	 * @param		Chaine a convertir
	 * @param		Encodage à utiliser.
	 * @return		Chaine convertie à recopier immédiatement, écrasée par tout appel ultérieur à cette fonction.
	 * @exception	Une exception peut être levée en cas d'erreur.
	 */
	static const XMLCh* unicodeToXMLString (const IN_UTIL UnicodeString& str, ENCODING encoding);

	/**
	 * Conversion UTF8String -> XMLChar*.
	 * @param		Chaine a convertir
	 * @param		Encodage à utiliser.
	 * @return		Chaine convertie à recopier immédiatement, écrasée par tout appel ultérieur à cette fonction.
	 * @exception	Une exception peut être levée en cas d'erreur.
	 */
	static const XMLCh* unicodeToXMLString (const IN_UTIL UTF8String& str, ENCODING encoding);

	/** L'attribut XML définissant le nom d'un élément.
	 */
	static const IN_STD string			nameAttrName;

	/** L'attribut XML définissant le type d'un élément.
	 */
	static const IN_STD string			typeAttrName;
	
	/** L'attribut XML définissant le caractère surchargeable d'un élément.
	 */
	static const IN_STD string			overloadableAttrName;			// v 5.7.0

	/** L'attribut XML définissant le caractère enregistrable d'un élément.
	 */
	static const IN_STD string			safeguardableAttrName;			// v 5.7.0
	
	/** L'élément XML définissant la valeur d'un élément simple.
	 */
	static const IN_STD string			valueElementName;

	/**
	 * La version de ce module.
	 */
	static const IN_UTIL Version		version;

	/**
	 * La balise indiquant la version de ce module.
	 */
	static const IN_STD string			versionTag;

	/**
	 * Classe destinée à stocker des informations sur le fichier XML telles que l'utilisation d'un schéma.
	 */
	class XMLDocInformations : public Element::OriginInfos
	{
		public :

		/**
		 * Constructeur. Affecte "XML Document" au nom de l'instance créée.
		 */
		XMLDocInformations ( );

		/**
		 * Constructeur de copie et opérateur = : RAS.
		 */
		XMLDocInformations (const XMLDocInformations& toCopy);
		XMLDocInformations& operator = (const XMLDocInformations& toCopy);

		/**
		 * Destructeur.
		 */
		virtual ~XMLDocInformations ( );

		/**
		 * @return		true si un schéma est associé au document.
		 * @see			getSchemaURL
		 */
		virtual bool hasSchema ( ) const;

		/**
		 * @return		l'URL du schéma associé au document.
		 * @see			hasSchema
		 */
		virtual IN_STD string getSchemaURL ( ) const;

		/**
		 * @param		l'URL du schéma associé au document.
		 * @see			getSchemaURL
		 */
		virtual void setSchemaURL (const IN_STD string& url);

		/**
		 * La balise de localisation des schémas. 
		 */
		static const IN_STD string			noNamespaceSchemaLocationTag;

		/**
		 * La balise de localisation de la description des schémas. 
		 */
		static const IN_STD string			xmlnsXsiTag;

		/**
		 * <P>
		 * Classe décrivant un espace de nommage XML. Un espace de nommage XML est définit par le prefix utilisé (ex : xsi, xsd) et l'URL où le
		 * type prefixé est définit.<BR>
		 * Ex : xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
		 * </P>
		 * <P>Probablement à approfondir en cas d'utilisation plus poussée de XML schema.
		 * </P>
		 */
		class XMLNamespace
		{
			public :

			/**
			 * Constructeur.
			 * @param		Prefix de l'espace de nommage
			 * @param		URL contenant la définition de l'élément
			 */
			XMLNamespace (const IN_STD string& prefix,const IN_STD string& url)
				: _prefix (prefix), _url (url)
			{ }

			/**
			 * Constructeur de copie et opérateur = : RAS.
			 */
			XMLNamespace (const XMLNamespace& toCopy)
				: _prefix (toCopy.getPrefix ( )), _url (toCopy.getURL ( ))
			{ }
			XMLNamespace& operator = (const XMLNamespace& toCopy)
			{
				if (&toCopy != this)
				{
					setPrefix (toCopy.getPrefix ( ));
					setURL (toCopy.getURL ( ));
				}
				return *this;
			}	// operator =

			/**
			 * Destructeur. RAS.
			 */
			~XMLNamespace ( )
			{ }

			/**
			 * @return		Le prefix de l'espace de nommage.
			 */
			const IN_STD string& getPrefix ( ) const
			{ return _prefix; }

			/**
			 * @param		Le prefix de l'espace de nommage.
			 */
			void setPrefix (const IN_STD string& prefix)
			{ _prefix	= prefix; }

			/**
			 * @return		L'URL contenant la définition de l'espace de nommage
			 */
			const IN_STD string& getURL ( ) const
			{ return _url; }

			/**
			 * @param		L'URL contenant la définition de l'espace de nommage
			 */
			void setURL (const IN_STD string& url)
			{ _url	= url; }


			private :


			/** Le prefix. */
			IN_STD string			_prefix;

			/** L'URL de définition de l'espace de nommage. */
			IN_STD string			_url;
		};	// class XMLNamespace

		/**
		 * @return		true si un espace de nommage est utilisé.
		 * @see			getNamespace
		 */
		virtual bool hasNamespace ( ) const;

		/**
		 * @return		l'espace de nommage utilisé.
		 * @exception	Une exception est levée en l'absence d'espace de nommage utilisé.
		 * @see			hasNamespace
		 */
		virtual const XMLNamespace& getNamespace ( ) const;

		/**
		 * @param		l'espace de nommage utilisé, ou 0. L'instance de XMLNamespace transmise est adoptée et de ce faite détruite par l'instance de XMLLoader.
		 * @see			getNamespace
		 */
		virtual void setNamespace (XMLNamespace* ns);

		/**
		 * @return		Une copie de l'instance.
		 */
		virtual Element::OriginInfos* clone ( ) const;


		protected :


		/**
		 * @param		Informations à copier.
		 */
		void copy (const XMLDocInformations& toCopy);


		private :


		/** Le schéma utilisé. */
		IN_STD string								_schemaURL;

		/** L'espace de nommage utilisé. */
		IN_STD unique_ptr<XMLNamespace>				_namespace;
	};	// class XMLDocInformations


	protected :


	/**
	 * @return		true si l'élément est un type simple, sinon false. Un type simple n'est composé que d'une seule valeur,
	 *				contrairement à un type complexe qui en a plusieurs (ex : couleur et ses composantes RGB).
	 */
	static bool isSimpleType (const Element& element);

	/**
	 * Méthode créée pour assurer la compatibilité entre les version 0.1 et 0.2 de ce module (bool/boolean, unsigned long/unsignedLong).
	 * @return		true si les deux types sont compatibles (ex : bool et  boolean), sinon false.
	 */
	static bool typesAreCompatible (const IN_STD string& type1, const IN_STD string& type2);

	/**
	 * @return		Le nom de l'élément représenté par l'instance d'élément DOM transmise en argument.
	 */
	static IN_UTIL UTF8String getElementName (const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& element);

	/**
	 * @return		Le type de l'élément représenté par l'instance d'élément DOM transmise en argument.
	 */
	static IN_STD string getElementType (const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& element);
	
	/**
	 * @return		true si l'élément est surchargeable, false dans le cas contraire
	 */
	static bool isElementOverloadable (const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& element);	// v 5.7.0

	/**
	 * @return		true si l'élément est enregistrable, false dans le cas contraire
	 */
	static bool isElementSafeguardable (const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& element);	// v 5.7.0
	
	/**
	 * Charge la section transmise en argument à l'aide des informations contenues dans l'élément DOM reçu en second argument. Cette fonction met
	 * à jour les commentaires, ses sous-sections et éléments.
	 */
	static void loadSection (Section& section, const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& node);

	/**
	 * Ajoute ou met à jour la valeur nommée de la section "section" et de nom "name" à partir des informations contenues dans "node".
	 * @param		section contenant la valeur nommée
	 * @param		nom de la valeur nommée
	 * @param		élément DOM contenant les informations relatives à la valeur nommée
	 * @param		type (renseigné par le fichier XML) de la valeur nommée
	 * @param		type, au sens applicatif, de la valeur nommée
	 * @param		Caractère surchargeable de l'élément.		// v 5.7.0
	 * @param		Caractère enregistrable de l'élément.		// v 5.7.0
	 */
	 static void updateNamedValue (
					Section& section, const IN_UTIL UTF8String& name, const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& node,
					const IN_STD string& type,  const IN_UTIL UTF8String& appType, bool overloadable, bool safeguardable);

	/**
	 * @return		une valeur nommée de nom "name" et de type (lu dans le fichier XML) correspondant à celui donné en second argument.
	 */
	static NamedValue* createNamedValue (const IN_UTIL UTF8String& name, const IN_STD string& type, bool overloadable, bool safeguardable);

	/**
	 * Met à jour la valeur nommée d'un type simple reçue en premier argument à l'aide des informations contenues dans l'élément DOM transmis en 
	 * second argument. Cette mise à jour comprend valeur et commentaires.
	 * @see			isSimpleType
	 * @see			updateComplexValue
	 * @see			getStrValue
	 */
	static void updateSimpleValue (NamedValue& namedValue, const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& node);

	/**
	 * @return		La valeur de l'élément simple dont les informations sont contenues dans l'élément DOM transmis en argument.
	 */
	static IN_UTIL UTF8String getStrValue (const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& node);

	/**
	 * Met à jour la valeur nommée d'un type complexe reçue en premier argument à l'aide des informations contenues dans l'élément DOM transmis en 
	 * second argument. Cette mise à jour comprend valeur et commentaires.
	 * @see			isSimpleType
	 * @see			updateSimpleValue
	 */
	static void updateComplexValue (NamedValue& namedValue, const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& node);

	/**
	 * Met à jour les commentaires de l'élément tranmis en premier argument à l'aide des informations contenues dans l'élément DOM transmis en 
	 * second argument.
	 */
	static void updateComments (Element& element, const XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& node);

	/**
	 * Ajoute la descendance enregistrable de la section transmise en argument à l'élément DOM reçu en second argument.
	 * @param		section dont il faut ajouter la descendance
	 * @param		élément DOM destiné à assurer la persistance de "section"
	 * @param		document DOM contenant "element"
	 * @param		encodage à utiliser. Dans le cas où il vaut <I>UTF_8</I>, les chaines de caractères utilisées seront au format ASCII.
	 */
	static void addChildren (const Section& section,  XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& element, XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument& doc, 
					ENCODING encoding);

	/**
	 * Ajoute la valeur nommée "element" de type simple à l'élément DOM "container". L'élément DOM créé est ajouté à "container" par sa méthode
	 * <I>appendChild</I>.
	 * @param		valeur nommée à ajouter
	 * @param		conteneur de l'élément DOM destiné à assurer la persistance de "element"
	 * @param		document DOM contenant "element"
	 * @param		encodage à utiliser. Dans le cas où il vaut <I>UTF_8</I>, les chaines de caractères utilisées seront au format ASCII.
	 * @return		élément DOM créé
	 * @see			addComplexNamedValue
	 * @see			createElement
	 */
	static XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* addSimpleNamedValue (
					const NamedValue& element, XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& container, XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument& doc, 
					ENCODING encoding);
	
	/**
	 * Ajoute la valeur nommée "element" de type complexe à l'élément DOM "container". L'élément DOM créé est ajouté à "container" par sa méthode
	 * <I>appendChild</I>.
	 * @param		valeur nommée à ajouter
	 * @param		conteneur de l'élément DOM destiné à assurer la persistance de "element"
	 * @param		document DOM contenant "element"
	 * @param		encodage à utiliser. Dans le cas où il vaut <I>UTF_8</I>, les chaines de caractères utilisées seront au format ASCII.
	 * @return		élément DOM créé
	 * @see			addSimpleNamedValue
	 * @see			createElement
	 */
	static XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* addComplexNamedValue (
					const NamedValue& element, XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& container, XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument& doc, 
					ENCODING encoding);

	/**
	 * Créé un élément DOM destiné à représenter "element". Cette méthode donne les renseignements de base à l'élément DOM : nom, type et 
	 * commentaires de l'élément. Le nom de l'élément XML est déterminé par XMLSchemaPrefsHelper::elementToXMLschemaTypeName. Des informations
	 * complémentaires peuvent être ajoutées par addSimpleNamedValue ou addComplexNamedValue.
	 * @param		élément dont on demande la création d'une instance DOM représentative
	 * @param		document conteanant l'instance à créer
	 * @param		encodage à utiliser. Dans le cas où il vaut <I>UTF_8</I>, les chaines de caractères utilisées seront au format ASCII.
	 * @return		instance créée
	 */
	static XERCES_CPP_NAMESPACE_QUALIFIER DOMElement* createElement (const Element& element,  XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument& doc, ENCODING encoding);

	/**
	 * Ajoute les commentaires de l'élément "element" à l'élément DOM "node".
	 * @param		élément dont on demande l'ajout des commentaires
	 * @param		document contenant l'instance DOM
	 * @param		encodage à utiliser. Dans le cas où il vaut <I>UTF_8</I>, les chaines de caractères utilisées seront au format ASCII.
	 */
	static void addComments (const Element& element, XERCES_CPP_NAMESPACE_QUALIFIER DOMElement& node, XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument& doc,
					ENCODING encoding);


	private :

	/**
	 * Constructeur. Interdit.
	 */
	XMLLoader ( )
	{ }

	/**
	 * Constructeur de copie. Interdit.
	 */
	XMLLoader (const XMLLoader&)
	{ }

	/**
	 * Opérateur =. Interdit.
	 */
	XMLLoader& operator = (const XMLLoader&)
	{ return *this; }

	/**
	 * Destructeur : RAS.
	 */
	virtual ~XMLLoader ( )
	{ }
};	// class XMLLoader




END_NAMESPACE_PREFS

#endif	// XML_LOADER_H
