#ifndef QT_CHARSET_COMBO_BOX_H
#define QT_CHARSET_COMBO_BOX_H

#include <TkUtil/UTF8String.h>
#include <QComboBox>

/**
 * Classe de boite à défilement permettant de choisir un jeu de caractères.
 */
class QtCharsetComboBox : public QComboBox
{
	public :

	/**
	 * Constructeur 1.
	 * @param	Widget parent
	 * @param	Jeu de caractères proposé par défaut.
	 */
	QtCharsetComboBox (
		QWidget* parent, TkUtil::Charset::CHARSET defaultCharset);

	/**
	 * Constructeur 2.
	 * @param	Widget parent
	 * @param	Jeux de caractères proposés.
	 * @param	Jeu de caractères proposé par défaut.
	 * @warning	Ne lève pas d'exception si le jeu de caractères par défaut n'est
	 * 			pas proposé.
	 */
	QtCharsetComboBox (
		QWidget* parent, const std::vector<TkUtil::Charset::CHARSET>& charsets,
		TkUtil::Charset::CHARSET defaultCharset);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~QtCharsetComboBox ( );

	/**
	 * @return	Le jeu de caractères retenu.
	 */
	virtual TkUtil::Charset::CHARSET getCharset ( ) const;

	/**
	 * @param	Nouveau jeu de caractère retenu.
	 * @throw	Une exception est levée si ce jeu de caractères n'est pas
	 * 			proposé.
	 */
	virtual void setCharset (TkUtil::Charset::CHARSET charset);


	private :

	/**
	 * Constructeur de copie, opérateur = : interdits.
	 */
	QtCharsetComboBox (const QtCharsetComboBox&);
	QtCharsetComboBox& operator = (const QtCharsetComboBox&);
};	// class QtCharsetComboBox


/**
 * Classe de boite à défilement avec libellé permettant de choisir un jeu de
 * caractères.
 */
class QtLabelisedCharsetComboBox : public QWidget
{
	public :

	/**
	 * Constructeur 1.
	 * @param	Widget parent
	 * @param	Libellé
	 * @param	Jeu de caractères proposé par défaut.
	 */
	QtLabelisedCharsetComboBox (
		QWidget* parent, const TkUtil::UTF8String& label,
		TkUtil::Charset::CHARSET defaultCharset);

	/**
	 * Constructeur 2.
	 * @param	Widget parent
	 * @param	Libellé
	 * @param	Jeux de caractères proposés.
	 * @param	Jeu de caractères proposé par défaut.
	 * @warning	Ne lève pas d'exception si le jeu de caractères par défaut n'est
	 * 			pas proposé.
	 */
	QtLabelisedCharsetComboBox (
		QWidget* parent, const TkUtil::UTF8String& label,	
		const std::vector<TkUtil::Charset::CHARSET>& charsets,
		TkUtil::Charset::CHARSET defaultCharset);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~QtLabelisedCharsetComboBox ( );

	/**
	 * @return	Le jeu de caractères retenu.
	 */
	virtual TkUtil::Charset::CHARSET getCharset ( ) const;

	/**
	 * @param	Nouveau jeu de caractère retenu.
	 * @throw	Une exception est levée si ce jeu de caractères n'est pas
	 * 			proposé.
	 */
	virtual void setCharset (TkUtil::Charset::CHARSET charset);

	/**
	 * @return	Une référence sur la combo box utilisée.
	 */
	virtual const QtCharsetComboBox& getComboBox ( ) const;
	virtual QtCharsetComboBox& getComboBox ( );


	private :

	/**
	 * Constructeur de copie, opérateur = : interdits.
	 */
	QtLabelisedCharsetComboBox (const QtLabelisedCharsetComboBox&);
	QtLabelisedCharsetComboBox& operator = (const QtLabelisedCharsetComboBox&);

	QtCharsetComboBox*	_comboBox;
};	// class QtLabelisedCharsetComboBox

#endif	// QT_CHARSET_COMBO_BOX_H
