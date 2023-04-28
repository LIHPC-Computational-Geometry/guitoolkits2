#ifndef AUTO_ARRAY_PTR_H
#define AUTO_ARRAY_PTR_H

#include <TkUtil/util_config.h>

#include <assert.h>


BEGIN_NAMESPACE_UTIL


/**
 * <P>
 * Cette classe permet de gérer des tableaux de type simple. Son destructeur
 * invoque notamment <I>delete []</I>. Divers opérateurs d'accès aux données
 * du tableau sont proposés.
 * @warning	Absence totale de test sur les indices.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
template <class T> class AutoArrayPtr
{
	public :

	/**
	 * Constructeur par défaut : absence de tableau.
	 */
	AutoArrayPtr ( )
		: _array (0)
	{ }

	/**
	 * Constructeur avec tableau : prise en charge du tableau qui sera
	 * détruit par cette instance (sauf appel de <I>release</I>).
	 */
	AutoArrayPtr (T* array)
		: _array (array)
	{ }

	/**
	 * Constructeur allocateur : instancie un tableau de taille donnée en
	 * argument.
	 * @warning		Une exception <I>bad_alloc</I> peut éventuellement être
	 *				levée. Cela dépend du compilateur et des directives de
	 *				compilation utilisées.
	 */
	AutoArrayPtr (size_t size)
		: _array (new T [size])
	{
	}

	/**
	 * Destructeur : détruit le tableau si <I>release</I> n'a pas été
	 * préalablement appelé.
	 * @see		release
	 */
	~AutoArrayPtr ( )
	{ delete [] _array; }

	/**
	 * @return		un pointeur sur le premier élément du tableau.
	 */
	const T* get ( ) const
	{ return _array; }
	T* get ( )
	{ return _array; }

	/**
	 * @return		le pointeur sur le tableau, qui n'est plus pris en charge
	 *				par cette instance.
	 */
	T* release ( )
	{
		T*	ptr	= _array;
		_array	= 0;
		return ptr;
	}	// release

	/**
	 * Détruit le tableau en cours et prend en charge le tableau reçu en
	 * argument.
	 */
	void reset (T* array)
	{
		delete [] _array;
		_array	= array;
	}	// reset (T* array)

	/**
	 * Détruit le tableau en cours et alloue un nouveau tableau de taille
	 * reçue en argument.
	 */
	void reset (size_t size)
	{
		delete [] _array;
		_array	= new T [size];
	}	// reset (size_t)

	/**
	 * @return		l'élément dont l'index est tranmis en argument
	 * @warning		ne vérifie pas un éventuel débordement
	 */
	const T& operator [] (size_t index) const
	{ return _array [index]; }
	T& operator [] (size_t index)
	{ return _array [index]; }


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	AutoArrayPtr (const AutoArrayPtr&)
	{ assert (0 && "AutoArrayPtr copy constructor is not allowed."); }
	AutoArrayPtr& operator = (const AutoArrayPtr&)
	{ assert (0 && "AutoArrayPtr operator = is not allowed."); return *this; }

	/** Le tableau géré. */
	T*							_array;
};	// class AutoArrayPtr



END_NAMESPACE_UTIL


#endif	// AUTO_ARRAY_PTR_H
