#ifndef QT_INT_TEXT_FIELD_H
#define QT_INT_TEXT_FIELD_H

#include <QtUtil/QtValidatedTextField.h>


class QIntValidator;


/**
 * <P>Editeur de texte Qt de saisie de <I>int</I>.</P>
 *
 * <P>A partir de la version 2.42.0 de cette bibliothèque la validation
 * automatique peut être désactivée lors de la perte de focus ou frappe d'une
 * touche.
 * </P>
 */
class QtIntTextField : public QtValidatedTextField 
{
	public :

	/** Constructeur.
	 * @param		widget parent.
	 * @param		<I>true</I> s'il faut procéder à une validation automatique
	 * 				lors de la perte de focus ou d'une frappe clavier.
	 * @param		nom du widget.
	 */
	QtIntTextField (
		QWidget* parent, bool autoValidate = false, const char* name = 0);

	/** Constructeur.
	 * @param		valeur initiale.
	 * @param		widget parent.
	 * @param		<I>true</I> s'il faut procéder à une validation automatique
	 * 				lors de la perte de focus ou d'une frappe clavier.
	 * @param		nom du widget.
	 * @warning		<B>ATTENTION : transmettre les valeurs initiales non pas
	 *				via le constructeur, mais via une méthode <I>setX</I> (ex :
	 *				<I>setValue</I>). En effet, le validateur n'étant pas
	 *				construit, le texte initial peut être jugé invalide pour des
	 *				questions de formatage (nombre de décimales trop important
	 *				par exemple). Une fois le validateur construit un appel à
	 *				<I>setX</I> prend en compte ces aspects formatage et la
	 *				valeur affichée est valide.</B>
	 */
	QtIntTextField(
				int value, QWidget* parent, bool autoValidate = false, 
				const char* name = 0);

	/** Destructeur.
	 */
	virtual ~QtIntTextField ( );

	/**
	 * @return		<I>true</I>.
	 */
	virtual bool isNumeric ( ) const;

	/** 
	 * @return		la valeur
	 * @exception	lève une exception si la valeur est invalide.
	 */
	virtual int getValue ( ) const;

	/**
	 * @param		nouvelle valeur.
	 * @exception	lève une exception si la valeur est invalide.
	 * @see			setRange
	 */	
	virtual void setValue (int value);

	/**
	 * @param		En retour, valeur minimale autorisée
	 * @param		En retour, valeur maximale autorisée
	 */
	virtual void getRange (int& min, int& max) const;

	/**
	 * @param		valeur minimale autorisée
	 * @param		valeur maximale autorisée
	 * @exception	lève une exception en cas de domaine incohérent.
	 */
	virtual void setRange (int min, int max);


	protected :

	/**
	 * Créé l'IHM.
	 */
	virtual void createGui ( );

	/**
	 * @return		Le validateur utilisé.
	 */
	const virtual QIntValidator& getValidator ( ) const;

	/**
	 * Valide le contenu du champ de texte. En cas d'erreur affiche un message
	 * d'erreur. Ne lève pas d'exception.
	 * @return		true si le champ de texte est valide, false dans le cas
	 *				contraire.
	 */
	virtual bool validate ( );


	private :

	/** Constructeur de copie. Interdit. */
	QtIntTextField (const QtIntTextField&);

	/** Opérateur de copie. Interdit. */
	QtIntTextField& operator = (const QtIntTextField&);
};	// class QtIntTextField



#endif	// QT_INT_TEXT_FIELD_H
