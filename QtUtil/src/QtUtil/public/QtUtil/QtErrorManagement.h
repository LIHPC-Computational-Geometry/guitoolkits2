/**
 * \file		QtErrorManagement.h
 * \author		Charles PIGNEROL
 * \date		29/11/2010
 */


#ifndef QT_ERROR_MANAGEMENT_H
#define QT_ERROR_MANAGEMENT_H

#include <QtUtil/QtMessageBox.h>
#include <TkUtil/Exception.h>

#include <QPalette>


/**
 * \brief	Macro définissant le début d'un block C++ try/catch en
 *			environnement <I>Qt</I> avec définition de deux variables,
 *			<I>hasError</I> et <I>errorString</I> simplifiant le traitement
 *			d'erreur en sortie du bloc try/catch. Cette macro est à utiliser
 *			conjointement avec la macro <I>QT_COMPLETE_TRY_CATCH_BLOCK</I>.
 * \see		QT_COMPLETE_TRY_CATCH_BLOCK
 * \see		RE_BEGIN_QT_TRY_CATCH_BLOCK
 */
#define BEGIN_QT_TRY_CATCH_BLOCK                                           \
	bool                hasError    = false;                               \
	string              errorString;                                       \
	try {


/**
 * \brief	Idem <I>BEGIN_QT_TRY_CATCH_BLOCK</I> mais pour un second (ou plus)
 * 			bloc try/catch. Ré-initialise les variables.
 * \see		BEGIN_QT_TRY_CATCH_BLOCK
 * \see		QT_COMPLETE_TRY_CATCH_BLOCK
 */
#define RE_BEGIN_QT_TRY_CATCH_BLOCK                                        \
	hasError	= false;                                                   \
	errorString.clear ( );                                                 \
	try {

/**
 * \brief	Macro définissant la fin d'un block C++ try/catch en environnement
 *			<I>Qt</I>.
 *			En cas d'exception attrapée, cette macro affecte <I>true</I> à
 *			la variable <I>hasError</I> et l'éventuel message d'erreur à la
 *			variable <I>errorString</I>. Le message d'erreur est affiché dans
 *			une boite de dialogue <I>Qt</I> de parent <I>window</I> et de titre
 *			<I>title</I> si <I>toMessageBox</I> vaut <I>true</I>.
 * \see		BEGIN_QT_TRY_CATCH_BLOCK
 * \see		QtMgx3DApplication::displayUpdatesErrors
 */
#define COMPLETE_QT_TRY_CATCH_BLOCK(toMessageBox, window, title)           \
	}                                                                      \
	catch (const TkUtil::Exception& exc)                                   \
	{                                                                      \
		errorString = exc.getFullMessage ( );                              \
		if (true == toMessageBox)                                          \
			QtMessageBox::displayErrorMessage (                            \
								window, title, exc.getFullMessage ( ));    \
		hasError    = true;                                                \
	}                                                                      \
	catch (const std::exception& stdExc)                                   \
	{                                                                      \
		errorString = stdExc.what ( );                                     \
		if (true == toMessageBox)                                          \
			QtMessageBox::displayErrorMessage (                            \
								window, title, stdExc.what ( ));           \
		hasError    = true;                                                \
	}                                                                      \
	catch (...)                                                            \
	{                                                                      \
		errorString = "Erreur non documentée.";                            \
		if (true == toMessageBox)                                          \
			QtMessageBox::displayErrorMessage (                            \
						window, title, "Erreur interne non documentée.");  \
		hasError    = true;                                                \
	}



/**
 * \brief	Petite macro pour la mise au point de panneaux. Permet de changer
 *			la couleur du widget transmis en argument. Ne marche peut être pas
 *			bien pour tous les types de widgets.
 */
#define SET_WIDGET_BACKGROUND(widget, color)                               \
{                                                                          \
	QPalette	palette	= widget->palette ( );                             \
	palette.setColor (QPalette::Active, QPalette::Window, color);          \
	widget->setPalette (palette);                                          \
	widget->setAutoFillBackground (true);                                  \
}


#endif	// QT_ERROR_MANAGEMENT_H
