#ifndef QT_UTIL_INFOS_H
#define QT_UTIL_INFOS_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>
#include <TkUtil/Version.h>


/*!
 * \mainpage    Page principale de la bibliothèque QtUtil.
 *
 * \section presentation    Présentation
 * <P>La bibliothèque <I>QtUtil</I> propose des classes génériques de panneaux,
 * boites de dialogues, affichage des traces d'un processus et une passerelle
 * chaine de caractères unicode de la bibliothèque  TkUtil avec les
 * <I>QString</I>.</P>
 * <P>Parmi les classes de cette bibliothèque, on trouve :
 * <OL>
 * <LI>Un gestionnaire de <B>sablier d'attente</B> (<I>QtAutoWaitingCursor</I>),
 * <LI>Un panneau (<I>Qt3DDataPanel</I>) et une boite de dialogue
 * (<I>QtCoordinatesDialog</I>) d'édition de <B>composantes 3D</B>,
 * <LI>Un <B>éditeur de matrice</B> (QtMatrixDialog),
 * <LI>Une fenêtre d'<B>aide en ligne</B>,
 * <LI>Une boite de dialogue de <B>sélection d'une liste de chaînes de
 * caractères</B>,
 * <LI>Une classe de <B>gestion du verrouillage</B> d'un widget (ex : menu),
 * <LI>Une classe de <B>widget texte labelisé</B>,
 * <LI>Deux classes d'affichage des <I>traces (logs)</I> d'une application
 * reposant sur les logs de la bibliothèque, <I>QtLogsView</I> et 
 * <I>QtStatusLogOutputStream</I>, TkUtil
 * </OL>
 * </P>
 *
 * \section depend      Dépendances
 * Cette bibliothèque utilise la bibliothèque TkUtil.
 * Les directives de compilation et d'édition des liens de la bibliothèque
 * <I>TkUtil</I> s'appliquent également à celle-ci.
 */

/**
 * Classe permettant de connaître la version de cette bibliothèque.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class QtUtilInfos
{
	public :

	/**
	 * @return		La version de cette bibliothèque
	 */
	static const IN_UTIL Version& getVersion ( );

	/**
	 * @return		La version détaillée de cette bibliothèque
	 * @since		5.5.0
	 */
	static IN_UTIL UTF8String getDetailedVersion ( );


	protected :

	/**
	 * Constructeurs, opérateur = et destructeur : interdits.
	 */
	QtUtilInfos ( );
	QtUtilInfos (const QtUtilInfos& exc);
	QtUtilInfos& operator = (const QtUtilInfos& exc);
	~QtUtilInfos ( );


	private :

	/**
	 * La version de cette bibliothèque.
	 */
	static const IN_UTIL Version		_version;
};	// class QtUtilInfos


#endif	// QT_UTIL_INFOS_H
