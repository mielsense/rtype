# Server Architecture

## Introduction

Our architecture is composed of a Master Server and a Game Server.
The master server is responsible for managing the game servers and the game server is responsible for managing the game itself.
As a player you will connect to the master server and the master server will redirect you to the game server you asked for.

![server_archi_diagram.png](./assets/server_archi_diagram.png)

## Master Server

Under the hood the master server is a double server, both uses RUDP to communicate.
The Master Server could be reponsible for matchmaking, but it is not implemented yet.
Once a player has selected a game server, the master server will redirect the player to the game server.

## Game Server

Nothing much to say about the game server, it is the server that will host the game.
They can be multiple game servers running at the same time with independant tick rates.

