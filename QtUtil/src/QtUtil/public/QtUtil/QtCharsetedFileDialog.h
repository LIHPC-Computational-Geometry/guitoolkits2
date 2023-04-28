#ifndef QT_CHARSETED_FILE_DIALOG_H
#define QT_CHARSETED_FILE_DIALOG_H

#include <TkUtil/util_config.h>
#include <QtUtil/QtCharsetComboBox.h>

#include <QFileDialog>

#include <string>


/**
 * Boite de dialogue Qt permettant de sélectionner un fichier et de spécifier
 * le jeu de caractères utilisé pour l'encoder.
 */
class QtCharsetedFileDialog : public QFileDialog 
{
	Q_OBJECT

	public :

	/**
	 * Constructeur 1.
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		titre de la boite de dialogue
	 * @param		Jeu de caractères à utiliser (cas écriture)
	 */
	QtCharsetedFileDialog (
		QWidget* parent, const TkUtil::UTF8String& title,
		TkUtil::Charset::CHARSET cs = TkUtil::Charset::UNKNOWN);

	/**
	 * Constructeur 2.
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		titre du panneau
	 * @param		Répertoire courant
	 * @param		Jeu de caractères à utiliser (cas écriture)
	 */
	QtCharsetedFileDialog (QWidget* parent, const TkUtil::UTF8String& title,
		const std::string&  directory,
		TkUtil::Charset::CHARSET cs = TkUtil::Charset::UNKNOWN);

	/**
	 * Constructeur 3.
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		titre de la boite de dialogue
	 * @param		Jeux de caractères proposés
	 * @param		Jeu de caractères à utiliser (cas écriture)
	 */
	QtCharsetedFileDialog (
		QWidget* parent, const TkUtil::UTF8String& title,
		const std::vector<TkUtil::Charset::CHARSET>& charsets,
		TkUtil::Charset::CHARSET cs = TkUtil::Charset::UNKNOWN);

	/**
	 * Constructeur 4.
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		titre du panneau
	 * @param		Répertoire courant
	 * @param		Jeux de caractères proposés
	 * @param		Jeu de caractères à utiliser (cas écriture)
	 */
	QtCharsetedFileDialog (QWidget* parent, const TkUtil::UTF8String& title,
		const std::string&  directory,
		const std::vector<TkUtil::Charset::CHARSET>& charsets,
		TkUtil::Charset::CHARSET cs = TkUtil::Charset::UNKNOWN);

	/** Destructeur. */
	virtual ~QtCharsetedFileDialog ( );

	/**
	 * @return		Le jeu de caractères à utiliser pour encoder le fichier.
	 */
	virtual TkUtil::Charset::CHARSET getCharset ( ) const;

	/**
	 * @return		La boite à défilement "jeux de caractères".
	 */
	virtual const QtCharsetComboBox& getCharsetComboBox ( ) const;
	virtual QtCharsetComboBox& getCharsetComboBox ( );


	protected :

	/**
	 * Création de l'extension.
	 * @param		type de rayons concerné
	 * @param		Jeu de caractères à utiliser par défaut
	 * @see			getExtension
	 */	
	virtual void createExtension (TkUtil::Charset::CHARSET cs);

	/**
	 * Création de l'extension.
	 * @param		type de rayons concerné
	 * @param		Jeux de caractères proposés
	 * @param		Jeu de caractères à utiliser par défaut
	 * @see			getExtension
	 */	
	virtual void createExtension (
						const std::vector<TkUtil::Charset::CHARSET>& charsets,
						TkUtil::Charset::CHARSET cs);

	/**
	 * @return		Le widget "extension" de la boite de dialogue, dans laquelle
	 *				figure la boite à défilement "jeu de caractères". On peut
	 *				y ajouter d'autres widgets. Le layout est de type
	 *				QVBoxLayout.
	 */
	virtual QWidget& getExtension ( );


	private :

	/** Constructeur de copie. Interdit. */
	QtCharsetedFileDialog (const QtCharsetedFileDialog&);

	/** Opérateur de copie. Interdit. */
	QtCharsetedFileDialog& operator = (const QtCharsetedFileDialog&);

	/** Le jeu de caractère à utiliser pour l'encodage du fichier. */
	QtLabelisedCharsetComboBox*		_charsetComboBox;

	/** L'extension au sélecteur de fichiers Qt. */
	QWidget*						_extensionWidget;
};	// class QtCharsetedFileDialog



#endif	// QT_CHARSETED_FILE_DIALOG_H
