#!/usr/bin/env python3
import os
import sys
import time

# 1. Lecture du cookie entrant transmis par webserv dans les variables d'environnement
raw_cookie = os.environ.get("HTTP_COOKIE", "")

visits = 1
if "visit_count=" in raw_cookie:
    try:
        # Extraction basique de la valeur de visit_count
        for item in raw_cookie.split(";"):
            if "visit_count=" in item:
                visits = int(item.split("=")[1].strip()) + 1
    except ValueError:
        visits = 1

# 2. Sortie des en-têtes CGI (Headers HTTP + Ligne vide)
# Le serveur CGI doit transmettre ces en-têtes Set-Cookie au client
print(f"Set-Cookie: session_id=webserv_secret_12345; Path=/; Max-Age=3600")
print(f"Set-Cookie: visit_count={visits}; Path=/; Max-Age=3600")
print("Content-Type: text/html\r\n")

# 3. Corps de la réponse HTML
print("<!DOCTYPE html>")
print("<html>")
print("<head><title>Test Cookies CGI</title></head>")
print("<body>")
print("<h1>Test des Cookies CGI</h1>")
print(f"<p>Nombre de visites comptabilisées : <b>{visits}</b></p>")
print("<hr>")
print("<h3>En-tête HTTP_COOKIE reçu par le CGI :</h3>")
if raw_cookie:
    print(f"<pre>{raw_cookie}</pre>")
else:
    print("<p><i>Aucun cookie reçu dans la requête.</i></p>")
print("</body>")
print("</html>")