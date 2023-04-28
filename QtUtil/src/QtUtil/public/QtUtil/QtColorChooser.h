#ifndef QT_COLOR_CHOOSER_H
#define QT_COLOR_CHOOSER_H

#include <TkUtil/util_config.h>
#include <TkUtil/PaintAttributes.h>
#include <TkUtil/UTF8String.h>

#include <QLabel>


/**
 * Widget de sélection de couleur et affichant la couleur en vigueur.
 */
class QtColorChooser : public QWidget
{
	Q_OBJECT

	public :

	QtColorChooser (
		QWidget* parent, const TkUtil::UTF8String& label, const TkUtil::Color& color);
	QtColorChooser (
		QWidget* parent, const TkUtil::UTF8String& label, const QColor& color);
	virtual ~QtColorChooser ( );

	virtual TkUtil::Color getColor ( ) const;
	virtual void setColor (const TkUtil::Color& c);
	virtual QColor getQColor ( ) const;
	virtual void setQColor (const QColor& c);


	signals :

	/**
	 * Envoyé lorsque l'utilisateur change de couleur.
	 * @param	Nouvelle couleur
	 */
	void colorChanged (QColor);


	protected :

	virtual void createGui (const TkUtil::UTF8String& label, const TkUtil::Color& color);


	protected slots :

	virtual void clickedCallback ( );


	private :

	QtColorChooser (const QtColorChooser&);
	QtColorChooser& operator = (const QtColorChooser&);

	QLabel*		_colorLabel;
};	// class QtColorChooser


#endif	// QT_COLOR_CHOOSER_H
