#ifndef APP_API_H
#define APP_API_H

/*
 * API de libpluginapi.so
 * Edition des liens effectuee necessairement dynamiquement afin que tout
 * plugin y accède.
 */
#ifdef  __cplusplus
extern "C"
{
#endif  /* __cplusplus */

	void hello ( );
	unsigned long getLongValue ( );

#ifdef  __cplusplus
};	/* extern "C" */
#endif  /* __cplusplus */

#endif	/* APP_API_H */
