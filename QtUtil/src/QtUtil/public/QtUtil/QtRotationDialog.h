#ifndef QT_ROTATION_DIALOG_H
#define QT_ROTATION_DIALOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>
#include <QtUtil/QtTextField.h>
#include <QtUtil/QtDlgClosurePanel.h>

#ifndef QT_5
#include <QtGui/QDialog>
#include <QtGui/QRadioButton>
#else	// QT_5
#include <QDialog>
#include <QRadioButton>
#endif	// QT_5


/**
 * Classe de boite de dialogue modale de saisie des paramètres définissant une
 * rotation dans un espace 3D.
 */
class QtRotationDialog : public QDialog
{
	public	:

	/**
	 * Constructeur. Délègue à la classe <I>QtDlgClosurePanel</I> l'éventuelle
	 * aide contextuelle.
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		Nom du premier axe de rotation.
	 * @param		Nom du second axe de rotation.
	 * @param		Nom du troisième axe de rotation.
	 * @param		Axe de rotation proposé par défaut. De 0 à 2 compris.
	 * @param		Valeur par défaut de l'angle.
	 * @param		Unité de l'angle.
	 * @param		Valeurs minimale et maximale de l'angle.
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 * @see			QtDlgClosurePanel
	 */
	QtRotationDialog (
					QWidget* parent, const IN_UTIL UTF8String& title, 
					const IN_UTIL UTF8String& axe1Label,
					const IN_UTIL UTF8String& axe2Label,
					const IN_UTIL UTF8String& axe3Label,
					unsigned char defaultAxe, double angle,
					const IN_UTIL UTF8String& angleUnit,
					double angleMin, double angleMax,
					const IN_STD string& helpURL = "",
					const IN_STD string& helpTag = "");

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtRotationDialog ( );

	/**
	 * @return		L'axe de rotation choisi (de 0 à 2 compris).
	 */
	virtual unsigned char getAxe ( ) const;

	/**
	 * @return		l'angle de rotation.
	 * @exception	Lève une exception en cas de valeur invalide.
	 */
	virtual double getAngle ( ) const;

	/**
	 * Demande une confirmation si l'angle de rotation est nul.
	 */
	virtual void accept ( );


	private	:

	/** 
	 * Constructeur de copie : interdit.
	 */
	QtRotationDialog (const QtRotationDialog&);

	/** 
	 * Opérateur = : interdit.
	 */
	QtRotationDialog& operator = (const QtRotationDialog&);

	/**
	 * Les cases à cocher "Modifier". 
	 */
	QRadioButton			*_axe1Button, *_axe2Button, *_axe3Button;

	/**
	 * La saisie des composantes.
	 */
	QtTextField*			_angleTextField;

	/**
	 * Le panneau contenant les boutons "OK" / "Annuler".
	 */
	QtDlgClosurePanel*		_closurePanel;
};	// class QtRotationDialog

#endif	// QT_ROTATION_DIALOG_H
