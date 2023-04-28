#ifndef QT_VALIDATED_TEXT_FIELD_H
#define QT_VALIDATED_TEXT_FIELD_H

#include <QtUtil/QtTextField.h>


class QValidatedValidator;


/**
 * <P>Classe abstraite de saisie de texte mono-ligne avec validation par une
 * instance de <I>QValidator</I>. La validation est automatiquement effectuée
 * lors de la perte du focus ou lorsque l'utilisateur enfonce la touche
 * "return".
 * </P>
 *
 * <P>
 * <B>IMPORTANT :</B> pour bénéficier des services de cette classe, les classes
 * dérivées doivent impérativement associer à l'instance une instance d'une
 * classe dérivée de <I>QValidator</I>. Cette classe personnalise les messages
 * d'erreurs pour des validateurs de types <I>QIntValidator</I>,
 * <I>QDoubleValidator</I> et <I>QRegExpValidator</I>.
 * </P>
 * 
 * <P>A partir de la version 2.42.0 de cette bibliothèque la validation
 * automatique peut être désactivée lors de la perte de focus ou frappe d'une
 * touche.
 * </P>
 *
 * @see		QtDoubleTextField
 * @see		QtIntTextField
 */
class QtValidatedTextField : public QtTextField 
{
	public :

	/**
	 *  Destructeur.
	 */
	virtual ~QtValidatedTextField ( );

	/**
	 * <P>Si <I>true</I> ou si <I>automaticValidation ( )</I> retourne
	 * <I>true</I> provoque alors la validation automatique du champ de texte
	 * lors de la perte du focus ou d'une frappe clavier.</P>
	 *
	 * <P>Vaut <I>true</I> par défaut.</P>
	 */
	static bool	automaticValidation;

	/**
	 * <P>Si <I>true</I> une erreur lors de la validation provoque l'affichage
	 * d'une boite de dialogue.
	 *
	 * <P>Vaut <I>true</I> par défaut.</P>
	 */
	static bool	dialogOnError;

	/** Couleurs utilisées lorsque la saisie est invalide (blanc pour le fond,
	 * rouge pour le texte par défaut).
	 */
	static QColor	errorBackground, errorForeground;


	protected :

	/** Constructeur.
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
	QtValidatedTextField (
			QWidget* parent, bool autoValidate = false, const char* name = 0);

	/**
	 * @return		<I>true</I> si le widget procède à une autovalidation en cas
	 *				de perte de focus ou de frappe clavier, <I>false</I> dans
	 *				le cas contraire.
	 * @warning		La variable de classe <I>automaticValidation</I> est
	 * 				également prise en compte.
	 */
	virtual bool doAutoValidation ( ) const
	{ return _autoValidation; }

	/**
	 * @param		<I>true</I> si le widget doit procéder à une autovalidation
	 * 				en cas de perte de focus ou de frappe clavier, <I>false</I>
	 * 				dans le cas contraire.
	 * @warning		La variable de classe <I>automaticValidation</I> est
	 * 				également prise en compte.
	 */
	virtual void doAutoValidation (bool autoValidation)
	{ _autoValidation	= autoValidation; }

	/**
	 * Invoqué lors du gain du focus : invoque <I>setSkin (true)</I>.
	 * @see		validate
	 * @see		setSkin
	 */
	virtual void focusInEvent (QFocusEvent* event);

	/**
	 * Invoqué lors de la perte du focus : effectue une vérification des
	 * données saisies et affiche un message en cas d'erreur.
	 * @see		validate
	 */
	virtual void focusOutEvent (QFocusEvent* event);

	/**
	 * Invoqué lorsqu'une touche a été pressée : effectue une vérification des
	 * données saisies et affiche un message en cas d'erreur.
	 * @see		validate
	 */
	virtual void keyPressEvent (QKeyEvent* event);

	/**
	 * Valide le contenu du champ de texte. En cas d'erreur affiche un message
	 * d'erreur si <I>dialogOnError</I> vaut <I>true</I>, et modifie l'aspect
	 * du champ de saisie. Ne lève pas d'exception.
	 * @return		true si le champ de texte est valide, false dans le cas
	 *				contraire.
	 * @see			setSkin
	 * @see			dialogOnError
	 * @see			errorBackground
	 * @see			errorForeground
	 */
	virtual bool validate ( );

	/**
	 * Modifie l'apparence du champ de saisie (<I>true</I> : apparence normale,
	 * <I>false</I> : apparence en erreur).
	 * @see		validate
	 * @see			errorBackground
	 * @see			errorForeground
	 */
	virtual void setSkin (bool normal);


	private :

	/** Constructeur de copie. Interdit. */
	QtValidatedTextField (const QtValidatedTextField&);

	/** Opérateur de copie. Interdit. */
	QtValidatedTextField& operator = (const QtValidatedTextField&);

	bool			_autoValidation;
};	// class QtValidatedTextField



#endif	// QT_VALIDATED_TEXT_FIELD_H
