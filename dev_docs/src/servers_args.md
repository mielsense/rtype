# Game Server Arguments

Le serveur R-Type (GameServer) peut être configuré à l'aide de plusieurs arguments de ligne de commande. Voici une liste des arguments disponibles et leur description :

## Arguments

### `--server-name`

- **Description**: Nom du serveur de jeu.
- **Type**: `string`
- **Valeur par défaut**: `GameServer`
- **Exemple**: `--server-name MyGameServer`

### `--max-players`

- **Description**: Nombre maximum de joueurs.
- **Type**: `int`
- **Valeur par défaut**: `4`
- **Exemple**: `--max-players 10`

### `--server-ip`

- **Description**: Adresse IP du serveur de jeu.
- **Type**: `string`
- **Valeur par défaut**: `0.0.0.0`
- **Exemple**: `--server-ip 192.168.1.1`

### `--server-port`

- **Description**: Port du serveur de jeu.
- **Type**: `int`
- **Valeur par défaut**: `4244`
- **Exemple**: `--server-port 8080`

### `--master-ip`

- **Description**: Adresse IP du serveur maître.
- **Type**: `string`
- **Valeur par défaut**: `localhost`
- **Exemple**: `--master-ip 192.168.1.2`

### `--master-port`

- **Description**: Port du serveur maître.
- **Type**: `int`
- **Valeur par défaut**: `4243`
- **Exemple**: `--master-port 8081`

### `--tickrate`

- **Description**: Tickrate du serveur.
- **Type**: `int`
- **Valeur par défaut**: `60`
- **Exemple**: `--tickrate 30`

### `--disable-console-logging`

- **Description**: Désactive les logs dans la console.
- **Type**: `bool`
- **Valeur par défaut**: `false`
- **Exemple**: `--disable-console-logging`

### `--logfile`

- **Description**: Fichier de log, vide pour ne pas utiliser de fichier de log.
- **Type**: `string`
- **Valeur par défaut**: `""`
- **Exemple**: `--logfile /var/log/rtype.log`

## Exemple de commande

```bash
./server --server-name MyGameServer --max-players 10 --server-ip 192.168.1.1 --server-port 8080 --master-ip 192.168.1.2 --master-port 8081 --tickrate 30 --disable-console-logging --logfile /var/log/rtype.log
```

# Master Server Arguments

The R-Type Master Server can be configured using several command-line arguments. Below is a list of the available arguments and their descriptions:

## Arguments

### `--server-ip`

- **Description**: IP address for the master server.
- **Type**: `string`
- **Default value**: `0.0.0.0`
- **Example**: `--server-ip 192.168.1.1`

### `--server-port`

- **Description**: Port for the master server.
- **Type**: `int`
- **Default value**: `4242`
- **Example**: `--server-port 8080`

### `--inter-server-ip`

- **Description**: IP address for the inter server.
- **Type**: `string`
- **Default value**: `0.0.0.0`
- **Example**: `--inter-server-ip 192.168.1.2`

### `--inter-server-port`

- **Description**: Port for the inter server.
- **Type**: `int`
- **Default value**: `4243`
- **Example**: `--inter-server-port 8081`

### `--tickrate`

- **Description**: Tickrate for the server.
- **Type**: `int`
- **Default value**: `60`
- **Example**: `--tickrate 30`

### `--disable-console-logging`

- **Description**: Disable console logging.
- **Type**: `bool`
- **Default value**: `false`
- **Example**: `--disable-console-logging`

### `--logfile`

- **Description**: Log file, empty for no log file.
- **Type**: `string`
- **Default value**: `""`
- **Example**: `--logfile /var/log/master_server.log`

## Example Command

```bash
./master_server --server-ip 192.168.1.1 --server-port 8080 --inter-server-ip 192.168.1.2 --inter-server-port 8081 --tickrate 30 --disable-console-logging --logfile /var/log/master_server.log