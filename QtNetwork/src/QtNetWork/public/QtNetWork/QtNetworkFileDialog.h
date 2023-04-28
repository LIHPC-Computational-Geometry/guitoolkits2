#ifndef QT_NETWORK_FILE_DIALOG_H
#define QT_NETWORK_FILE_DIALOG_H


#include <TkUtil/File.h>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <string>


/*!
 * \mainpage    Page principale de la bibliothèque QtNetwork.
 *
 * \section presentation    Présentation
 * La bibliothèque <I>qtnetwork</I> propose un sélecteur de fichier <I>Qt</I>
 * fonctionnant sur le réseau. Elle utilise à cet effet l'outil <I>TkUtilLauncher</I>
 * de la bibliothèque TkUtil.
 *
 * \section depend      Dépendances
 * Cette bibliothèque utilise la bibliothèque TkUtil.
 * Les directives de compilation et d'édition des liens de la bibliothèque
 * <I>TkUtil</I> s'appliquent également à celle-ci.
 */



/**
 * Widget type "File dialog" permettant de sélectionner un fichier sur le
 * réseau. Cette classe propose une API simplifiée de la classe
 * <I>QFileDialog</I>, à laquelle il convient d'ajouter la méthode
 * <I>getHostName</I> permettant de savoir sur quelle machine du réseau est
 * localisé le fichier sélectionné.
 * 
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class QtNetworkFileDialog : public QDialog 
{
	Q_OBJECT

	public :

	/** Commande à exécuter pour connaitre le répertoire <I>home
	 * utilisateur</I> (<I>home</I> par défaut).
	 */
	static std::string		homeCommand;

	/**
	 * Classe représentant un système de fichiers localisé sur une autre machine
	 * que celle du processus courant.
	 * Implémentation à revoir.
	 * La classe refaite devrait comprendre le nom de la machine hôte,
	 * surcharger des méthodes telles que <I>create</I> ou <I>remove</I>.
	 */
	class RemoteFile : public TkUtil::File
	{
		public :

		/**
		 * @param		Fichier représenté
		 * @param		true si c'est un répertoire, false dans le cas contraire
		 */	
		RemoteFile (const std::string& path, bool dir)
			: File (path), _isDir (dir)
		{ }

		RemoteFile (const RemoteFile& rf)
			: File (rf), _isDir (rf._isDir)
		{ }
		RemoteFile& operator = (const RemoteFile& rf)
		{
			if (&rf != this)
			{
				File::operator = (rf);
				_isDir	= rf._isDir;
			}

			return *this;
		}	// operator =
		virtual ~RemoteFile ( )
		{ }

		/**
		 * Lève une exception, méthode non implémentée.
		 */
		virtual bool exists ( ) const;
		virtual bool isDirectory ( ) const
		{ return _isDir; }
		virtual bool isLink ( ) const;
		virtual bool isReadable ( ) const;
		virtual bool isWritable ( ) const;
		virtual bool isExecutable ( ) const;
		virtual void create (bool asFile);
		virtual void reset ( );
		virtual void remove ( );


		private :

		bool			_isDir;
	};	// class RemoteFile

	/**
	 * Classe d'item représentant une inode (fichier, répertoire) dans une
	 * instance de la classe QListWidget.
 	*/
	class QtInodItem : public QListWidgetItem
	{
		public :

		/**
		 * @param		Widget parent
		 * @param		Inode représentée
		 */
		QtInodItem (const RemoteFile& inod, QListWidget * parent);

		/**
		 * Destructeur. RAS.
		 */
		virtual ~QtInodItem ( )
		{ }

		/**
		 * @return		L'inode.
		 */
		virtual const RemoteFile& getInod ( ) const
		{ return _inod; }

		/**
		 * @return		Une icône pour l'inode transmise en argument
		 */	
		static QIcon getIcon (const RemoteFile& inod);


		private :

		/**
		 * Constructeur de copie, opérateur = : interdits.
		 */
		QtInodItem (const QtInodItem&);
		QtInodItem& operator = (const QtInodItem&);

		/** L'inode représentée. */
		const RemoteFile			_inod;
	};	// class QtInodItem

	/**
	 * Constructeur
	 * @param		parent
	 * @param		titre de l'application
	 * @param		caractère modal de la boite de dialogue
	 */
	QtNetworkFileDialog (QWidget* parent, const char* title, bool modal = true);

	/**
	 * Constructeur
	 * @param		machine initiale
	 * @param		répertoire initial
	 * @param		filtre appliqué
	 * @param		parent
	 * @param		titre de l'application
	 * @param		caractère modal de la boite de dialogue
	 * @param		caractère "figé" de la boite de dialogue. S'il vaut
	 * 				<I>true</I> le contenu de la boite de dialogue ne sera
	 * 				actualisé que lors d'un appel à <I>update</I>.
	 */
	QtNetworkFileDialog (const QString& hostName, const QString& dirName,
	                     const QString& filter, QWidget* parent,
	                     const char* title, bool modal = true,
	                     bool frozen = false);

	/**
	 * Destructeur.
	 */
	virtual ~QtNetworkFileDialog ( );

	/**
	 * Ajoute à la boite de dialogue les répertoires présélectionnés transmis en argument.
	 */
	 virtual void addPreselectedDirectories (const QStringList& directories);

	/**
	 * @return		Le nom de la machine hôte.
	 */
	virtual QString getHostName ( ) const;

	/**
	 * @return		<I>true</I> si on autorise l'utilisateur à changer de
	 *				machine hôte, <I>false</I> dans le cas contraire.
	 */
	virtual bool allowHostNameModification ( ) const;

	/**
	 * Autorise ou interdit la modification de la machine hôte.
	 * @param		<I>true</I> si on autorise l'utilisateur à changer de
	 *				machine hôte, <I>false</I> dans le cas contraire.
	 */
	virtual void allowHostNameModification (bool allow);

	/**
	 * @return		Le répertoire courant.
	 */
	virtual QString getDirectory ( ) const
	{ return _directory; }

	/**
	 * Ferme la boite de dialogue et mémorise le choix de l'utilisateur.
	 */
	virtual void accept ( );

	/**
	 * @return		La liste des fichiers sélectionnés.
	 */
	virtual QStringList selectedFiles ( ) const;

	/**
	 * @return	Le type de boite de dialogue (lecture/enregistrement).
	 */
	virtual QFileDialog::AcceptMode acceptMode ( ) const
	{ return _acceptMode; }

	/**
	 * @param	Le type de boite de dialogue (lecture/enregistrement).
	 */
	virtual void setAcceptMode (QFileDialog::AcceptMode mode);

	/**
	 * @return		Le mode de fonctionnement courant de la boite de dialogue.
	 */
	virtual QFileDialog::FileMode fileMode ( ) const
	{ return _fileMode; }

	/**
	 * @param		Le mode de fonctionnement courant de la boite de dialogue.
	 */
	virtual void setFileMode (QFileDialog::FileMode fileMode);

	/**
	 * @return		La liste des filtres possibles du sélecteur de fichiers.
	 */
	virtual QStringList nameFilters ( ) const;

	/**
	 * @param		La liste des filtres possibles du sélecteur de fichiers.
	 */
	virtual void setNameFilters (const QStringList& filters);

	/**
	 * @return		Le filtre courant sur les noms de fichier.
	 */
	virtual QString selectedNameFilter ( ) const
	{ return _filter; }

	/**
	 * @param		Filtre à appliquer sur les noms de fichier.
	 */
	virtual void selectNameFilter (const QString& name);

	/**
	 * Se met à jour en fonction des paramètres courants.
	 * @see		freeze
	 */
	virtual void update ( );

	/**
	 * @param	Inactive (<I>true</I> ou active (<I>false</I>) les appels
	 *			à <I>update ( )</I>). Permet d'optimiser la création du
	 *			sélecteur de fichier, chaque appel à une méthode (set*) étant
	 *			susceptible d'appeler <I>update ( )</I>.
	 *			Appelle <I>update ( )</I>) si <I>freez</I> vaut <I>false</I>.
	 */
	virtual void freeze (bool freez);


	protected :

	/**
	 * Création de l'IHM.
	 * @param		Titre de la boite de dialogue.
	 * @param		Machine hôte.
	 */
	virtual void createGui (const QString& title, const QString& hostName);

	/**
	 * Met à jour l'IHM.
	 * @param		Nouveau répertoire courant.
	 */
	virtual void setDirectory (const QString& dir);

	/**
	 * Met à jour le titre de la boite de dialogue en fonction de ses
	 * paramètres.
	 * @see		setAcceptMode
	 * @see		setFileMode
	 */
	virtual void updateWindowTitle ( );


	protected slots :

	/**
	 * Appelé lorsque l'utilisateur fait un clic sur un item.
	 * Ajoute, s'il y a lieu, le nom de fichier sélectionné au champ de texte
	 * "Fichier".
	 */
	virtual void itemClicked (QListWidgetItem* item);

	/**
	 * Appelé lorsque l'utilisateur fait un double-clic sur un item.
	 * Effectue l'action associée à la nature de l'item cliqué.
	 */
	virtual void itemDoubleClicked (QListWidgetItem* item);

	/**
	 * Appellé lorsque l'utilisateur clique sur le bouton "home".
	 * Se déplace dans le répertoire racine de l'utilisateur.
	 */
	virtual void homeClicked (bool);

	/**
	 * Appellé lorsque l'utilisateur clique sur le bouton "parent".
	 * Se déplace dans le répertoire parent.
	 */
	virtual void parentClicked (bool);

	/**
	 * Appelé lorsque l'utilisateur sélectionne un répertoire de la présélection. Se déplace dans ce répertoire.
	 */
	 virtual void preselectionChanged (int);

	/**
	 * Appelé lorsque l'utilisateur change le nom du répertoire sélectionné.
	 * Met à jour l'arborescence de fichiers.
	 */
	virtual void directoryChanged ( );

	/**
	 * Appelé lorsque l'utilisateur change le filtre appliqué sur les noms de
	 * fichiers.
	 * Met à jour l'arborescence de fichiers.
	 */
	virtual void filtersChanged (int);

	/**
	 * Appelé lorsque l'utilisateur modifie le nom du fichier sélectionné dans	 le champ de texte prévu à cet effet.
	 * Annule la sélection dans la vue "arborescence".
	 */
	virtual void fileNameEdited (const QString&);

	/**
	 * Appelé lorsque l'utilisateur change la machine explorée.
	 * Met à jour l'arborescence de fichiers.
	 */
	virtual void hostChanged ( );


	private :

	/** Constructeur de copie. Interdit. */
	QtNetworkFileDialog (const QtNetworkFileDialog&);

	/** Opérateur de copie. Interdit. */
	QtNetworkFileDialog& operator = (const QtNetworkFileDialog&);

	/** Si <I>true</I> les appels à <I>update ( )</I> sont sans effet. */
	bool					_frozen;		// v 3.1.0

	/** Le répertoire courant sur la machine hôte. */
	QString					_directory;

	/** Le filtre appliqué sur les noms de fichiers. */
	QString					_filter;

	/** Le type de boite de dialogue (lecture/enregisterment). */
	QFileDialog::AcceptMode	_acceptMode;

	/** Le mode de fonctionnement courant de la boite de dialogue. */
	QFileDialog::FileMode	_fileMode;

	/** Affichage des répertoires et fichiers. */
	QListWidget*			_fileSystemListView;

	/** Filtres sur les noms de fichier. */
	QComboBox*				_filtersComboBox;

	/** Le champ de saisi du nom répertoire parent. */
	QLineEdit*				_directoryTextField;

	/** Répertoires pré-sélectionnés. */
	QComboBox*				_preselectionComboBox;	// v 5.3.0

	/** Le label associé au champ de saisi du nom de fichier ou répertoire sélectionné. */
	QLabel*					_fileNameLabel;

	/** Le champ de saisi du nom de fichier ou répertoire sélectionné. */
	QLineEdit*				_fileNameTextField;

	/** Le champ de saisi du nom de la machine explorée. */
	QLineEdit*				_hostTextField;
};	// class QtNetworkFileDialog



#endif	// QT_NETWORK_FILE_DIALOG_H
