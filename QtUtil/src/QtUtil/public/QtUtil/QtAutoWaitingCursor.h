#ifndef QT_AUTO_WAITING_CURSOR_H
#define QT_AUTO_WAITING_CURSOR_H

#ifndef QT_5
#include <QtGui/QApplication>
#else	// QT_5
#include <QApplication>
#endif	// QT_5
#include <QCursor>


/**
 * <P>Classe affichant un curseur d'attente dans une application Qt. Le curseur
 * initial est restauré automatiquement à la destruction de l'instance.
 * </P>
 * @warning		Sous Qt 4, seules les utilisations depuis le <I>thread</I>
 *				principal sont acceptées.
 * @warning		Comportement indéfini sous Qt 3 en dehors du thread principal.
 */
class QtAutoWaitingCursor
{
	public :

	/** Constructeur : affiche le curseur d'attente si show vaut true.
	 */
	QtAutoWaitingCursor (bool show);

	/** Destructeur : masque le curseur d'attente.
	 */
	virtual ~QtAutoWaitingCursor ( );

	/**
	 * Affiche le curseur d'attente si celui-ci n'est pas déjà affiché.
	 */
	virtual void show ( );

	/**
	 * Masque le curseur d'attente si celui-ci n'est pas déjà masqué.
	 */
	virtual void hide ( );


	private :

	/** Constructeur de copie : interdit. */
	QtAutoWaitingCursor (const QtAutoWaitingCursor&)
	{ }

	/** Opérateur = : interdit. */
	QtAutoWaitingCursor& operator = (const QtAutoWaitingCursor&)
	{ return *this; }

	/** Le curseur est-il affiché ? */
	bool				_shown;
};	// class QtAutoWaitingCursor


#endif	// QT_AUTO_WAITING_CURSOR_H
