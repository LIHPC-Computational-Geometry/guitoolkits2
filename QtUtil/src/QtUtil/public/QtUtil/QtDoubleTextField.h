#ifndef QT_DOUBLE_TEXT_FIELD_H
#define QT_DOUBLE_TEXT_FIELD_H

#include <QtUtil/QtValidatedTextField.h>


#include <QDoubleValidator>


/**
 * <P>Editeur de texte Qt de saisie de <I>double</I>.
 * Le nombre de décimales est fixé par défaut à 8.</P>
 *
 * <P>A partir de la version 2.42.0 de cette bibliothèque la validation
 * automatique peut être désactivée lors de la perte de focus ou frappe d'une
 * touche.
 * </P>
 */
class QtDoubleTextField : public QtValidatedTextField 
{
	public :

	/** Constructeur.
	 * @param		widget parent.
	 * @param		<I>true</I> s'il faut procéder à une validation automatique
	 * 				lors de la perte de focus ou d'une frappe clavier.
	 * @param		nom du widget.
	 */
	QtDoubleTextField (
			QWidget* parent, bool autoValidation = false, const char* name = 0);

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
	QtDoubleTextField (
					double value, QWidget* parent, bool autoValidation = false,
					const char* name = 0);

	/** Destructeur.
	 */
	virtual ~QtDoubleTextField ( );

	/**
	 * @return		<I>true</I>.
	 */
	virtual bool isNumeric ( ) const;

	/** 
	 * @return		la valeur
	 * @exception	lève une exception si la valeur est invalide.
	 */
	virtual double getValue ( ) const;

	/**
	 * @param		nouvelle valeur.
	 * @exception	lève une exception si la valeur est invalide.
	 * @see			setRange
	 */	
	virtual void setValue (double value);

	/**
	 * @param		En retour, valeur minimale autorisée
	 * @param		En retour, valeur maximale autorisée
	 */
	virtual void getRange (double& min, double& max) const;

	/**
	 * @param		valeur minimale autorisée
	 * @param		valeur maximale autorisée
	 * @exception	lève une exception en cas de domaine incohérent.
	 */
	virtual void setRange (double min, double max);

	/**
	 * @return		Précision numérique souhaitée.
	 */
	virtual unsigned short getDecimals ( ) const;

	/**
	 * @param		Précision numérique souhaitée.
	 */
	virtual void setDecimals (unsigned short num);

	/**
	 * @return		La notation utilisée.
	 */
	virtual QDoubleValidator::Notation getNotation ( ) const;

	/**
	 * @param		La notation utilisée.
	 */
	virtual void setNotation (QDoubleValidator::Notation notation);


	protected :

	/**
	 * Créé l'IHM.
	 */
	virtual void createGui ( );

	/**
	 * @return		Le validateur utilisé.
	 */
	const virtual QDoubleValidator& getValidator ( ) const;

	/**
	 * Créé une copie du validateur courrant et la retourne.
	 */
	virtual QDoubleValidator* cloneValidator ( );

	/**
	 * Valide le contenu du champ de texte. En cas d'erreur affiche un message
	 * d'erreur. Ne lève pas d'exception.
	 * @return		true si le champ de texte est valide, false dans le cas
	 *				contraire.
	 */
	virtual bool validate ( );


	private :

	/** Constructeur de copie. Interdit. */
	QtDoubleTextField (const QtDoubleTextField&);

	/** Opérateur de copie. Interdit. */
	QtDoubleTextField& operator = (const QtDoubleTextField&);
};	// class QtDoubleTextField



#endif	// QT_DOUBLE_TEXT_FIELD_H
