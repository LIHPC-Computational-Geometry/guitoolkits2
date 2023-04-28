#ifndef QT_MIN_MAX_DIALOG_H
#define QT_MIN_MAX_DIALOG_H

#include <TkUtil/util_config.h>

#include <QtUtil/QtDlgClosurePanel.h>
#include <QtUtil/QtDoubleTextField.h>
#include <QtUtil/QtIntTextField.h>


/**
 * <P>Boîte de dialogue de saisie des valeurs extrêmales d'un domaine.
 * Les types <I>QtDoubleMinMaxDialog</I> et <I>QtIntMinMaxDialog</I>
 * définissent ce type ed boîte ed dialogue pour la saisie respectivement de
 * domaine de type <I>double</I> et <I>int</I>.
 * </P>
 * <P>Cette boite de dialogue peut utiliser le système d'aide en ligne proposé
 * par la classe <I>QtHelpWindow</I>.
 * </P>
 */
template <class T, class TF> class QtMinMaxDialog : public QDialog
{
	public :

	/** Constructeur. 
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		titre de l'application.
	 * @param		titre de la boîte de dialogue.
	 * @param		libellé de la valeur minimale
	 * @param		libellé de la valeur maximale
	 * @param		Valeur minimale proposée par défaut
	 * @param		Valeur maximale proposée par défaut
	 * @param		Caractère modal de la boite de dialogue.
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 */
	QtMinMaxDialog (
		QWidget* parent, const IN_STD string& appTitle,
		const IN_STD string& title,
		const QString& minLabel, const QString& maxLabel,
		T min, T max, bool modal = true,
		const IN_STD string& helpURL ="", const IN_STD string& helpTag = "");

	/** Destructeur. */
	virtual ~QtMinMaxDialog ( );

	/**
	 * @param		Les valeurs extrêmales que peut prendre la valeur minimale
	 *				à saisir.
	 * @exception	lève une exception en cas de domaine incohérent.
	 */
	virtual void setMinRange (T min, T max);

	/**
	 * @param		Les valeurs extrêmales que peut prendre la valeur maximale
	 *				à saisir.
	 * @exception	lève une exception en cas de domaine incohérent.
	 */
	virtual void setMaxRange (T min, T max);

	/**
	 * Invoqué lorsque l'utilisateur clique sur le bouton "OK". Effectue des
	 * vérifications préalables en cas d'acceptation de l'utilisateur
	 * (min < max).
	 */
	virtual void accept ( );

	/**
	 * @return		La valeur minimum
	 */
	virtual T getMin ( ) const;

	/**
	 * @return		La valeur maximum
	 */
	virtual T getMax ( ) const;


	private :


	/** Constructeur de copie. Interdit. */
	QtMinMaxDialog (const QtMinMaxDialog&);

	/** Opérateur de copie. Interdit. */
	QtMinMaxDialog& operator = (const QtMinMaxDialog&);

	/** Le champ de saisie de la valeur minimum. */
	TF*									_minTextField;

	/** Le champ de saisie de la valeur maximum. */
	TF*									_maxTextField;

	/** Le panneau "appliquer", "fermer", ... */
	QtDlgClosurePanel*					_closurePanel;

	/** Le titre de l'application. */
	IN_STD string						_appTitle;
};	// class QtMinMaxDialog


typedef QtMinMaxDialog<double, QtDoubleTextField>	QtDoubleMinMaxDialog;
typedef QtMinMaxDialog<int, QtDoubleTextField>		QtIntMinMaxDialog;


#include "QtUtil/QtMinMaxDialog_impl.h"

#endif	// QT_MIN_MAX_DIALOG_H
