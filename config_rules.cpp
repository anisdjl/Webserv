/*

server:
	- listen : host:port ou juste port-> obligatoire si pas dispo -> erreur
	- server_name -> string _> pas obligatoire
	- error_page -> pas obligatoire mais peut se repeter -> si absemte ca sera une page generee par defaut 
	- client_max_body_size: pas obligatoire -> fixe une valeur par defaut genre 1M pour eviter les illimites
	- location -> au moins un -> si absent -> erreur

location:
	- root: pas obligatoire mais indispensable -> soit herite du root server parent, soit erreur 404 systemeatique
	- methods: pas obligatoire si absent on met get seul par defaut 
	- index: pas obligatoire -> je mets rien si dossier demande et pas d'auto index ou 404/403
	- autoindex -> pas obligatoire -> off par defaut
	- return (redirection) -> pas obligatoire -> pas de redirection par defaut
	- upload_store : pas obligatoire -> upload interdit par defaut sur celle route si absent
	- cgi_extension: pas obligatoire mais rpeetable -> si absent pas de cgi sur cette route


*/