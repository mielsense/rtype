# Network game protocol

## Types

### Position

| type   | name |
|--------|------|
| double | x    |
| double | y    |

### Color

In the sRGB color space.

| type | name |
|------|------|
| char | r    |
| char | g    |
| char | b    |

### Player

| type          | name     |
|---------------|----------|
| string[3-256] | name     |
| Color         | color    |
| Position      | position |

## Packets

### Connection

Sent from the client when it connects to the game server.

| type          | name              |
|---------------|-------------------|
| string[3–256] | player\_name      |
| Color         | color             |
| u64           | protocol\_version |

The server replies, accepting or not the connection. If the connection
is not validated, the server will close the connection after sending
the response.

| type     | name                 |
|----------|----------------------|
| bool     | connection_validated |

### Initial synchronisation

After connection, the server sends the necessary data to bootstrap the
player into the game. This packet can also be sent by the server at
any point in case of desynchronization with the client.

| type     | name            | description                        |
|----------|-----------------|------------------------------------|
| u64      | player\_id      | Your id in the players list.       |
| Player[] | players         | The list of all connected players. |
| u64      | level\_id       |                                    |
| Position | player_position |                                    |

### Gradual synchronisation

These packets are continuously set by the server to the client to keep
him up to date on the state of the game.

- **Entity movement**

| type     | name  |
|----------|-----------|
| u64      | entity_id |
| Position | position  |

- **New entity**

| type     | name | description                    |
|----------|----------|--------------------------------|
| u64      | id       | New ID assigned to the entity. |
| Position | position |                                |

- **Destroy entity**

| type | name |
|------|----------|
| u64  | id       |

### Client events

These packets are sent by clients when they want to perform actions in
the game.

- **Move player**

The new position is to be clamped by the server based on distance for
anti-cheating reasons.

| type     | name |
|----------|----------|
| Position | new_pos  |

- **Shoot**

The server should clamp the origin based on a maximum distance from
the player for anti-cheating.

| type     | name |
|----------|----------|
| Position | origin   |

- **Use special ability**

| type     | name     |
|----------|----------|
| u8       | ability  |
| Position | position |
