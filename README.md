# R-Type
The not so simple multiplayer "game"

<img width="1612" alt="SCR-20241217-mbox" src="https://github.com/user-attachments/assets/4b781bc5-dc57-4bc2-826e-b5d29d0add31" />

A modern reimplementation of the classic shoot'em up game R-Type, developed as part of EPITECH's Advanced C++ project. This multiplayer networked version features a custom game engine, real-time multiplayer support, and an integrated map editor.

## Features

- **Multiplayer Gameplay**: Up to 4 players can join together in epic space battles
- **Custom Game Engine**: Built with a robust Entity Component System (ECS) architecture
- **Integrated Map Editor**: Drag-and-drop interface for creating custom levels
- **Network Architecture**: 
  - Master Server for managing game instances
  - Individual Game Servers for match hosting
  - Custom networking library built on ENet for reliable UDP communication
- **Cross-Platform**: Runs on Linux (primary) and Windows
- **Docker Support**: Easy deployment with containerization
- **Kubernetes Ready**: Scale your game servers effortlessly

## Screenshots

<img width="1612" alt="SCR-20241217-mcvd" src="https://github.com/user-attachments/assets/b60ae529-e786-4639-ab45-f6615c0fdb91" />

<img width="1612" alt="SCR-20241217-mcmu" src="https://github.com/user-attachments/assets/5135a567-a6c8-4b55-beb4-cf923972f5c9" />

<img width="1612" alt="SCR-20241217-mbmg" src="https://github.com/user-attachments/assets/82bbf0cb-19ef-4f5b-bcc5-72835e1f4a44" />

## Technologies Used

- **SFML**: Graphics and window management
- **ImGui**: User interface and map editor
- **ENet**: UDP networking foundation
- **Custom ECS**: Game engine architecture
- **Docker & Kubernetes**: Deployment and scaling
- **VcPkg**: Package Manager

## Building and Running

### Prerequisites

- CMake (3.15+)
- C++17 compatible compiler
- vcpkg package manager
- Docker (optional, for containerized deployment)

### Dependencies

This project uses vcpkg for dependency management. Here are the required dependencies with their minimum versions:

```json
{
  "dependencies": [
    {
      "name": "sfml",
      "version>=": "2.6.1"
    },
    {
      "name": "fmt",
      "version>=": "11.0.2"
    },
    {
      "name": "gtest",
      "version>=": "1.15.2"
    },
    {
      "name": "imgui-sfml",
      "version>=": "2.6"
    },
    {
      "name": "enet",
      "version>=": "1.3.17#2"
    },
    {
      "name": "asio",
      "version>=": "1.31.0"
    },
    {
      "name": "argparse",
      "version>=": "3.1"
    },
    {
      "name": "nlohmann-json",
      "version>=": "3.11.3#1"
    }
  ]
}
```

#### Installing Dependencies

You have two options for installing the dependencies:

1. **Using vcpkg.json (Recommended)**:
   ```bash
   # Clone the repository if you haven't already
   git clone https://github.com/mielsense/rtype/
   cd rtype

   # CMake will automatically handle the vcpkg installation
   # using the vcpkg.json file in the project root
   ```

2. **Manual Installation**:
   ```bash
   # For Linux:
   vcpkg install sfml:x64-linux fmt:x64-linux gtest:x64-linux imgui-sfml:x64-linux \
   enet:x64-linux asio:x64-linux argparse:x64-linux nlohmann-json:x64-linux

   # For Windows:
   vcpkg install sfml:x64-windows fmt:x64-windows gtest:x64-windows imgui-sfml:x64-windows \
   enet:x64-windows asio:x64-windows argparse:x64-windows nlohmann-json:x64-windows
   ```

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/mielsense/rtype/
cd rtype
```

2. Build the project:
```bash
mkdir build && cd build
cmake ..
make
```

### Running the Game

1. Start the Master Server:
```bash
./r-type_master_server [options]
```

2. Launch a Game Server:
```bash
./r-type_server [options]
```

3. Start the Client:
```bash
./r-type_client
```

### Docker Deployment

Build and run using Docker:

```bash
# Build Master Server
docker build . -t godlyjaaaaj/r-type_master_server --target r-type_master_server

# Build Game Server
docker build . -t godlyjaaaaj/r-type_server --target r-type_server

# Run Master Server
docker run -d --name r-type_master_server godlyjaaaaj/r-type_master_server

# Run Game Server
docker run -d --name r-type_server godlyjaaaaj/r-type_server
```

### Kubernetes Deployment

Deploy on Kubernetes:

```bash
kubectl apply -f r-type_server.configmap.yaml
kubectl apply -f r-type_master_server.deployment.yaml
kubectl apply -f r-type_server.deployment.yaml
```

Or use the provided script:
```bash
./kube/deploy_on_kube.sh
```

## Server Configuration

### Master Server Arguments

| Argument | Description | Default |
|----------|-------------|---------|
| --server-ip | IP address for master server | 0.0.0.0 |
| --server-port | Port for master server | 4242 |
| --inter-server-ip | IP for inter-server communication | 0.0.0.0 |
| --inter-server-port | Port for inter-server communication | 4243 |
| --tickrate | Server tickrate | 60 |
| --disable-console-logging | Disable console logging | false |
| --logfile | Log file path | "" |

### Game Server Arguments

| Argument | Description | Default |
|----------|-------------|---------|
| --server-name | Name of the game server | GameServer |
| --max-players | Maximum number of players | 4 |
| --server-ip | Game server IP | 0.0.0.0 |
| --server-port | Game server port | 4244 |
| --master-ip | Master server IP | localhost |
| --master-port | Master server port | 4243 |
| --tickrate | Server tickrate | 60 |

## Map Editor

<img width="1612" alt="SCR-20241217-mcvd" src="https://github.com/user-attachments/assets/5fb566bf-e374-4580-90e2-847b6289b7ca" />

The integrated map editor allows you to:
- Create custom levels through drag-and-drop
- Define enemy spawn points and patterns
- Set up background layers and scrolling effects
- Save and load map configurations

## Team

This project was developed by a team of 5 EPITECH students (including myself)

## License

This project is licensed under the terms of the [LICENSE](LICENSE) file included in this repository.
