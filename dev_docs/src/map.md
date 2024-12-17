# Map Format Documentation

## Overview
This format defines a 2D space shooter game map, including map dimensions, assets, player configuration, enemy placement, and background elements.

## Basic Properties
```json
{
  "mapWidth": 10000.0,
  "mapHeight": 1080.0,
  "music": "client/assets/audio/map-1.ogg"
}
```
- `mapWidth`: Width of the playable map area in pixels
- `mapHeight`: Height of the playable map area in pixels
- `music`: Path to the background music file for this map

## Asset References

### Textures
```json
"textures": {
  "player": "path/to/texture.png",
  "enemy": "path/to/enemy.png",
  ...
}
```
Defines paths to all texture assets used in the map, including:
- Player ships
- Enemy units
- Background elements
- UI elements
- Projectiles

### Animations
```json
"animations": {
  "player": "path/to/animation.json",
  "enemy": "path/to/animation.json",
  ...
}
```
Maps entity types to their animation definition files.

## Background Layers
```json
"background": [
  {
    "type": "regular",
    "texture": "background",
    "scrollSpeed": 50.0
  },
  {
    "type": "random",
    "texture": "asteroid-1",
    "scrollSpeed": 150.0,
    "count": 5
  }
]
```
Array of background layer definitions, supporting two types:
- `regular`: Single repeating background layer
    - `texture`: Reference to texture in texture map
    - `scrollSpeed`: Horizontal scroll speed in pixels per second
- `random`: Randomly placed decorative elements
    - `texture`: Reference to texture in texture map
    - `scrollSpeed`: Horizontal scroll speed in pixels per second
    - `count`: Number of instances to generate

## Player Configuration
```json
"player": {
  "texture": "player",
  "health": 100.0,
  "position": [400.0, 300.0],
  "maxSpeed": [300.0, 300.0],
  "scale": [4.0, 4.0],
  "animation": "player",
  "hitboxOffset": [-16.0, -24.0],
  "tag": "tag for player"
}
```
Defines player starting properties:
- `texture`: Reference to player texture
- `health`: Initial health value
- `position`: Starting [x, y] coordinates
- `maxSpeed`: Maximum [horizontal, vertical] speed
- `scale`: Texture scale factor [x, y]
- `animation`: Reference to animation definition
- `hitboxOffset`: Collision box offset from sprite center [x, y]
- `tag`: Tag for the player entity

## Missile Configuration
```json
"missile": {
  "texture": "missile",
  "position": [0.0, 0.0],
  "maxSpeed": [0.0, 0.0],
  "scale": [3.0, 3.0],
  "animation": "missile",
  "hitboxOffset": [-16.0, -16.0]
}
```
Defines properties for player projectiles, using the same property structure as the player configuration.

## Enemy Placements
```json
"enemies": [
  {
    "texture": "enemy-type",
    "health": 100.0,
    "position": [1200.0, 500.0],
    "velocity": [0.0, 0.0],
    "scale": [4.0, 4.0],
    "animation": "enemy-animation",
    "hitboxOffset": [2.0, 2.0],
    "tag": "tag for enemy"
  }
]
```
Array of enemy definitions, each containing:
- `texture`: Reference to enemy texture
- `health`: Initial health value
- `position`: Spawn [x, y] coordinates
- `velocity`: Initial [horizontal, vertical] velocity
- `scale`: Texture scale factor [x, y]
- `animation`: Reference to animation definition
- `hitboxOffset`: Collision box offset from sprite center [x, y]
- `tag`: Tag for the enemy entity
