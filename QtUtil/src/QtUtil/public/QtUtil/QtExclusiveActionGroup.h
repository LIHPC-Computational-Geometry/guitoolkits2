// NO_QT3_MOC  Tag utilisé par le filtre MocFilter.py


#ifndef QT_EXCLUSIVE_ACTION_GROUP_H
#define QT_EXCLUSIVE_ACTION_GROUP_H

#ifndef QT_5
#include <QtGui/QActionGroup>
#else	// QT_5
#include <QActionGroup>
#endif	// QT_5



/**
 * Classe de <I>QAction</I> gérant des actions dont une et une seule
 * est active, à la façon des boutons "radios". L'icône de l'action active
 * est celle de l'instance.
 */
class QtExclusiveActionGroup : public QAction
{
	Q_OBJECT

	public :

	/** Constructeur
	 * @param		parent.
	 */
	QtExclusiveActionGroup (QObject* parent);

	/** Destructeur.
	 */
	virtual ~QtExclusiveActionGroup ( );

	/**
	 * Ajoute une action dont l'icône et le texte sont tranmis en arguments.
	 */
	virtual void addAction (const QIcon& icon, const QString& text);

	/**
	 * @return		L'index de l'action active, ou -1 s'il n'y en a pas.
	 * @see			setCurrentIndex
	 */
	virtual int currentIndex ( ) const
	{ return _currentAction; }

	/**
	 * Active l'action dont l'index est transmis en argument.
	 * @see		currentIndex
	 */
	virtual void setCurrentIndex (int index);


	signals :

	/**
	 * Signal émis lorsque l'action courante change.
	 * @param		Index de la nouvelle action courrante.
	 */
	void activated (int index);


	protected slots :

	/**
	 * Appellé lorsque l'action courrante change. Emet le signal activated.
	 */
	virtual void currentSelectionCallback (QAction*);


	private :

	/** Constructeur de copie. Interdit. */
	QtExclusiveActionGroup (const QtExclusiveActionGroup&);

	/** Opérateur de copie. Interdit. */
	QtExclusiveActionGroup& operator = (const QtExclusiveActionGroup&);

	QActionGroup*			_actionGroup;
	int						_currentAction;
};	// class QtExclusiveActionGroup


#endif	// QT_EXCLUSIVE_ACTION_GROUP_H

