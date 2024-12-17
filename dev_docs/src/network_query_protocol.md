# Network query protocol

The query protocol is used between the master server and game lobbies
to share server availability and do simple actions.

The connection is always initiated by the lobbies to the master server
of their choice.

## Request

The Master Server queries the lobbies with a QueryRequest packet.

This is an empty packet.
## Response

The lobby responds to this request with the following information:

| type   | name          | desc  |
|--------|---------------|-------|
| u64    | player\_count |       |
| u64    | max\_players  |       |
| u16    | tick\_rate    | in Hz |
| string | server\_name  |       |
| u64    | current_level |       |
| string | server_ip     |       |
| u16    | server_port   |       |
