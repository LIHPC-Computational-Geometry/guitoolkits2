#ifndef QT_CONFIGURATION_H
#define QT_CONFIGURATION_H


#include <Qt>


/**
 * Classe contenant des paramètres utiles à la création de composants 
 * Qt : marges, espaces entre 2 composants, ...
 */
class QtConfiguration
{
	public :

	/** Les marges. */
	static int			margin;

	/** L'espace entre les composants. */
	static int			spacing;

	/** Paramètres de création des boites de dialogues modales. C'est une
	 * combinaison de valeurs possibles de l'énuméré Qt::WidgetFlags. */
	static const Qt::WindowFlags	modalDialogWFlags;

	/** Paramètres de création des boites de dialogues non modales. C'est une
	 * combinaison de valeurs possibles de l'énuméré Qt::WidgetFlags. */
	static const Qt::WindowFlags	amodalDialogWFlags;

};

#endif	// QT_CONFIGURATION_H
