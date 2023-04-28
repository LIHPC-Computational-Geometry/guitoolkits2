#ifndef QT_ABOUT_DIALOG_H
#define QT_ABOUT_DIALOG_H

#ifndef QT_5
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#else	// QT_5
#include <QDialog>
#include <QVBoxLayout>
#endif	// QT_5
#include <TkUtil/UTF8String.h>



/**
 * Boite de dialogue d'affichage d'informations sur les composants utilisés par
 * une application.
 * Affiche le nom de l'application, sa version, diverses informations, 
 * les bibliothèques utilisées, ...
 */
class QtAboutDialog : public QDialog 
{
	Q_OBJECT

	public :

	/** Constructeur
	 * @param		parent
	 * @param		titre de l'application
	 * @param		version de l'application
	 * @param		site www de l'application
	 * @param		Description de l'application
	 */
	QtAboutDialog (QWidget* parent,
	               const TkUtil::UTF8String& title,
	               const std::string& version,
	               const std::string& url,
	               const TkUtil::UTF8String& description);

	/** Destructeur.
	 */
	virtual ~QtAboutDialog ( );

	/**
	 * Ajoute l'auteur de l'application transmis en argument.
	 */
	virtual void addAuthor (const TkUtil::UTF8String& author);

	/**
	 * Ajoute la bibliothèque transmise en argument comme composant de
	 * l'application.
	 */
	virtual void addComponent (
					const TkUtil::UTF8String& name, const std::string& version,
					const TkUtil::UTF8String& what, const std::string& url);

	struct	Component
	{
		TkUtil::UTF8String		_name, _what;
		std::string				_version, _url;
		Component (const TkUtil::UTF8String& name, const std::string& version,
		           const TkUtil::UTF8String& what, const std::string& url);
		Component (const Component&);
		Component& operator = (const Component&);
	};	// struct  Component


	protected :

	/**
	 * Construit si nécessaire la boite de dialogue.
	 */
	virtual void showEvent (QShowEvent* event);


	private :

	/** Constructeur de copie. Interdit. */
	QtAboutDialog (const QtAboutDialog&);

	/** Opérateur de copie. Interdit. */
	QtAboutDialog& operator = (const QtAboutDialog&);

	QVBoxLayout*						_layout;
	TkUtil::UTF8String					_name, _description;
	std::string							_version, _url;
	std::vector< TkUtil::UTF8String >	_authors;
	std::vector <Component>				_components;
};	// class QtAboutDialog



#endif	// QT_ABOUT_DIALOG_H
