Exemple d'utilisation de cet exemple.
-------------------------------------

A compiler sur calculateur pour le serveur (connect_srv) et station de travail pour le client (connect_clt).

Lancer le client comme suit :
/path/connect_clt -host machine.nom_domaine -server /path/connect_srv -nshost machine.nom_domaine

=> il lancera le service de nommage puis le serveur.
Suivant ce qui suit -nshost, le service de nommage sera lancé sur calculateur ou sur station de travail.

Au 07/02/11, ce qui fonctionne :
--------------------------------

nom_station : station de travail RHEL_5__x86_64
machine.nom_domaine : noeud sur calculateur.

/path/connect_clt -host machine.nom_domaine -server /path/connect_srv -nshost nom_station -ORBtraceExceptions true

/path/exe/connect_clt -host machine.nom_domaine -server /path/exe/connect_srv -nshost nom_station.nom_domaine -ORBtraceExceptions true

Le cas ci-dessous marche, avec des échecs assez fréquents pour avoir une référence sur le service de nommage :
/path/exe/connect_clt -host machine.nom_domaine -server /path/exe/connect_srv -nshost machine.nom_domaine -ORBtraceLevel 10
Erreur éventuellement rencontrée :
omniORB: throw giopStream::CommFailure from
giopStream.cc:1077(0,NO,TRANSIENT_ConnectFailed)
omniORB: throw TRANSIENT from omniObjRef.cc:759 (NO,TRANSIENT_ConnectFailed)
Impossibilité d'avoir une référence sur le service de  nommage : CORBA::SystemException (TRANSIENT, TRANSIENT_ConnectFailed).



