#!/usr/bin/env python3
import os
import sys

# 1. En-têtes CGI obligatoires (séparés du body par une ligne vide \r\n\r\n)
sys.stdout.write("Content-Type: text/html; charset=utf-8\r\n")
sys.stdout.write("Status: 200 OK\r\n")
sys.stdout.write("\r\n")

# 2. Récupération des variables d'environnement transmises par Webserv
method = os.environ.get("REQUEST_METHOD", "Non défini")
query = os.environ.get("QUERY_STRING", "Aucun paramètre")
protocol = os.environ.get("SERVER_PROTOCOL", "Non défini")
script_name = os.environ.get("SCRIPT_FILENAME", "Non défini")

# 3. Génération du corps HTML
html = f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Test CGI Webserv</title>
    <style>
        body {{ font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; padding: 40px; background: #0f172a; color: #f8fafc; }}
        .card {{ background: #1e293b; border-radius: 8px; padding: 24px; max-width: 600px; margin: 0 auto; box-shadow: 0 4px 6px -1px rgba(0,0,0,0.3); }}
        h1 {{ color: #38bdf8; margin-top: 0; }}
        ul {{ list-style: none; padding: 0; }}
        li {{ padding: 8px 0; border-bottom: 1px solid #334155; }}
        span.key {{ color: #94a3b8; font-weight: bold; }}
        span.val {{ color: #a5f3fc; font-family: monospace; }}
    </style>
</head>
<body>
    <div class="card">
        <h1> Succès CGI !</h1>
        <p>Le script Python s'est exécuté sans crash et le navigateur a rendu le HTML.</p>
        <ul>
            <li><span class="key">Méthode :</span> <span class="val">{method}</span></li>
            <li><span class="key">Query String :</span> <span class="val">{query}</span></li>
            <li><span class="key">Protocole :</span> <span class="val">{protocol}</span></li>
            <li><span class="key">Chemin Script :</span> <span class="val">{script_name}</span></li>
            <li><span class="key">Calcul dynamique :</span> <span class="val">42 * 2 = {42 * 2}</span></li>
        </ul>
    </div>
</body>
</html>
"""

sys.stdout.write(html)
sys.stdout.flush()