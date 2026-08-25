# #!/usr/bin/env python3
# import os
# import sys

# # 1. En-têtes CGI obligatoires (séparés du body par une ligne vide \r\n\r\n)
# sys.stdout.write("Content-Type: text/html; charset=utf-8\r\n")
# sys.stdout.write("Status: 200 OK\r\n")
# sys.stdout.write("\r\n")

# # 2. Récupération des variables d'environnement transmises par Webserv
# method = os.environ.get("REQUEST_METHOD", "Non défini")
# query = os.environ.get("QUERY_STRING", "Aucun paramètre")
# protocol = os.environ.get("SERVER_PROTOCOL", "Non défini")
# script_name = os.environ.get("SCRIPT_FILENAME", "Non défini")

# # 3. Génération du corps HTML
# html = f"""<!DOCTYPE html>
# <html lang="fr">
# <head>
#     <meta charset="UTF-8">
#     <title>Test CGI Webserv</title>
#     <style>
#         body {{ font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; padding: 40px; background: #0f172a; color: #f8fafc; }}
#         .card {{ background: #1e293b; border-radius: 8px; padding: 24px; max-width: 600px; margin: 0 auto; box-shadow: 0 4px 6px -1px rgba(0,0,0,0.3); }}
#         h1 {{ color: #38bdf8; margin-top: 0; }}
#         ul {{ list-style: none; padding: 0; }}
#         li {{ padding: 8px 0; border-bottom: 1px solid #334155; }}
#         span.key {{ color: #94a3b8; font-weight: bold; }}
#         span.val {{ color: #a5f3fc; font-family: monospace; }}
#     </style>
# </head>
# <body>
#     <div class="card">
#         <h1> Succès CGI !</h1>
#         <p>Le script Python s'est exécuté sans crash et le navigateur a rendu le HTML.</p>
#         <ul>
#             <li><span class="key">Méthode :</span> <span class="val">{method}</span></li>
#             <li><span class="key">Query String :</span> <span class="val">{query}</span></li>
#             <li><span class="key">Protocole :</span> <span class="val">{protocol}</span></li>
#             <li><span class="key">Chemin Script :</span> <span class="val">{script_name}</span></li>
#             <li><span class="key">Calcul dynamique :</span> <span class="val">42 * 2 = {42 * 2}</span></li>
#         </ul>
#     </div>
# </body>
# </html>
# """

# sys.stdout.write(html)
# sys.stdout.flush()

#!/usr/bin/env python3
import os
import sys

# 1. En-têtes CGI
sys.stdout.write("Content-Type: text/html; charset=utf-8\r\n")
sys.stdout.write("Status: 200 OK\r\n")
sys.stdout.write("\r\n")

# 2. Récupération des métadonnées
method = os.environ.get("REQUEST_METHOD", "Non défini")
content_length = os.environ.get("CONTENT_LENGTH", "0")

# 3. Lecture du Body sur stdin (fourni par ft_cgi_out via le pipe)
body_received = ""
try:
    # Si CONTENT_LENGTH est renseigné, on peut lire exactement la taille
    length = int(content_length)
    if length > 0:
        body_received = sys.stdin.read(length)
    else:
        # Fallback : lecture jusqu'à l'EOF (pipe fermé)
        body_received = sys.stdin.read()
except Exception as e:
    body_received = f"Erreur lors de la lecture : {e}"

# 4. Rendu HTML
html = f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Test CGI POST</title>
    <style>
        body {{ font-family: monospace; padding: 30px; background: #18181b; color: #f4f4f5; }}
        .box {{ background: #27272a; border-radius: 6px; padding: 20px; max-width: 650px; margin: 0 auto; border: 1px solid #3f3f46; }}
        h2 {{ color: #22c55e; margin-top: 0; }}
        pre {{ background: #09090b; padding: 12px; border-radius: 4px; color: #a1a1aa; overflow-x: auto; }}
        .tag {{ color: #38bdf8; font-weight: bold; }}
    </style>
</head>
<body>
    <div class="box">
        <h2> Requête POST traitée avec succès !</h2>
        <p><span class="tag">Méthode :</span> {method}</p>
        <p><span class="tag">Content-Length déclaré :</span> {content_length} octets</p>
        <p><span class="tag">Taille lue :</span> {len(body_received)} octets</p>
        <p><span class="tag">Contenu brut reçu sur stdin :</span></p>
        <pre>{body_received if body_received else "(Body vide)"}</pre>
    </div>
</body>
</html>
"""

sys.stdout.write(html)
sys.stdout.flush()

# #!/usr/bin/env python3
# import os
# import sys
# import time

# # 1. Lecture du cookie entrant transmis par webserv dans les variables d'environnement
# raw_cookie = os.environ.get("HTTP_COOKIE", "")

# visits = 1
# if "visit_count=" in raw_cookie:
#     try:
#         # Extraction basique de la valeur de visit_count
#         for item in raw_cookie.split(";"):
#             if "visit_count=" in item:
#                 visits = int(item.split("=")[1].strip()) + 1
#     except ValueError:
#         visits = 1

# # 2. Sortie des en-têtes CGI (Headers HTTP + Ligne vide)
# # Le serveur CGI doit transmettre ces en-têtes Set-Cookie au client
# print(f"Set-Cookie: session_id=webserv_secret_12345; Path=/; Max-Age=3600")
# print(f"Set-Cookie: visit_count={visits}; Path=/; Max-Age=3600")
# print("Content-Type: text/html\r\n")

# # 3. Corps de la réponse HTML
# print("<!DOCTYPE html>")
# print("<html>")
# print("<head><title>Test Cookies CGI</title></head>")
# print("<body>")
# print("<h1>Test des Cookies CGI</h1>")
# print(f"<p>Nombre de visites comptabilisées : <b>{visits}</b></p>")
# print("<hr>")
# print("<h3>En-tête HTTP_COOKIE reçu par le CGI :</h3>")
# if raw_cookie:
#     print(f"<pre>{raw_cookie}</pre>")
# else:
#     print("<p><i>Aucun cookie reçu dans la requête.</i></p>")
# print("</body>")
# print("</html>")