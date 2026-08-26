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
