#ifndef QT_STRING_HELPER_H
#define QT_STRING_HELPER_H

#include <QString>
#include <QFont>



/**
 * <P>Classe utilitaire permettant de mélanger différentes polices de caractères
 * dans une instance de la classe QString. Ceci est par exemple utile lorsqu'une
 * chaîne, destinée à être affichée dans un menu ou dans un label, contient
 * des symbôles (ex : caractères grecs).</P>
 *
 * <P>Deux moyens sont possibles pour y arriver :<BR>
 * <OL>
 * <LI>Utilisation du format <I>rich text</I> de <I>Qt</I>, mais peu de classes
 * le supportent (parmi les classes qui ne le supportent pas : <I>QMenu</I>,
 * <I>QComboBox</I>, ...),
 * <LI>Utilisation du jeu de caractères <I>unicode</I> supporté naturellement
 * par la classe <I>QString</I>.
 * </OL>
 * </P>
 * 
 * @warning		Utilisable uniquement avec Qt 4, non testé sous Qt 3.
 */
class QtStringHelper
{
	public :

	/**
	 * A appeler avant tout usage des services de cette classe, initialise
	 * certaines variables.
	 * Requiert que l'application <I>Qt</I> soit elle même initialisée.
	 */
	static void initialize ( );

	/**
	 * A appeler en fin d'utilisation des services de cette classe. Libère les
	 * ressources utilisées.
	 */
	static void finalize ( );

	/**
	 * @param		texte que l'on souhaite obtenir en caractères grecs
	 *				(jeu unicode).
	 * @return		une chaîne de caractères au format unicode dont les
	 *				caractères ASCII ont été remplacés par des caractères grecs.
	 */
	static QString textToUnicodeGreekText (const QString& text);

	/**
	 * @param		texte dont on souhaite fixer la police de caractères.
	 * @param		police de caractères à appliquer à <I>text</I>.
	 * @return	une chaîne de caractères au format "rich text" Qt bordée
	 * des balises HTML définissant sa police de caractères.
	 */
	static QString textToRichText (
							const std::string& text, const std::string& font);

	/**
	 * @param		texte que l'on souhaite obtenir en caractères grecs.
	 * @return	une chaîne de caractères au format "rich text" Qt bordée
	 * des balises HTML définissant la police de caractères grecque.
	 */
	static QString textToGreekText (const std::string& text);

	/**
	 * Caractères grecs en minuscules et majuscules au format <I>unicode</I>.
	 * @warning		initialize doit être appellé au préalable.
	 */
	static const QString alphaMin ( )
	{ return _alphaMin; }
	static const QString alphaMaj ( )
	{ return _alphaMaj; }
	static const QString betaMin ( )
	{ return _betaMin; }
	static const QString betaMaj ( )
	{ return _betaMaj; }
	static const QString khiMin ( )
	{ return _khiMin; }
	static const QString khiMaj ( )
	{ return _khiMaj; }
	static const QString deltaMin ( )
	{ return _deltaMin; }
	static const QString deltaMaj ( )
	{ return _deltaMaj; }
	static const QString epsilonMin ( )
	{ return _epsilonMin; }
	static const QString epsilonMaj ( )
	{ return _epsilonMaj; }
	static const QString etaMin ( )
	{ return _etaMin; }
	static const QString etaMaj ( )
	{ return _etaMaj; }
	static const QString gammaMin ( )
	{ return _gammaMin; }
	static const QString gammaMaj ( )
	{ return _gammaMaj; }
	static const QString iotaMin ( )
	{ return _iotaMin; }
	static const QString iotaMaj ( )
	{ return _iotaMaj; }
	static const QString phiMin ( )
	{ return _phiMin; }
	static const QString phiMaj ( )
	{ return _phiMaj; }
	static const QString kappaMin ( )
	{ return _kappaMin; }
	static const QString kappaMaj ( )
	{ return _kappaMaj; }
	static const QString lambdaMin ( )
	{ return _lambdaMin; }
	static const QString lambdaMaj ( )
	{ return _lambdaMaj; }
	static const QString muMin ( )
	{ return _muMin; }
	static const QString muMaj ( )
	{ return _muMaj; }
	static const QString nuMin ( )
	{ return _nuMin; }
	static const QString nuMaj ( )
	{ return _nuMaj; }
	static const QString omicronnMin ( )
	{ return _omicronnMin; }
	static const QString omicronnMaj ( )
	{ return _omicronnMaj; }
	static const QString piMin ( )
	{ return _piMin; }
	static const QString piMaj ( )
	{ return _piMaj; }
	static const QString thetaMin ( )
	{ return _thetaMin; }
	static const QString thetaMaj ( )
	{ return _thetaMaj; }
	static const QString roMin ( )
	{ return _roMin; }
	static const QString roMaj ( )
	{ return _roMaj; }
	static const QString sigmaMin ( )
	{ return _sigmaMin; }
	static const QString sigmaMaj ( )
	{ return _sigmaMaj; }
	static const QString tauMin ( )
	{ return _tauMin; }
	static const QString tauMaj ( )
	{ return _tauMaj; }
	static const QString upsilonMin ( )
	{ return _upsilonMin; }
	static const QString upsilonMaj ( )
	{ return _upsilonMaj; }
	static const QString omegaMin ( )
	{ return _omegaMin; }
	static const QString omegaMaj ( )
	{ return _omegaMaj; }
	static const QString xsiMin ( )
	{ return _xsiMin; }
	static const QString xsiMaj ( )
	{ return _xsiMaj; }
	static const QString psiMin ( )
	{ return _psiMin; }
	static const QString psiMaj ( )
	{ return _psiMaj; }
	static const QString dzetaMin ( )
	{ return _dzetaMin; }
	static const QString dzetaMaj ( )
	{ return _dzetaMaj; }


	/**
	 * Caractères grecs en minuscules et majuscules au format
	 * <I>rich text</I>..
	 * @warning		initialize doit être appellé au préalable.
	 */
	static const QString richTextAlphaMin ( )
	{ return _alphaMinRichText; }
	static const QString richTextAlphaMaj ( )
	{ return _alphaMajRichText; }
	static const QString richTextBetaMin ( )
	{ return _betaMinRichText; }
	static const QString richTextBetaMaj ( )
	{ return _betaMajRichText; }
	static const QString richTextKhiMin ( )
	{ return _khiMinRichText; }
	static const QString richTextKhiMaj ( )
	{ return _khiMajRichText; }
	static const QString richTextDeltaMin ( )
	{ return _deltaMinRichText; }
	static const QString richTextDeltaMaj ( )
	{ return _deltaMajRichText; }
	static const QString richTextEpsilonMin ( )
	{ return _epsilonMinRichText; }
	static const QString richTextEpsilonMaj ( )
	{ return _epsilonMajRichText; }
	static const QString richTextEtaMin ( )
	{ return _etaMinRichText; }
	static const QString richTextEtaMaj ( )
	{ return _etaMajRichText; }
	static const QString richTextGammaMin ( )
	{ return _gammaMinRichText; }
	static const QString richTextGammaMaj ( )
	{ return _gammaMajRichText; }
	static const QString richTextIotaMin ( )
	{ return _iotaMinRichText; }
	static const QString richTextIotaMaj ( )
	{ return _iotaMajRichText; }
	static const QString richTextPhiMin ( )
	{ return _phiMinRichText; }
	static const QString richTextPhiMaj ( )
	{ return _phiMajRichText; }
	static const QString richTextKappaMin ( )
	{ return _kappaMinRichText; }
	static const QString richTextKappaMaj ( )
	{ return _kappaMajRichText; }
	static const QString richTextLambdaMin ( )
	{ return _lambdaMinRichText; }
	static const QString richTextLambdaMaj ( )
	{ return _lambdaMajRichText; }
	static const QString richTextMuMin ( )
	{ return _muMinRichText; }
	static const QString richTextMuMaj ( )
	{ return _muMajRichText; }
	static const QString richTextNuMin ( )
	{ return _nuMinRichText; }
	static const QString richTextNuMaj ( )
	{ return _nuMajRichText; }
	static const QString richTextOmicronnMin ( )
	{ return _omicronnMinRichText; }
	static const QString richTextOmicronnMaj ( )
	{ return _omicronnMajRichText; }
	static const QString richTextPiMin ( )
	{ return _piMinRichText; }
	static const QString richTextPiMaj ( )
	{ return _piMajRichText; }
	static const QString richTextThetaMin ( )
	{ return _thetaMinRichText; }
	static const QString richTextThetaMaj ( )
	{ return _thetaMajRichText; }
	static const QString richTextRoMin ( )
	{ return _roMinRichText; }
	static const QString richTextRoMaj ( )
	{ return _roMajRichText; }
	static const QString richTextSigmaMin ( )
	{ return _sigmaMinRichText; }
	static const QString richTextSigmaMaj ( )
	{ return _sigmaMajRichText; }
	static const QString richTextTauMin ( )
	{ return _tauMinRichText; }
	static const QString richTextTauMaj ( )
	{ return _tauMajRichText; }
	static const QString richTextUpsilonMin ( )
	{ return _upsilonMinRichText; }
	static const QString richTextUpsilonMaj ( )
	{ return _upsilonMajRichText; }
	static const QString richTextOmegaMin ( )
	{ return _omegaMinRichText; }
	static const QString richTextOmegaMaj ( )
	{ return _omegaMajRichText; }
	static const QString richTextXsiMin ( )
	{ return _xsiMinRichText; }
	static const QString richTextXsiMaj ( )
	{ return _xsiMajRichText; }
	static const QString richTextPsiMin ( )
	{ return _psiMinRichText; }
	static const QString richTextPsiMaj ( )
	{ return _psiMajRichText; }
	static const QString richTextDzetaMin ( )
	{ return _dzetaMinRichText; }
	static const QString richTextDzetaMaj ( )
	{ return _dzetaMajRichText; }

	/**
	 * @return		une police de caractère dotée des caractères grecs, si
	 *				possible.
	 */
	static const QFont& greekFont ( );


	private :

	/** Constructeurs et destructeur interdits. */
	QtStringHelper ( );
	QtStringHelper (const QtStringHelper&);
	QtStringHelper& operator = (const QtStringHelper&);
	virtual ~QtStringHelper ( )
	{ }

	/** Une police de caractère dotée des caractères grecs, si possible. */
	static QFont			_greekFont;

	/** Les caractères grecs (format <I>unicode</I>). */
	static QString _alphaMin;
	static QString _alphaMaj;
	static QString _betaMin;
	static QString _betaMaj;
	static QString _khiMin;
	static QString _khiMaj;
	static QString _deltaMin;
	static QString _deltaMaj;
	static QString _epsilonMin;
	static QString _epsilonMaj;
	static QString _etaMin;
	static QString _etaMaj;
	static QString _gammaMin;
	static QString _gammaMaj;
	static QString _iotaMin;
	static QString _iotaMaj;
	static QString _phiMin;
	static QString _phiMaj;
	static QString _kappaMin;
	static QString _kappaMaj;
	static QString _lambdaMin;
	static QString _lambdaMaj;
	static QString _muMin;
	static QString _muMaj;
	static QString _nuMin;
	static QString _nuMaj;
	static QString _omicronnMin;
	static QString _omicronnMaj;
	static QString _piMin;
	static QString _piMaj;
	static QString _thetaMin;
	static QString _thetaMaj;
	static QString _roMin;
	static QString _roMaj;
	static QString _sigmaMin;
	static QString _sigmaMaj;
	static QString _tauMin;
	static QString _tauMaj;
	static QString _upsilonMin;
	static QString _upsilonMaj;
	static QString _omegaMin;
	static QString _omegaMaj;
	static QString _xsiMin;
	static QString _xsiMaj;
	static QString _psiMin;
	static QString _psiMaj;
	static QString _dzetaMin;
	static QString _dzetaMaj;

	/** Les caractères grecs (format <I>rich text</I>). */
	static QString _alphaMinRichText;
	static QString _alphaMajRichText;
	static QString _betaMinRichText;
	static QString _betaMajRichText;
	static QString _khiMinRichText;
	static QString _khiMajRichText;
	static QString _deltaMinRichText;
	static QString _deltaMajRichText;
	static QString _epsilonMinRichText;
	static QString _epsilonMajRichText;
	static QString _etaMinRichText;
	static QString _etaMajRichText;
	static QString _gammaMinRichText;
	static QString _gammaMajRichText;
	static QString _iotaMinRichText;
	static QString _iotaMajRichText;
	static QString _phiMinRichText;
	static QString _phiMajRichText;
	static QString _kappaMinRichText;
	static QString _kappaMajRichText;
	static QString _lambdaMinRichText;
	static QString _lambdaMajRichText;
	static QString _muMinRichText;
	static QString _muMajRichText;
	static QString _nuMinRichText;
	static QString _nuMajRichText;
	static QString _omicronnMinRichText;
	static QString _omicronnMajRichText;
	static QString _piMinRichText;
	static QString _piMajRichText;
	static QString _thetaMinRichText;
	static QString _thetaMajRichText;
	static QString _roMinRichText;
	static QString _roMajRichText;
	static QString _sigmaMinRichText;
	static QString _sigmaMajRichText;
	static QString _tauMinRichText;
	static QString _tauMajRichText;
	static QString _upsilonMinRichText;
	static QString _upsilonMajRichText;
	static QString _omegaMinRichText;
	static QString _omegaMajRichText;
	static QString _xsiMinRichText;
	static QString _xsiMajRichText;
	static QString _psiMinRichText;
	static QString _psiMajRichText;
	static QString _dzetaMinRichText;
	static QString _dzetaMajRichText;
};	// class QtStringHelper

#endif	// QT_STRING_HELPER_H

