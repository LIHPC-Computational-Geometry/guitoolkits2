#ifndef QT_STRING_NUMBER_FORMAT_COMBOBOX_H
#define QT_STRING_NUMBER_FORMAT_COMBOBOX_H

#ifndef QT_5
#include <QtGui/QComboBox>
#else	// QT_5
#include <QComboBox>
#endif	// QT_5


/**
 * <P>Classe de boite à défilement permettant de sélectionner un format pour
 * les conversions nombre -> QString.
 * </P>
 * @see		QString::arg
 * @see		QString::number
 */
class QtStringNumberFormatComboBox : public QComboBox
{
	public :

	/**
	 * Constructeur par défaut.
	 * @param		Widget parent.
	 * @param		Format proposé par défaut
	 */
	QtStringNumberFormatComboBox (QWidget* parent, char format = 'g');

	/**
	 * Destructeur. RAS.
	 */
	virtual ~QtStringNumberFormatComboBox ( );

	/**
	 * @return		Le format en cours.
	 * @see			setFormat
	 */
	virtual char getFormat ( ) const;

	/**
	 * @param		Le nouveau format sélectionné.
	 * @see			getFormat
	 * @exception	Une exception est levée si le caractère transmis en argument
	 *				n'est pas valide
	 * @see			QString::arg
	 * @see			QString::number
	 */
	virtual void setFormat (char format);

	private :

	/** Constructeurs de copie interdits. */
	QtStringNumberFormatComboBox (const QtStringNumberFormatComboBox&);
	QtStringNumberFormatComboBox& operator = (
										const QtStringNumberFormatComboBox&);
};	// class QtStringNumberFormatComboBox

#endif	// QT_STRING_NUMBER_FORMAT_COMBOBOX_H
