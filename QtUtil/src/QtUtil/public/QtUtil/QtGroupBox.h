#ifndef QT_GROUP_BOX_H
#define QT_GROUP_BOX_H

#ifndef QT_5
#include <QtGui/QWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>
#else	// QT_5
#include <QWidget>
#include <QGroupBox>
#include <QLayout>
#endif	// QT_5



/**
 * Widget type "GroupBox" calculant sa taille préférée. Le gestionnaire
 * de "layout" à utiliser est à donner aux instances de cette classe.
 */
class QtGroupBox : public QGroupBox 
{
	public :

	/** Constructeur
	 * @param		widget parent.
	 * @param		nom du widget.
	 */
	QtGroupBox (QWidget* parent, const char* name = 0);

	/** Constructeur
	 * @param		titre du groupe.
	 * @param		widget parent.
	 * @param		nom du widget.
	 */
	QtGroupBox (const QString& title, QWidget* parent, const char* name = 0);

	/** Destructeur.
	 */
	virtual ~QtGroupBox ( );

	/** 
	 * @param		Gestionnaire de layout à utiliser.
	 * @warning		Attention, cette fonction est à appeler une seule fois.
	 */
	void setLayout (QLayout* layout);

	/**
	 * @param		l'épaisseur de la marge.
	 * @warning		Cette épaisseur est ajoutée à la hauteur de la
	 *				fonte.
	 * @warning		Appel efficace que si setLayout a été préalablement
	 *				invoqué.
	 */
	virtual void setMargin (int margin);

	/**
	 * @param		la distance entre chaque widget enfant.
	 * @warning		Appel efficace que si setLayout a été préalablement
	 *				invoqué.
	 */
	virtual void setSpacing (int margin);

	/**
	 * @return		La taille préférée de ce widget. 
	 * @warning		Attention : utilise getLayout.
	 */
	virtual QSize sizeHint ( ) const;

	/** S'assure que la taille du panneau sera en toute circonstances au moins
	 * assez grande.
	 * @warning		Fonction efficace à condition que 
	 *				setMinimumSize (widget->sizeHint ( )) ait été
	 *				invoqué pour chacun de ses enfants.
	 */
	virtual void adjustSize ( );


	protected :

	/** @return		Le gestionnaire de layout utilisé. */
	QLayout* getLayout ( ) const
	{ return _layout; }


	private :

	/** Constructeur de copie. Interdit. */
	QtGroupBox (const QtGroupBox&);

	/** Opérateur de copie. Interdit. */
	QtGroupBox& operator = (const QtGroupBox&);

	/** Le gestionnaire de "layout" utilisé. */
	QLayout*				_layout;
};	// class QtGroupBox



#endif	// QT_GROUP_BOX_H
