#ifndef QT_TEXT_EDITOR_H
#define QT_TEXT_EDITOR_H

#ifndef QT_5
#include <QtGui/QPlainTextEdit>
#else	// QT_5
#include <QPrinter>	// requested by Qt 5.8.0
#include <QPlainTextEdit>
#endif	// QT_5


/**
 * Editeur de texte Qt étendu permettant, entre autres, l'affichage des numéros 
 * de ligne.
 *
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class QtTextEditor : public QPlainTextEdit
{
	Q_OBJECT

	public :

	/**
	 * Constructeur. RAS.
	 * @param	widget parent
	 * @param	<I>true</I> si les numéros de lignes doivent être affichés,
	 *			<I>false</I> dans le cas contraire.
	 */
	QtTextEditor (QWidget* parent, bool displayLinesNumbers);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~QtTextEditor ( );

	/**
	 * @return	<I>true</I> si les numéros de lignes sont affichés,
	 *			<I>false</I> dans le cas contraire.
	 */
	virtual bool displayLinesNumbers ( ) const;

	/**
	 *			<I>false</I> dans le cas contraire.
	 */
	virtual void displayLinesNumbers (bool display);

	/**
	 * @return	La largeur souhaitée pour l'affichage des numéros de ligne.
	 */
	virtual int lineNumberAreaWidth ( ) const;

	/**
	 * Dessine les numéros de ligne dans le rectangle transmis en argument.
	 */
	virtual void drawLinesNumbers (const QRect& rect);


	protected :

	/**
	 * Classe permettant l'affichage des numéros de ligne dans l'éditeur de
	 * texte.
	 */
	class QtLineNumberArea : public QWidget
	{
		public :

		/**
		 * Constructeur. RAS.
		 * @param	éditeur (parent) associé.
		 */
		QtLineNumberArea (QtTextEditor&);

		/**
		 * Destructeur. RAS.
		 */
		virtual ~QtLineNumberArea ( );

		/**
		 * @return	La taille souhaitée
		 */
		virtual QSize sizeHint ( ) const;

		/**
		 * @return	Une référence sur l'éditeur de texte.
		 */
		virtual const QtTextEditor& getTextEditor ( ) const;
		virtual QtTextEditor& getTextEditor ( );


		protected :

		/**
		 * Redessiner les numéros de lignes de la zone scrollée.
		 */
		virtual void paintEvent (QPaintEvent*);


		private :

		/**
		 * Constructeur de copie, opérateur = : interdits.
		 */
		QtLineNumberArea (const QtLineNumberArea&);
		QtLineNumberArea& operator = (const QtLineNumberArea&);

		/** L'éditeur associé. */
		QtTextEditor*		_textEditor;
	};	// class QtLineNumberArea


	protected :

	/**
	 * Affiche le menu contextuel.
	 * @see		createPopupMenu
	 */
	virtual void  contextMenuEvent (QContextMenuEvent* event);

	/**
	 * Invoqué lors d'un redimensionnement. Réorganise et réaffiche le contenu
	 * de l'éditeur.
	 */
	virtual void resizeEvent (QResizeEvent*);

	/**
	 * @return	La zone d'affichage des numéros de ligne.
	 */
	virtual const QtLineNumberArea& getLineNumberArea ( ) const;
	virtual QtLineNumberArea& getLineNumberArea ( );

	/**
	 * @return	Un menu contextuel.
	 */
	virtual QMenu* createPopupMenu ( );


	protected slots :

	/**
	 * Appelé en cas de modification du texte. Actualise la numérotation des
	 * lignes.
	 */
	virtual void updateLineNumberAreaWidthCallback (int);

	/**
	 * Appelé lors d'un évènement de scrolling.
	 * @param	zone à actualiser
	 * @param	nuombre de pixels de défilement vertical
	 */
	virtual void updateLineNumberAreaWidthCallback (const QRect&, int);

	/**
	 * Appelé lorsque le curseur change de position. Met en évidence la ligne
	 * courante.
	 */
	virtual void cursorPositionCallback ( );

	/**
	 * Appelé lorsque l'utilisateur change l'affichage des numéros de lignes.
	 */
	virtual void displayLinesNumbersCallback ( );


	private :

	/**
	 * Constructeur de copie, opérateur = : interdits.
	 */
	QtTextEditor (const QtTextEditor&);
	QtTextEditor& operator = (const QtTextEditor&);

	/** L'affichage des numéros de ligne. */
	QtLineNumberArea*		_lineNumberArea;

	/** Les numéros de lignes doivent ils être affichés ? */
	bool					_displayLinesNumbers;
};	// class QtTextEditor


#endif	// QT_TEXT_EDITOR_H
