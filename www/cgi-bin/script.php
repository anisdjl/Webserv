#!/usr/bin/php-cgi
<?php
// En-tête envoyé par PHP (php-cgi formate automatiquement les headers CGI)
header("Content-Type: text/html; charset=UTF-8");

$method = $_SERVER['REQUEST_METHOD'] ?? 'Inconnu';
$queryString = $_SERVER['QUERY_STRING'] ?? 'Vide';
$cookies = $_COOKIE;
$postData = file_get_contents('php://input');
?>
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Test CGI PHP - Webserv</title>
    <style>
        body { font-family: monospace; background: #1e1e2e; color: #cdd6f4; padding: 30px; }
        .card { background: #313244; border-radius: 8px; padding: 20px; max-width: 700px; margin: 0 auto; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }
        h1 { color: #a6e3a1; margin-top: 0; }
        .row { margin: 10px 0; padding: 8px; background: #181825; border-radius: 4px; }
        .label { color: #89b4fa; font-weight: bold; }
        pre { background: #11111b; padding: 10px; border-radius: 4px; color: #f9e2af; overflow-x: auto; }
    </style>
</head>
<body>
    <div class="card">
        <h1>🐘 Succès CGI PHP !</h1>
        <div class="row"><span class="label">Méthode :</span> <?php echo htmlspecialchars($method); ?></div>
        <div class="row"><span class="label">Query String :</span> <?php echo htmlspecialchars($queryString); ?></div>
        <div class="row"><span class="label">Variables GET reçues :</span>
            <pre><?php print_r($_GET); ?></pre>
        </div>
        <div class="row"><span class="label">Corps POST brut (php://input) :</span>
            <pre><?php echo htmlspecialchars($postData ?: '(Vide)'); ?></pre>
        </div>
        <div class="row"><span class="label">Cookies reçus :</span>
            <pre><?php print_r($cookies); ?></pre>
        </div>
    </div>
</body>
</html>