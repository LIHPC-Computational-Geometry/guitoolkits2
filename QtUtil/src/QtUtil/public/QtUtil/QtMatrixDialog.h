#ifndef QT_MATRIX_DIALOG_H
#define QT_MATRIX_DIALOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>
#include <QtUtil/QtDlgClosurePanel.h>

#ifndef QT_5
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#else	// QT_5
#include <QDialog>
#include <QLineEdit>
#endif	// QT_5


/**
 * Classe de base de boite de dialogue modale de saisie des paramètres
 * définissant une matrice.
 */
class QtMatrixDialog : public QDialog
{
	public	:

	/**
	 * Constructeur. Délègue à la classe <I>QtDlgClosurePanel</I> l'éventuelle
	 * aide contextuelle.
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		Nombre de rangées de la matrice (> 0)
	 * @param		Nombre de colonnes de la matrice (> 0)
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 * @see			QtDlgClosurePanel
	 */
	QtMatrixDialog (
					QWidget* parent, const IN_UTIL UTF8String& title, 
					size_t rows, size_t columns,
					const IN_STD string& helpURL = "",
					const IN_STD string& helpTag = "");

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtMatrixDialog ( );

	/**
	 * @return		Le nombre de rangées.
	 */
	virtual size_t getRowsNum ( ) const
	{ return _rows; }

	/**
	 * @return		Le nombre de colonnes.
	 */
	virtual size_t getColumnsNum ( ) const
	{ return _columns; }

	/**
	 * @return		la valeur textuelle en (i, j) de la matrice
	 * @exception	Lève une exception en cas d'arguments invalides.
	 */
	virtual QString getStringValue (size_t i, size_t j) const;


	protected :

	/**
	 * @return		l'instance de QLineEdit qui représente la cellule de la
	 *				matrice en (i, j).
	 * @exception	Lève une exception en cas d'arguments invalides.
	 */
	virtual QLineEdit& getEditor (size_t i, size_t j) const;


	private	:

	/** 
	 * Constructeur de copie : interdit.
	 */
	QtMatrixDialog (const QtMatrixDialog&);

	/** 
	 * Opérateur = : interdit.
	 */
	QtMatrixDialog& operator = (const QtMatrixDialog&);

	/**
	 * Les cases à cocher "Modifier". 
	 */
	QLineEdit***			_valuesTextField;

	/**
	 * Le panneau contenant les boutons "OK" / "Annuler".
	 */
	QtDlgClosurePanel*		_closurePanel;

	/** Le nombre de lignes et de colonnes. */
	size_t					_rows, _columns;
};	// class QtMatrixDialog

#endif	// QT_MATRIX_DIALOG_H
