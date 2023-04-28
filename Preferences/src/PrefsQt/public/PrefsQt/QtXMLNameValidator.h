#ifndef QT_XML_NAME_VALIDATOR_H
#define QT_XML_NAME_VALIDATOR_H

#include <PrefsCore/prefs_config.h>

#include <QValidator>




/**
 * Classe permettant la validation des noms XML.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class QtXMLNameValidator : public QValidator
{
	public :

	/**
	 * Constructeur et destructeur : RAS.
	 */
	QtXMLNameValidator (QObject* parent, const char* name = 0);
	virtual ~QtXMLNameValidator ( );

	/**
	 * Valide la chaine reçue en argument.
	 * @return		QValidator::Invalid, QValidator::Intermediate ou
	 * 				QValidator::Acceptable selon le cas.
	 */
	QValidator::State validate (QString& input, int& pos) const;


	protected :

	/**
	 * Constructeur de copie. Interdit.
	 */
	QtXMLNameValidator (const QtXMLNameValidator&);

	/**
	 * Opérateur =. Interdit.
	 */
	QtXMLNameValidator& operator = (const QtXMLNameValidator&);
};	// class QtXMLNameValidator

#endif	// QT_XML_NAME_VALIDATOR_H
