#ifndef QT_DOUBLE_MATRIX_DIALOG_H
#define QT_DOUBLE_MATRIX_DIALOG_H

#include <TkUtil/util_config.h>
#include <QtUtil/QtMatrixDialog.h>

#include <QValidator>


/**
 * Classe de boite de dialogue modale de saisie des paramètres
 * définissant une matrice de <I>double</I>.
 */
class QtDoubleMatrixDialog : public QtMatrixDialog
{
	public	:

	/**
	 * Constructeur. Délègue à la classe <I>QtDlgClosurePanel</I> l'éventuelle
	 * aide contextuelle.
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		Nombre de rangées de la matrice (> 0)
	 * @param		Nombre de colonnes de la matrice (> 0)
	 * @param		Valeurs initiales de la matrice, ou 0
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 * @see			QtDlgClosurePanel
	 */
	QtDoubleMatrixDialog (
					QWidget* parent, const IN_UTIL UTF8String& title, 
					size_t rows, size_t columns, const double* const* matrix,
					const IN_STD string& helpURL = "",
					const IN_STD string& helpTag = "");

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtDoubleMatrixDialog ( );

	/**
	 * @return		La matrice
	 */
	virtual const double* const* getMatrix ( ) const;

	/**
	 * @param		La matrice, en retour
	 */
	virtual void getMatrix (double*const*const matrix) const;

	/**
	 * @return		La valeur dans la cellule (i, j) de la matrice.
	 * @exception	Lève une exception en cas d'arguments invalides.
	 */
	virtual double getValue (size_t i, size_t j) const;

	/**
	 * @return		Le validateur Qt utilisé par toutes les cellules de la
	 *				matrice.
	 */
	QDoubleValidator& getValidator ( ) const;


	private	:

	/** 
	 * Constructeur de copie : interdit.
	 */
	QtDoubleMatrixDialog (const QtDoubleMatrixDialog&);

	/** 
	 * Opérateur = : interdit.
	 */
	QtDoubleMatrixDialog& operator = (const QtDoubleMatrixDialog&);

	/** La matrice.  */
	double**			_matrix;

	/** Le validateur. */
	QDoubleValidator*	_validator;
};	// class QtDoubleMatrixDialog

#endif	// QT_DOUBLE_MATRIX_DIALOG_H
