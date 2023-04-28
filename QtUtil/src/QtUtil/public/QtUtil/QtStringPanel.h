#ifndef QT_STRING_PANEL_H
#define QT_STRING_PANEL_H

#include <TkUtil/util_config.h>
#include <QtUtil/QtTextField.h>
#include <QtUtil/QtGroupBox.h>
#include <TkUtil/UTF8String.h>

#include <string>


/**
 * Panneau Qt permettant la saisie d'une chaine de caractères.
 */
class QtStringPanel : public QtGroupBox 
{
	public :

	/** Constructeur
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		titre du panneau
	 * @param		true si le panneau est éditable, sinon false.
	 * @param		nom de la chaine de caractères
	 * @param		valeur initiale de la chaine de caractères.
	 * @param		éventuel validateur du champs de saisie.
	 */
	QtStringPanel (QWidget* parent, const IN_UTIL UTF8String& title,
	               bool editable,
	               const IN_UTIL UTF8String& strTitle,
	               const IN_UTIL UTF8String& value,
	               QValidator* validator = 0);

	/** Destructeur. */
	virtual ~QtStringPanel ( );

	/**
	 * @return			true si l'utilisateur a modifié la chaine de 
	 *					caractères.
	 */
	virtual bool isModified ( ) const;

	/** @return			la chaine de caractere.
	 * @throw			Exception si la valeur est incorrecte. */
	virtual IN_UTIL UTF8String getValue ( ) const;

	/** @return			la chaine de caractere.
	 * @throw			Exception si la valeur est incorrecte. */
	virtual IN_STD string getStdValue ( ) const;


	protected :


	private :

	/** Constructeur de copie. Interdit. */
	QtStringPanel (const QtStringPanel&);

	/** Opérateur de copie. Interdit. */
	QtStringPanel& operator = (const QtStringPanel&);

	/** La valeur initiale de la chaine de caractères. */
	IN_UTIL UTF8String		_initialValue;

	/** Le champ de saisie de la chaine de caractères. */
	QtTextField*			_textField;
};	// class QtStringPanel



#endif	// QT_STRING_PANEL_H
