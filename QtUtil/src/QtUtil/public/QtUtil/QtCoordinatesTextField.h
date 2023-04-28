/**
 * @file		QtCoordinatesTextField.h
 * @author		Charles PIGNEROL
 * @date		15/03/2017
 */
#ifndef QT_COORDINATES_TEXFIELD_H
#define QT_COORDINATES_TEXFIELD_H

#include <QtUtil/QtValidatedTextField.h>


/**
 * Classe de champ de texte spécialisé dans la saisie de coordonnées sous la
 * forme <I>(réel, réel, réel)</I>. La notation des 3 réels peut être de type
 * scientifique, avec le e/E pour l'exposant.
 */
class QtCoordinatesTextField : public QtValidatedTextField
{
	public :

	/**
	 * Constructeur. RAS.
	 * @param	coordonnées initiales
	 * @param	précision attendue pour chacune des composantes (et donc
	 *			utilisée lors de l'affichage des coordonnées initiales, un
	 *			objectif étant de faire la ligne la plus courte possible).
	 */
	QtCoordinatesTextField (
					QWidget* parent, double x, double y, double z,
					unsigned short precision = 9);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~QtCoordinatesTextField ( );

	/**
	 * Accesseurs get/set aux coordonnées. RAS.
	 */
	virtual void getCoordinates (double& x, double& y, double& z) const;
	virtual void getCoordinates (double coords [3]) const;
	virtual void setCoordinates (double x, double y, double z);
	virtual void setCoordinates (const double coords [3]);


	protected :

	/**
	 * Appelé lorsque le champ perd le focus. Effectue une vérification de la
	 * saisie en cours et affiche un message en cas d'erreur.
	 */
	virtual bool validate ( );


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	QtCoordinatesTextField (const QtCoordinatesTextField&);
	QtCoordinatesTextField& operator = (const QtCoordinatesTextField&);

	unsigned short		_precision;
};	// class QtCoordinatesTextField


#endif	// QT_COORDINATES_TEXFIELD_H
